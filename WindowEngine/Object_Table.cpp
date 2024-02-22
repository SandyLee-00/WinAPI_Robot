#include "pch.h"
#include "Object_Table.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Texture.h"

Object_Table::Object_Table(GameProcess* gameProcess, Texture* textureTable)
{
	// 씬에서 생성할 때부터 테이블 텍스쳐를 넣고 생성
	SetTexture(textureTable);

	// Collider 생성
	CreateCollider();

	// Collider Scale / OffsetPos 설정
	GetBoxCollider()->SetScale(Vector2((float)textureTable->GetWidth(), (float)textureTable->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)textureTable->GetWidth() / 2, (float)textureTable->GetHeight() / 2));

	SetName(L"Object_Table");
}

Object_Table::~Object_Table()
{

}

void Object_Table::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Table::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	Object* otherObject = _other->GetOwner();

	// Player인지 확인하고 밀기
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);
	}
}

void Object_Table::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_Table::Update(GameProcess* _gameProcess)
{

}

void Object_Table::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();

	TransparentBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, width, height, RGB(255, 0, 255));

		// Collider 랜더링
	ComponentRender(_dc);
}
