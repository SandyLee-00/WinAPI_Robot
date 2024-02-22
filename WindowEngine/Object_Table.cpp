#include "pch.h"
#include "Object_Table.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Texture.h"

Object_Table::Object_Table(GameProcess* gameProcess, Texture* textureTable)
{
	// ������ ������ ������ ���̺� �ؽ��ĸ� �ְ� ����
	SetTexture(textureTable);

	// Collider ����
	CreateCollider();

	// Collider Scale / OffsetPos ����
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

	// Player���� Ȯ���ϰ� �б�
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

		// Collider ������
	ComponentRender(_dc);
}
