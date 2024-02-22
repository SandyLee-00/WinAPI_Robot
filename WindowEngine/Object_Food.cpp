#include "pch.h"
#include "Object.h"
#include "Object_Food.h"

Object_Food::Object_Food()
	: m_owner(nullptr)
	, m_pos(0, 0)
	, m_slice(0)
	, m_offset(0, 0)
	, m_lookdir(LOOKDIR::DOWN)
	, m_animOffset(0.f)
	, m_animOffsetTime(0.f)
	, m_animCount(0)
{

}

Object_Food::~Object_Food()
{

}

void Object_Food::Update(GameProcess* _gameProcess)
{

}

void Object_Food::Render(HDC _dc)
{

}

void Object_Food::OwnerRender(HDC _dc)
{

}


