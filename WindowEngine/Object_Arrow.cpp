#include "pch.h"
#include "Object_Arrow.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TimeManager.h"

Object_Arrow::Object_Arrow(GameProcess* _gameProcess)
	:m_addtime()
{
	m_tex = _gameProcess->GetResourceManager()->LoadTexture(L"Arrow", L"\\Texture\\Background\\arrow_resize.bmp"
		, _gameProcess->GetPathManager(), _gameProcess->GetMainDC());
}

Object_Arrow::~Object_Arrow()
{

}

void Object_Arrow::Update(GameProcess* _gameProcess)
{
	TimeManager* timeManager = _gameProcess->GetTimeManager();
	m_addtime += static_cast<float>(timeManager->GetDeltaTime());

	if (m_addtime >= 1.f)
		m_addtime = 0.f;
}

void Object_Arrow::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = m_tex->GetWidth();
	int height = m_tex->GetHeight();

	if (m_addtime >= 0.5f)
		TransparentBlt(_dc, (int)pos.x, (int)pos.y, width/2, height, m_tex->GetDC(), 0, 0, width/2, height, RGB(255, 0, 255));
	else
		TransparentBlt(_dc, (int)pos.x, (int)pos.y, width/2, height, m_tex->GetDC(), width / 2, 0, width/2, height, RGB(255, 0, 255));
}
