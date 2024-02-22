#include "pch.h"
#include "Object_Wall.h"
#include "BoxCollider.h"
#include "CollisionManager.h"


Object_Wall::Object_Wall(GameProcess* gameProcess)
{
	// Collider ����
	CreateCollider();

	// Collider Scale / OffsetPos �����ϱ� -> texture�� ��� ������ ���ֱ�
}

Object_Wall::~Object_Wall()
{

}

void Object_Wall::Update(GameProcess* _gameProcess)
{

}

void Object_Wall::Render(HDC _dc)
{
	ComponentRender(_dc);
}

void Object_Wall::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{
	
}

void Object_Wall::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	CollisionManager::PushBox(GetBoxCollider(), _other);
}
