#include "pch.h"
#include "Object_Dish.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "KeyManager.h"
#include "Object_Pizza.h"
#include "EventManager.h"
#include "Object_Order.h"
#include "SoundManager.h"
#include "Object_Player.h"

Object_Dish::Object_Dish(GameProcess* gameProcess, Object_Order* _order)
	: m_orderSystem(_order)
	, m_eventManager(gameProcess->GetEvenetManager())
{
	// Texture 로드
	Texture* texture = gameProcess->GetResourceManager()->LoadTexture(L"Object_Dish", L"\\Texture\\Tile\\dish.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
	SetTexture(texture);

	// Collider 생성
	CreateCollider();
	GetBoxCollider()->SetScale(Vector2((float)texture->GetWidth(), (float)texture->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)texture->GetWidth() / 2, (float)texture->GetHeight() / 2));

	SetName(L"Object_Dish");
}

Object_Dish::~Object_Dish()
{

}

void Object_Dish::CheckOk()
{
	// 체크 완료 후 객체 삭제 호출
	DeleteFood(m_eventManager);
}

void Object_Dish::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Dish::OnCollision(BoxCollider* _other, GameProcess* _gameProcess)
{
	KeyManager* keyManager = _gameProcess->GetKeyManager();
	EventManager* eventManager = _gameProcess->GetEvenetManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	Object* otherObject = _other->GetOwner();

	// Player인지 확인하고 밀기
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);
	}

	// !Player || ![SPACE] : 바로 빠져나가기
	if (otherObject->GetName() != L"Object_Player" || !keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	if (otherObject->GetPizza() == nullptr)
	{
		Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
		player->SetPopMessage(L"피자를 놓아야 해!");
		return;
	}

	// 접시에 피자가 없을 때 && 플레이어가 피자를 들고 있을 때
	if (m_pizza == nullptr && otherObject->GetPizza() != nullptr)
	{
		soundManager->PlaySound(L"SE_action", SOUND_TYPE::PLAYER);

		m_pizza = otherObject->GetPizza(); // 피자 받아오고
		m_pizza->SetOwner(this); // 피자의 오너를 접시로 설정
		otherObject->SetPizza(nullptr); // 플레이어의 피자를 없앰

		// 시스템에 피자 정보 넘김
		m_orderSystem->OrderCheck(this, m_pizza->GetPizzaName(), m_pizza->GetPizzaBaked());
	}

}

void Object_Dish::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Dish::Update(GameProcess* _gameProcess)
{

}

void Object_Dish::Render(HDC _dc)
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