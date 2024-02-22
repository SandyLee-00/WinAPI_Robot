#include "pch.h"
#include "Object_Oven.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "KeyManager.h"
#include "Object_Food.h"
#include "Object_Pizza.h"
#include "Object_Player.h"
#include "EventManager.h"
#include "TimeManager.h"
#include "Object_TextureBarUI.h"
#include "Scene_Stage01.h"
#include "SceneManager.h"
#include "SoundManager.h"

Object_Oven::Object_Oven(GameProcess* _gameProcess, Object_TextureBarUI* _ovenTimerBar, int _type)
	:m_ovenTime {},
	m_type(_type),
	m_ovenState(OVENSTATE::EMPTY),
	m_ovenTimerBar(nullptr),
	m_isBaking(),
	m_isBaked(),
	m_isOverCooked()
{
	// Texture 로드
	ResourceManager* _resourceManager = _gameProcess->GetResourceManager();
	PathManager* _pathManager = _gameProcess->GetPathManager();
	HDC _mainDC = _gameProcess->GetMainDC();

	// 텍스쳐 잘라서 로드하기
	m_tex = _resourceManager->LoadTexture(L"Object_Oven", L"\\Texture\\Tile\\stove_2.bmp", _pathManager, _mainDC);

	// Collider 생성
	CreateCollider();

	// Collider Scale / OffsetPos 설정
	GetBoxCollider()->SetScale(Vector2((float)m_tex->GetWidth() / 3, (float)m_tex->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)m_tex->GetWidth() / 6, (float)m_tex->GetHeight() / 2));

	SetName(L"Object_Oven");

	// 피자 굽는 타이머 바 연결하고 초기화하기
	m_ovenTimerBar = _ovenTimerBar;
}

Object_Oven::~Object_Oven()
{

}

void Object_Oven::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Oven::OnCollision(BoxCollider* _other, GameProcess* _gameProcess)
{
	KeyManager* _keyManager = _gameProcess->GetKeyManager();
	EventManager* _eventManager = _gameProcess->GetEvenetManager();
	SoundManager* _soundManager = _gameProcess->GetSoundManager();

	Object* otherObject = _other->GetOwner();

	// Player인지 확인하고 밀기
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);
	}

	// !Player || ![SPACE] : 바로 빠져나가기
	if (otherObject->GetName() != L"Object_Player" || !_keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	// 플레이어가 들고있는게 토핑이면 팝업 메세지 출력
	if (otherObject->GetTopping() != nullptr)
	{
		Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
		if (player != nullptr) player->SetPopMessage(L"피자만 구울 수 있어!");

		return;
	}

	// 1. 오븐 비어있을 때 안 구워진 피자 넣기 -> UNBAKED -> UNBAKED/BAKED/OVERBAKED
	// 2. 오븐 비어있을 떄 이미 구워진 피자 넣기 -> OVERBAKED
	// 3. 오븐에서 플레이어가 피자 꺼내기

	// Player && [SPACE]
	switch (m_ovenState)
	{
	case OVENSTATE::EMPTY:
	{
		// 1. 오븐 비어있을 때 안 구워진 피자 넣기 -> UNBAKED -> UNBAKED/BAKED/OVERBAKED
		if (GetPizza() == nullptr &&
			otherObject->GetPizza() != nullptr &&
			otherObject->GetPizza()->GetPizzaToppingSize() > 3 &&
			otherObject->GetPizza()->GetPizzaBaked() == PIZZABAKE::UNBAKED
			)
		{
			// 피자 오븐에 넣기
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			otherObject->SetPizza(nullptr);

			// 오븐 상태 변경
			m_ovenState = OVENSTATE::BAKING_UNBAKED;
		}

		// 2. 오븐 비어있을 떄 이미 구워진 피자 넣기 -> OVERBAKED
		else if (GetPizza() == nullptr &&
			otherObject->GetPizza() != nullptr &&
			otherObject->GetPizza()->GetPizzaToppingSize() > 3 &&
			otherObject->GetPizza()->GetPizzaBaked() == PIZZABAKE::BAKED)
		{
			// 피자 오븐에 넣기
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			otherObject->SetPizza(nullptr);

			// 오븐 상태 변경
			m_ovenState = OVENSTATE::OVERCOOKING;

			// TODO : 신뢰도 -10
			Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
			scene->AddTrust(-20.f);
			_soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
		}
		break;
	}

	case OVENSTATE::BAKING_UNBAKED:
	{
		// 3. 오븐에서 플레이어가 피자 꺼내기
		if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() == nullptr)
		{
			if (m_type == 1)
			{
				_soundManager->Oven1_Pause();
				m_isBaking = false;
			}
			else if (m_type == 2)
			{
				_soundManager->Oven2_Pause();
				m_isBaking = false;
			}
			else if (m_type == 3)
			{
				_soundManager->Oven3_Pause();
				m_isBaking = false;
			}

			_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

			// 피자 꺼내기
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// 오븐 상태 변경
			m_ovenState = OVENSTATE::EMPTY;
		}
		break;
	}

	case OVENSTATE::BAKING_BAKED:
	{
		// 3. 오븐에서 플레이어가 피자 꺼내기
		if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() == nullptr)
		{
			// 피자 꺼내기
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// 다 구워진 상태에서 피자 꺼내기
			if (!m_isBaked)
			{
				if (m_type == 1)
				{
					_soundManager->Oven1_Pause();
					m_isBaking = false;
				}
				else if (m_type == 2)
				{
					_soundManager->Oven2_Pause();
					m_isBaking = false;
				}
				else if (m_type == 3)
				{
					_soundManager->Oven3_Pause();
					m_isBaking = false;
				}

				_soundManager->PlaySound(L"SE_Pizza", SOUND_TYPE::PLAYER);
				m_isBaked = true;
			}

			// 오븐 상태 변경
			m_ovenState = OVENSTATE::EMPTY;
		}
		break;
	}

	case OVENSTATE::OVERCOOKING:
	{
		// 3. 오븐에서 플레이어가 피자 꺼내기
		if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() == nullptr)
		{
			// 피자 꺼내기
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// 오븐 상태 변경
			m_ovenState = OVENSTATE::EMPTY;
		}
		break;
	}
	default:
		break;
	}
}

void Object_Oven::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Oven::Update(GameProcess* _gameProcess)
{
	UpdateTime(_gameProcess);
	// TODO : Animator 들어가서 불타는 효과
}

void Object_Oven::UpdateTime(GameProcess* _gameProcess)
{
	KeyManager* _keyManager = _gameProcess->GetKeyManager();
	TimeManager* _timeManager = _gameProcess->GetTimeManager();
	SoundManager* _soundManager = _gameProcess->GetSoundManager();

	// 1. 0 ~ 10초 : 안 구워진 상태 그대로 -> 타이머 바 길어지기
	// 2. 10 ~ 20초 : 구워진 상태 -> 타이머 바 MAX_LENGTH / 타이머 깜빡거리기
	// 3. 20초 ~ : 탄 상태 -> 타이머 바 MAX_LENGTH

	switch (m_ovenState)
	{
	case OVENSTATE::EMPTY:
	{
		m_ovenTime = 0.f;
		m_ovenTimerBar->SetBarLen(m_ovenTime * 5);
		break;
	}
	case OVENSTATE::BAKING_UNBAKED:
	{
		m_ovenTime += (float)_timeManager->GetDeltaTime();

		// 1. 0 ~ 10초 : 안 구워진 상태 그대로
		if (!m_isBaking)
		{
			if (m_type == 1)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN1, 0.1f); // ** 활활 타오르는 소리 ** //
				m_isBaking = true;
			}
			else if (m_type == 2)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN2, 0.1f); // ** 활활 타오르는 소리 ** //
				m_isBaking = true;
			}
			else if (m_type == 3)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN3, 0.1f); // ** 활활 타오르는 소리 ** //
				m_isBaking = true;
			}
		}

		// 타이머 바 길어지기
		m_ovenTimerBar->SetBarLen(m_ovenTime * 10);

		// 2. 10 ~ 20초 : 구워진 상태
		if (m_ovenTime >= BAKINGTIME)
		{
			SetOvenState(OVENSTATE::BAKING_BAKED);
			m_pizza->SetPizzaBaked(PIZZABAKE::BAKED);
		}
		break;
	}
	case OVENSTATE::BAKING_BAKED:
	{
		m_ovenTime += (float)_timeManager->GetDeltaTime();

		// 타이머 바 MAX_LENGTH / 타이머 깜빡거리기
		float flickTimeGap = 0.5f;

		bool isLightOn = (int)(m_ovenTime / flickTimeGap) % 2;
		m_ovenTimerBar->SetBarLen(100.f);

		if (isLightOn == true)
		{
			m_ovenTimerBar->SetBarLen(100.f);

		}
		else if(isLightOn == false)
		{
			m_ovenTimerBar->SetBarLen(0.f);
		}

		// 3. 20초 ~ : 탄 상태
		if (m_ovenTime >= OVERCOOKINGTIME)
		{
			if (!m_isOverCooked)
			{
				if (m_type == 1)
				{
					_soundManager->Oven1_Pause();
				}
				else if (m_type == 2)
				{
					_soundManager->Oven2_Pause();
				}
				else if (m_type == 3)
				{
					_soundManager->Oven3_Pause();
				}

				_soundManager->PlaySound(L"SE_Pizza_overcook", SOUND_TYPE::OVEN2); // 피자 타버린 사운드
				m_isOverCooked = true;
			}

			SetOvenState(OVENSTATE::OVERCOOKING);
			m_pizza->SetPizzaBaked(PIZZABAKE::OVERBAKED);

			// TODO : 신뢰도 -10
			Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
			_soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
			scene->AddTrust(-10.f);
		}
		break;
	}

	case OVENSTATE::OVERCOOKING:
	{
		// 타이머 바 MAX_LENGTH
		m_ovenTime = OVERCOOKINGTIME;

		m_ovenTimerBar->SetBarLen(100.f);

		m_ovenState = OVENSTATE::OVERCOOKING;
		m_pizza->SetPizzaBaked(PIZZABAKE::OVERBAKED);

		break;
	}
	default:
		break;
	}
}


void Object_Oven::Render(HDC _dc)
{
	Vector2 _pos = GetPos();

	int ovenTextureNumber = 0;

	switch (m_ovenState)
	{
	case OVENSTATE::EMPTY:
	{
		ovenTextureNumber = 0;
		break;
	}

	case OVENSTATE::BAKING_UNBAKED:
	{
		ovenTextureNumber = 1;
		break;
	}

	case OVENSTATE::BAKING_BAKED:
	{
		ovenTextureNumber = 1;
		break;
	}

	case OVENSTATE::OVERCOOKING:
	{
		ovenTextureNumber = 2;
		break;
	}
	default:
		break;
	}

	// 오븐 랜더링
	/// render
	TransparentBlt(_dc
		, (int)(_pos.x)
		, (int)(_pos.y)
		, (int)m_tex->GetWidth() / 3
		, (int)m_tex->GetHeight()
		, m_tex->GetDC()
		, SLICE * ovenTextureNumber
		, 0
		, (int)m_tex->GetWidth() / 3
		, (int)m_tex->GetHeight()
		, RGB(255, 0, 255)
	);

	// Collider 랜더링
	ComponentRender(_dc);
}
