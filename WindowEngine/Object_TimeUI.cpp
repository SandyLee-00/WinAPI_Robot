#include "pch.h"
#include "Object_TimeUI.h"
#include "SelectGDI.h"
#include "TimeManager.h"
#include "GameProcess.h"

Object_TimeUI::Object_TimeUI(GameProcess* gameProcess)
	:m_hour(0)
	,m_minute(0)
{
}

Object_TimeUI::~Object_TimeUI()
{
}

void Object_TimeUI::SetMinute(int _minute)
{
	m_hour = (_minute / 60) % 24;
	m_minute = _minute % 60;
}

void Object_TimeUI::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	// 시간을 문자열로 변환
	wchar_t timeStr[8];
	swprintf_s(timeStr, L"%02d:%02d", m_hour, m_minute);

	SelectGDI font(_dc, FONT_TYPE::GALMURI7_FONT,RGB(89,33,33));
	TextOutW(_dc, (int)pos.x, (int)pos.y, timeStr, (int)wcslen(timeStr));
}

void Object_TimeUI::Update(GameProcess* _gameProcess)
{
	static double time =0.;
	_gameProcess->GetTimeManager()->GetDeltaTime();
	time += _gameProcess->GetTimeManager()->GetDeltaTime();
	if (time >= 1.)
	{
		time -= 1.;
		++m_minute;
	}

	if (m_minute >= 60)
	{
		m_hour += 1;
		m_minute = 0;
	}
}

