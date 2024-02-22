#include "pch.h"
#include "Object_BarUI.h"
#include "SelectGDI.h"

Object_BarUI::Object_BarUI(GameProcess* gameProcess)
	:m_barLen(0.f)
	,m_brush()
	,m_pen()
	,m_offset{}
{
}

Object_BarUI::~Object_BarUI()
{
}

void Object_BarUI::Update(GameProcess* _gameProcess)
{
	// 마이너스 체력 상태는 0으로 표시
	if (m_barLen < 0.f)
		m_barLen = 0.f;

	m_barLen = Math::Clamp(m_barLen, 0.f, 100.f);
}

void Object_BarUI::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	Vector2 scale = GetScale();

	// 외부 막대기
	{
		SelectGDI brush(_dc, BRUSH_TYPE::BROWN);
		SelectGDI pen(_dc, PEN_TYPE::BROWN);
		Rectangle(_dc
			, (int)pos.x
			, (int)pos.y
			, (int)(pos.x + scale.x)
			, (int)(pos.y + scale.y));
	}

	// 내부막대기 밑 레이어 
	SelectGDI pen(_dc, PEN_TYPE::WHITE);
	Rectangle(_dc
		, (int)(pos.x + m_offset.x / 2)
		, (int)(pos.y + m_offset.y / 2)
		, (int)(pos.x + scale.x - m_offset.x / 2)
		, (int)(pos.y + scale.y - m_offset.y / 2));

	// 내부 막대기
	{
		SelectGDI pen(_dc, m_pen);
		SelectGDI brush(_dc, m_brush);

		float dist = (scale.x - m_offset.x) * m_barLen / 100;

		Rectangle(_dc
			, (int)(pos.x + m_offset.x / 2)
			, (int)(pos.y + m_offset.y / 2)
			, (int)(pos.x + dist+m_offset.x/2)
			, (int)(pos.y + scale.y - m_offset.y / 2));
	}
}
