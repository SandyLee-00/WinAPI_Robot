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
	// Texture �ε�
	Texture* texture = gameProcess->GetResourceManager()->LoadTexture(L"Object_CuttingBoard", L"\\Texture\\Tile\\cutting board.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
	SetTexture(texture);

	// Collider ����
	CreateCollider();
	// Collider Scale / OffsetPos ����
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

	// TODO : if�� ���� 4 -> 2
	// 1. ����ִ� Ŀ�ú��忡 ���츦 �÷����´� -> ���� ����
	// 2. ����ִ� Ŀ�ú��忡 ���ڸ� �������´�
	// 3. ���ڰ� �ִ� Ŀ�ú��忡 ��Ḧ �÷����´� -> ��� ����
	// 4. ���ڰ� �ִ� Ŀ�ú��忡�� ���ڸ� ��������

	// Ŀ�ú��忡 ���ڰ� ���� ��
	if (nullptr == m_pizza)
	{
		// ���ڸ� ��� �ִ� ���¸�
		if (otherObject->GetPizza() != nullptr)
		{
			_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

			// ���� Ŀ�ú��忡 ����
			m_pizza = otherObject->GetPizza();
			m_pizza->SetOwner(this);
			// TODO: �̰� �����ΰ�?
			otherObject->SetPizza(nullptr);
		}
		// ��Ḧ ��� �ִ� ���¸�
		else if (otherObject->GetTopping() != nullptr)
		{
			// ������ ��� ��� �����ϰ� ���� ����
			if (otherObject->GetTopping()->GetToppingKind() == TOPPING::DOUGH)
			{
				_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

				otherObject->DeleteFood(_eventManager);
				Object_Pizza* pizza = new Object_Pizza(gameProcess);
				m_pizza = pizza;
				pizza->SetOwner(this);
				_eventManager->CreateObject(pizza, OBJECT_TYPE::FOOD);
			}
			// ���찡 �ƴҰ�� �˾� �޼��� ���
			else
			{
				Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
				if (player != nullptr)
				{
					player->SetPopMessage(L"���츦 ���� ����!");
				}
				
			}
		}
	}
	// Ŀ�ú��忡 ���ڰ� ���� ��
	else if (nullptr != m_pizza)
	{
		// �÷��̾ ���� ���� ���� && ���� ���� ����
		if (otherObject->GetPizza() == nullptr && otherObject->GetTopping() == nullptr)
		{
			_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

			// ���� �÷��̾�� ����
			m_pizza->SetOwner(otherObject);
			otherObject->SetPizza(m_pizza);
			m_pizza = nullptr;
		}
		// �÷��̾ ���� ���� ���� && ���� �ִ� ���� && ���� �� ������ ���� && ���� �߰� ����
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
					player->SetPopMessage(L"�⺻ ��Ḧ ���� ����!");
				}
					break;
				case BASETOPPINGCHECK::STAND:
				{
					player->SetPopMessage(L"�̹� �� �⺻ ����!");
				}
					break;
				case BASETOPPINGCHECK::FULL:
				{
					player->SetPopMessage(L"���� ���ִ� ������ �־��?");
				}
					break;
				case BASETOPPINGCHECK::ALLFULL:
				{
					player->SetPopMessage(L"���̻� ������ �߰��� �� ����!");
				}
					break;
				case BASETOPPINGCHECK::OK:
				{
					_soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);
					// ����ִ� ��� ����
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

	// Collider ������
	ComponentRender(_dc);
}
