#include "pch.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "Object.h"
#include "SelectGDI.h"
#include "Object.h"

UINT BoxCollider::g_iNextID = 0;

BoxCollider::BoxCollider()
	:m_finalPos{}
	,m_ID(0)
	,m_offsetPos{}
	,m_owner(nullptr)
	,m_scale{}
	,m_isActive(false)
	,m_collisionCount(0)
{	
	m_ID = g_iNextID++;
}

BoxCollider::BoxCollider(const BoxCollider& _origin)
	:m_finalPos(_origin.m_finalPos)
	,m_ID(0)
	,m_isActive{_origin.m_isActive}
	,m_offsetPos{_origin.m_offsetPos}
	,m_owner{nullptr}
	,m_scale{_origin.m_scale}
	, m_collisionCount(0)
{
	m_ID = g_iNextID++;
}

BoxCollider::~BoxCollider()
{}


void BoxCollider::FinalUpdate()
{
	// 오브젝트와 충돌체 위치 동기화
	Vector2 objectPos = m_owner->GetPos();
	m_finalPos = objectPos + m_offsetPos;

	// 층돌카운트가 0보다 작으면 버그
	assert(0 <= m_collisionCount);
}

void BoxCollider::Render(HDC _dc)
{
#ifdef _DEBUG
	//PEN_TYPE p = PEN_TYPE::GREEN;
	//if (m_collisionCount > 0)
	//	p = PEN_TYPE::RED;

	//SelectGDI pen(_dc, p);
	//SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);

	//Rectangle(_dc
	//	, (int)(m_finalPos.x - m_scale.x / 2.f)
	//	, (int)(m_finalPos.y - m_scale.y / 2.f)
	//	, (int)(m_finalPos.x + m_scale.x / 2.f)
	//	, (int)(m_finalPos.y + m_scale.y / 2.f));
#endif 

}



void BoxCollider::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	m_owner->OnCollision(_other, gameProcess);
}

void BoxCollider::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{
	--m_collisionCount;
	m_owner->OnCollisionExit(_other, gameProcess);
}

void BoxCollider::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{
	++m_collisionCount;
	m_owner->OnCollisionEnter(_other, gameProcess);
}

//BoxCollider::BoxCollider()
//	: m_owner(nullptr)
//	, m_offsetPos(Vector2::Zero)
//	, m_finalPos(Vector2::Zero)
//	, m_scale(Vector2::One)
//	, m_ID(0)
//	, m_isActive(true)
//{
//	m_ID = CollisionMgr::GetInst()->AssignColliderID();
//}

