#include "pch.h"
#include "Object_Credit.h"
#include "Texture.h"
#include "ResourceManager.h"

Object_Credit::Object_Credit(GameProcess* gameProcess)
{
	// 텍스처 로딩
	m_tex = gameProcess->GetResourceManager()->LoadTexture(L"CreditFace", L"\\Texture\\Background\\credit.bmp"
		, gameProcess->GetPathManager(), gameProcess->GetMainDC());
}

Object_Credit::~Object_Credit()
{
}

void Object_Credit::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();
	// 랜더링
	BitBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, SRCCOPY);
}

void Object_Credit::Update(GameProcess* _gameProcess)
{
}
