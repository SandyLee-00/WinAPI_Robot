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
	// Texture 로드
	Texture* texture = gameProcess->GetResourceManager()->LoadTexture(L"Object_FoodBox", L"\\Texture\\Tile\\Food_box.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
	SetTexture(texture);

	// Collider 생성
	CreateCollider();

	// Collider Scale / OffsetPos 설정
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

	// Player인지 확인
	// TODO : Food박스가 플레이어 밀지 얘기하고 결정하기
// 	if (otherObject->GetName() == L"Object_Player")
// 	{
// 		// 밀기
// 		CollisionManager::PushBox(GetBoxCollider(), _other);
// 	}

	// !Player || ![SPACE] : 바로 빠져나가기
	if (otherObject->GetName() != L"Object_Player" || !keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	// TODO : 플레이어인지 확인하기
	// Player && Interaction

	//Topping과 Pizza 없는지 확인
	if (otherObject->GetPizza() == nullptr && otherObject->GetTopping() == nullptr)
	{
		soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

		// 토핑 생성
		Object_Topping* topping = new Object_Topping(m_thisTopping, gameProcess);
		topping->SetOwner(otherObject);
		otherObject->SetTopping(topping);
		gameProcess->GetEvenetManager()->CreateObject((Object*)topping, OBJECT_TYPE::FOOD);
		return;
	}

	// Topping 있는지 확인
	if (otherObject->GetTopping() != nullptr)
	{
		// 같은 종류의 토핑 삭제
		if (otherObject->GetTopping()->GetToppingKind() == m_thisTopping)
		{
			soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);
			otherObject->DeleteFood(gameProcess->GetEvenetManager()); // Player의 Topping을 비움
		}
		else
		{
			Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
			if (player != nullptr) player->SetPopMessage(L"같은 재료가 아냐!");
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

	// Collider 랜더링
	ComponentRender(_dc);
}
