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
	// Texture �ε�
	ResourceManager* _resourceManager = _gameProcess->GetResourceManager();
	PathManager* _pathManager = _gameProcess->GetPathManager();
	HDC _mainDC = _gameProcess->GetMainDC();

	// �ؽ��� �߶� �ε��ϱ�
	m_tex = _resourceManager->LoadTexture(L"Object_Oven", L"\\Texture\\Tile\\stove_2.bmp", _pathManager, _mainDC);

	// Collider ����
	CreateCollider();

	// Collider Scale / OffsetPos ����
	GetBoxCollider()->SetScale(Vector2((float)m_tex->GetWidth() / 3, (float)m_tex->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)m_tex->GetWidth() / 6, (float)m_tex->GetHeight() / 2));

	SetName(L"Object_Oven");

	// ���� ���� Ÿ�̸� �� �����ϰ� �ʱ�ȭ�ϱ�
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

	// Player���� Ȯ���ϰ� �б�
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);
	}

	// !Player || ![SPACE] : �ٷ� ����������
	if (otherObject->GetName() != L"Object_Player" || !_keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	// �÷��̾ ����ִ°� �����̸� �˾� �޼��� ���
	if (otherObject->GetTopping() != nullptr)
	{
		Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
		if (player != nullptr) player->SetPopMessage(L"���ڸ� ���� �� �־�!");

		return;
	}

	// 1. ���� ������� �� �� ������ ���� �ֱ� -> UNBAKED -> UNBAKED/BAKED/OVERBAKED
	// 2. ���� ������� �� �̹� ������ ���� �ֱ� -> OVERBAKED
	// 3. ���쿡�� �÷��̾ ���� ������

	// Player && [SPACE]
	switch (m_ovenState)
	{
	case OVENSTATE::EMPTY:
	{
		// 1. ���� ������� �� �� ������ ���� �ֱ� -> UNBAKED -> UNBAKED/BAKED/OVERBAKED
		if (GetPizza() == nullptr &&
			otherObject->GetPizza() != nullptr &&
			otherObject->GetPizza()->GetPizzaToppingSize() > 3 &&
			otherObject->GetPizza()->GetPizzaBaked() == PIZZABAKE::UNBAKED
			)
		{
			// ���� ���쿡 �ֱ�
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			otherObject->SetPizza(nullptr);

			// ���� ���� ����
			m_ovenState = OVENSTATE::BAKING_UNBAKED;
		}

		// 2. ���� ������� �� �̹� ������ ���� �ֱ� -> OVERBAKED
		else if (GetPizza() == nullptr &&
			otherObject->GetPizza() != nullptr &&
			otherObject->GetPizza()->GetPizzaToppingSize() > 3 &&
			otherObject->GetPizza()->GetPizzaBaked() == PIZZABAKE::BAKED)
		{
			// ���� ���쿡 �ֱ�
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			otherObject->SetPizza(nullptr);

			// ���� ���� ����
			m_ovenState = OVENSTATE::OVERCOOKING;

			// TODO : �ŷڵ� -10
			Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
			scene->AddTrust(-20.f);
			_soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
		}
		break;
	}

	case OVENSTATE::BAKING_UNBAKED:
	{
		// 3. ���쿡�� �÷��̾ ���� ������
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

			// ���� ������
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// ���� ���� ����
			m_ovenState = OVENSTATE::EMPTY;
		}
		break;
	}

	case OVENSTATE::BAKING_BAKED:
	{
		// 3. ���쿡�� �÷��̾ ���� ������
		if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() == nullptr)
		{
			// ���� ������
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// �� ������ ���¿��� ���� ������
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

			// ���� ���� ����
			m_ovenState = OVENSTATE::EMPTY;
		}
		break;
	}

	case OVENSTATE::OVERCOOKING:
	{
		// 3. ���쿡�� �÷��̾ ���� ������
		if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() == nullptr)
		{
			// ���� ������
			otherObject->SetPizza(m_pizza);
			m_pizza->SetOwner(otherObject);
			m_pizza = nullptr;

			// ���� ���� ����
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
	// TODO : Animator ���� ��Ÿ�� ȿ��
}

void Object_Oven::UpdateTime(GameProcess* _gameProcess)
{
	KeyManager* _keyManager = _gameProcess->GetKeyManager();
	TimeManager* _timeManager = _gameProcess->GetTimeManager();
	SoundManager* _soundManager = _gameProcess->GetSoundManager();

	// 1. 0 ~ 10�� : �� ������ ���� �״�� -> Ÿ�̸� �� �������
	// 2. 10 ~ 20�� : ������ ���� -> Ÿ�̸� �� MAX_LENGTH / Ÿ�̸� �����Ÿ���
	// 3. 20�� ~ : ź ���� -> Ÿ�̸� �� MAX_LENGTH

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

		// 1. 0 ~ 10�� : �� ������ ���� �״��
		if (!m_isBaking)
		{
			if (m_type == 1)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN1, 0.1f); // ** ȰȰ Ÿ������ �Ҹ� ** //
				m_isBaking = true;
			}
			else if (m_type == 2)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN2, 0.1f); // ** ȰȰ Ÿ������ �Ҹ� ** //
				m_isBaking = true;
			}
			else if (m_type == 3)
			{
				_soundManager->PlaySound(L"SE_Oven", SOUND_TYPE::OVEN3, 0.1f); // ** ȰȰ Ÿ������ �Ҹ� ** //
				m_isBaking = true;
			}
		}

		// Ÿ�̸� �� �������
		m_ovenTimerBar->SetBarLen(m_ovenTime * 10);

		// 2. 10 ~ 20�� : ������ ����
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

		// Ÿ�̸� �� MAX_LENGTH / Ÿ�̸� �����Ÿ���
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

		// 3. 20�� ~ : ź ����
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

				_soundManager->PlaySound(L"SE_Pizza_overcook", SOUND_TYPE::OVEN2); // ���� Ÿ���� ����
				m_isOverCooked = true;
			}

			SetOvenState(OVENSTATE::OVERCOOKING);
			m_pizza->SetPizzaBaked(PIZZABAKE::OVERBAKED);

			// TODO : �ŷڵ� -10
			Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
			_soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
			scene->AddTrust(-10.f);
		}
		break;
	}

	case OVENSTATE::OVERCOOKING:
	{
		// Ÿ�̸� �� MAX_LENGTH
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

	// ���� ������
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

	// Collider ������
	ComponentRender(_dc);
}
