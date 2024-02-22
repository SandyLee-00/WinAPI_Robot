#include "pch.h"
#include "Object_CuttingBoard.h"

#include "Object_Food.h"
#include "Object_Topping.h"
#include "Object_Pizza.h"
#include "Object_Player.h"

#include "CollisionManager.h"
#include "ResourceManager.h"
#include "KeyManager.h"
#include "EventManager.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "SoundManager.h"

Object_CuttingBoard::Object_CuttingBoard(GameProcess* gameProcess)
{
	// Texture 로드
	Texture* texture = gameProcess->GetResourceManager()->LoadTexture(L"Object_CuttingBoard", L"\\Texture\\Tile\\cutting board.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
	SetTexture(texture);

	// Collider 생성
	CreateCollider();
	// Collider Scale / OffsetPos 설정
	GetBoxCollider()->SetScale(Vector2((float)texture->GetWidth(), (float)texture->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)texture->GetWidth() / 2, (float)texture->GetHeight() / 2));

	SetName(L"Object_CuttingBoard");
}

Object_CuttingBoard::~Object_CuttingBoard()
{

}

void Object_CuttingBoard::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_CuttingBoard::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	Object* otherObject = _other->GetOwner();
	KeyManager* _keyManager = gameProcess->GetKeyManager();
	EventManager* _eventManager = gameProcess->GetEvenetManager();
	SoundManager* _soundManager = gameProcess->GetSoundManager();

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

	// TODO : if문 정리 4 -> 2
	// 1. 비어있는 커팅보드에 도우를 올려놓는다 -> 피자 생성
	// 2. 비어있는 커팅보드에 피자를 내려놓는다
	// 3. 피자가 있는 커팅보드에 재료를 올려놓는다 -> 재료 삭제
	// 4. 피자가 있는 커팅보드에서 피자를 가져간다

	// 커팅보드에 피자가 없을 때
	if (nullptr == m_pizza)
	{
		// 피자를 들고 있는 상태면
		if (otherObject->GetPizza() != nullptr)
		{
			_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

			// 피자 커팅보드에 놓기
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			// TODO: 이거 문제인가?
			otherObject->SetPizza(nullptr);
		}
		// 재료를 들고 있는 상태면
		else if (otherObject->GetTopping() != nullptr)
		{
			// 도우일 경우 재료 삭제하고 피자 생성
			if (otherObject->GetTopping()->GetToppingKind() == TOPPING::DOUGH)
			{
				_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

				otherObject->DeleteFood(_eventManager);
				Object_Pizza* pizza = new Object_Pizza(gameProcess);
				m_pizza = pizza;
				pizza->SetOwner(this);
				_eventManager->CreateObject(pizza, OBJECT_TYPE::FOOD);
			}
			// 도우가 아닐경우 팝업 메세지 출력
			else
			{
				Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
				if (player != nullptr)
				{
					player->SetPopMessage(L"도우를 먼저 놓자!");
				}
				
			}
		}
	}
	// 커팅보드에 피자가 있을 때
	else if (nullptr != m_pizza)
	{
		// 플레이어에 피자 없는 상태 && 토핑 없는 상태
		if (otherObject->GetPizza() == nullptr && otherObject->GetTopping() == nullptr)
		{
			_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

			// 피자 플레이어에게 전달
			m_pizza->SetOwner(otherObject);
			otherObject->SetPizza(m_pizza);
			m_pizza = nullptr;
		}
		// 플레이어에 피자 없는 상태 && 토핑 있는 상태 && 피자 안 구워진 상태 && 토핑 추가 성공
		else if (otherObject->GetPizza() == nullptr &&
			otherObject->GetTopping() != nullptr &&
			m_pizza->GetPizzaBaked() == PIZZABAKE::UNBAKED)
		{
			Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
			if (player == nullptr) return;

			BASETOPPINGCHECK check = m_pizza->AddTopping(otherObject->GetTopping()->GetToppingKind());
			switch (check)
			{
				case BASETOPPINGCHECK::EMPTY:
				{
					player->SetPopMessage(L"기본 재료를 먼저 넣자!");
				}
					break;
				case BASETOPPINGCHECK::STAND:
				{
					player->SetPopMessage(L"이미 들어간 기본 재료야!");
				}
					break;
				case BASETOPPINGCHECK::FULL:
				{
					player->SetPopMessage(L"이제 맛있는 토핑을 넣어볼까?");
				}
					break;
				case BASETOPPINGCHECK::ALLFULL:
				{
					player->SetPopMessage(L"더이상 토핑을 추가할 수 없어!");
				}
					break;
				case BASETOPPINGCHECK::OK:
				{
					_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);
					// 들고있던 재료 삭제
					otherObject->DeleteFood(_eventManager);
				}
					break;
				case BASETOPPINGCHECK::END:
				{

				}
					break;
				default:
					break;
			}

			
		}
	}

}


void Object_CuttingBoard::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_CuttingBoard::Update(GameProcess* _gameProcess)
{

}

void Object_CuttingBoard::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();

	TransparentBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, width, height, RGB(255, 0, 255));

	if (nullptr != m_pizza)
		m_pizza->OwnerRender(_dc);

	// Collider 랜더링
	ComponentRender(_dc);
}
