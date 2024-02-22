#include "pch.h"
#include "Object.h"
#include "Animator.h"
#include "Texture.h"
#include "BoxCollider.h"

#include "EventManager.h"
#include "Object_Food.h"

Object::Object()
	: m_alive(true)
	, m_name()
	, m_pos{}
	, m_scale{}
	, m_lookdir()
	, m_animator(nullptr)
	, m_tex(nullptr)
	, m_boxCollider(nullptr)
	, m_pizza(nullptr)
	, m_topping(nullptr)
{
}

Object::Object(const Object& _origin)
	: m_alive(true)
	, m_name()
	, m_pos{}
	, m_scale{}
	, m_animator(nullptr)
	, m_tex(_origin.m_tex)
	, m_boxCollider(nullptr)
	, m_lookdir()
	, m_pizza(nullptr)
	, m_topping(nullptr)
{

	if (_origin.m_boxCollider)
	{
		m_boxCollider = new BoxCollider(*_origin.m_boxCollider);
		m_boxCollider->m_owner = this;
	}

	if (_origin.m_animator)
	{
		m_animator = new Animator(*_origin.m_animator);
		m_animator->m_owner = this;
	}
}

Object::~Object()
{

	if (nullptr != m_boxCollider)
	{
		delete m_boxCollider;
	}

	if (nullptr != m_animator)
	{
		delete m_animator;
	}
}

void Object::DeleteFood(EventManager* _eventManager)
{
	// 피자랑 토핑이 nullptr 상태가 아닌 경우 삭제해줌
	if (nullptr != m_pizza)
	{
		_eventManager->DeleteObject((Object*)m_pizza);
		m_pizza = nullptr;
	}
	if (nullptr != m_topping)
	{
		_eventManager->DeleteObject((Object*)m_topping);
		m_topping = nullptr;
	}
}

Animator* Object::CreateAnimator()
{
	m_animator = new Animator();
	m_animator->m_owner = this;

	return m_animator;
}

void Object::CreateCollider()
{
	m_boxCollider = new BoxCollider();
	m_boxCollider->m_owner = this;
}


void Object::Finalupdate(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager)
{
	if (m_boxCollider)
		m_boxCollider->FinalUpdate();

	if (m_animator)
		m_animator->FinalUpdate(_deltaTime);
}

void Object::Render(HDC _dc)
{
	ComponentRender(_dc);
}

void Object::ComponentRender(HDC _dc)
{
	if (nullptr != m_animator)
	{
		m_animator->Render(_dc);
	}

	if (nullptr != m_boxCollider)
	{
		m_boxCollider->Render(_dc);
	}
}

