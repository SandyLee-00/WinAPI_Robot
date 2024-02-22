#include "pch.h"
#include "Object.h"
#include "Object_Food.h"
#include "Object_FoodBox.h"
#include "Object_Topping.h"
#include "Object_Player.h"

#include "CollisionManager.h"
#include "EventManager.h"
#include "KeyManager.h"
#include "ResourceManager.h"

#include "BoxCollider.h"
#include "Texture.h"
#include "SoundManager.h"

const int foodBoxScale = 100;

Object_FoodBox::Object_FoodBox(GameProcess* gameProcess)
	:m_thisTopping(TOPPING::NONE)
{

}

Object_FoodBox::Object_FoodBox(TOPPING _topping, GameProcess* gameProcess)
	: m_thisTopping(_topping)
{
	// Texture �ε�
	Texture* texture = gameProcess->GetResourceManager()->LoadTexture(L"Object_FoodBox", L"\\Texture\\Tile\\Food_box.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
	SetTexture(texture);

	// Collider ����
	CreateCollider();

	// Collider Scale / OffsetPos ����
	GetBoxCollider()->SetScale(Vector2(foodBoxScale, foodBoxScale));
	GetBoxCollider()->SetOffsetPos(Vector2(foodBoxScale / 2, foodBoxScale / 2));

	SetName(L"Object_FoodBox");
}

Object_FoodBox::~Object_FoodBox()
{

}

void Object_FoodBox::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_FoodBox::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	SoundManager* soundManager = gameProcess->GetSoundManager();
	KeyManager* keyManager = gameProcess->GetKeyManager();
	Object* otherObject = _other->GetOwner();

	// Player���� Ȯ��
	// TODO : Food�ڽ��� �÷��̾� ���� ����ϰ� �����ϱ�
// 	if (otherObject->GetName() == L"Object_Player")
// 	{
// 		// �б�
// 		CollisionManager::PushBox(GetBoxCollider(), _other);
// 	}

	// !Player || ![SPACE] : �ٷ� ����������
	if (otherObject->GetName() != L"Object_Player" || !keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	// TODO : �÷��̾����� Ȯ���ϱ�
	// Player && Interaction

	//Topping�� Pizza ������ Ȯ��
	if (otherObject->GetPizza() == nullptr && otherObject->GetTopping() == nullptr)
	{
		soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

		// ���� ����
		Object_Topping* topping = new Object_Topping(m_thisTopping, gameProcess);
		topping->SetOwner(otherObject);
		otherObject->SetTopping(topping);
		gameProcess->GetEvenetManager()->CreateObject((Object*)topping, OBJECT_TYPE::FOOD);
		return;
	}

	// Topping �ִ��� Ȯ��
	if (otherObject->GetTopping() != nullptr)
	{
		// ���� ������ ���� ����
		if (otherObject->GetTopping()->GetToppingKind() == m_thisTopping)
		{
			soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);
			otherObject->DeleteFood(gameProcess->GetEvenetManager()); // Player�� Topping�� ���
		}
		else
		{
			Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
			if (player != nullptr) player->SetPopMessage(L"���� ��ᰡ �Ƴ�!");
		}

		return;
	}
}

void Object_FoodBox::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_FoodBox::Update(GameProcess* _gameProcess)
{

}

void Object_FoodBox::Render(HDC _dc)
{
	Vector2 pos = GetPos();

	TransparentBlt(_dc
		, (int)(pos.x)
		, (int)(pos.y)
		, foodBoxScale
		, foodBoxScale
		, m_tex->GetDC()
		, foodBoxScale * (int)m_thisTopping
		, 0
		, foodBoxScale
		, foodBoxScale
		, RGB(255, 0, 255)
	);

	// Collider ������
	ComponentRender(_dc);
}
