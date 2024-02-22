#include "pch.h"
#include "Object_Title.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TimeManager.h"
#include "KeyManager.h"

Object_Title::Object_Title(GameProcess* _gameProcess)
	:m_subTitle(nullptr)
	, m_mousePos{}
{
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();
	m_subTitle = resourceManager->LoadTexture(L"TitleSub", L"\\Texture\\title2.bmp", pathManager, mainDC);
	m_tex = resourceManager->LoadTexture(L"Title", L"\\Texture\\title1.bmp", pathManager, mainDC);
	SetPos(Vector2(0.f, 0.f));
}

Object_Title::~Object_Title()
{

}

void Object_Title::Update(GameProcess* _gameProcess)
{
	m_mousePos = _gameProcess->GetKeyManager()->GetMousePos();
}

void Object_Title::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();

	if (m_mousePos.x > 710.f && m_mousePos.x < 1110.f && m_mousePos.y > 600.f)
		BitBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_subTitle->GetDC(), 0, 0, SRCCOPY);
	else
		BitBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, SRCCOPY);

}
