#include "pch.h"
#include "Object_Background.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GameProcess.h"

Object_Background::Object_Background(GameProcess* gameProcess)
{
	// 텍스처 로딩
	m_tex = gameProcess->GetResourceManager()->LoadTexture(L"Background_Kitchen", L"\\Texture\\Background\\Background_2.bmp"
		,gameProcess->GetPathManager(), gameProcess->GetMainDC());
}

Object_Background::~Object_Background()
{

}

void Object_Background::Update(GameProcess* _gameProcess)
{

}

void Object_Background::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();
	// 랜더링
	BitBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, SRCCOPY);
}
