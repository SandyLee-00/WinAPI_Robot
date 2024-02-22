#include "pch.h"
#include "Object_TextureBarUI.h"
#include "Texture.h"
#include "TimeManager.h"

Object_TextureBarUI::Object_TextureBarUI(Texture* _emptyTexture, Texture* _fillTexture)
	:m_offset{}
	,m_flipTime(0.f)
{
	assert(_emptyTexture);
	assert(_fillTexture);

	SetEmptyBarTexture(_emptyTexture);
	SetFillBarTexture(_fillTexture);
}

Object_TextureBarUI::~Object_TextureBarUI()
{
}

void Object_TextureBarUI::Render(HDC _dc)
{
	Vector2 pos = GetPos();

	int widthEmpty = m_emptyBar->GetWidth();
	int heightEmpty = m_emptyBar->GetHeight();

	// 빈바 랜더링
	TransparentBlt(_dc, static_cast<int>(pos.x), static_cast<int>(pos.y)
		, widthEmpty, heightEmpty, m_emptyBar->GetDC()
		, 0, 0, widthEmpty, heightEmpty, RGB(255, 0, 255));

	int widthFill = m_fillBar->GetWidth();
	int heightFill = m_fillBar->GetHeight();

	// m_barLen이 20이하 이면 딸피효과를 부여
	if (m_barLen <= 20.f && m_flipTime >= 0.25f)
		return;

	// BarLen 에따라서 출력하는 범위를 지정
	int renderLength = widthFill * (int)m_barLen / 100;
	Vector2 fillBarPos = pos + m_offset;

	// 채우는바 랜더링
	TransparentBlt(_dc, static_cast<int>(fillBarPos.x), static_cast<int>(fillBarPos.y), renderLength, heightFill
		, m_fillBar->GetDC(), 0, 0, renderLength, heightFill, RGB(255, 0, 255));

}

void Object_TextureBarUI::Update(GameProcess* _gameProcess)
{
	m_barLen = Math::Clamp<float>(m_barLen, 0.f, 100.f);
	m_flipTime += static_cast<float>(_gameProcess->GetTimeManager()->GetDeltaTime());

	if (m_flipTime >= 0.5f)
		m_flipTime = 0.f;
}
