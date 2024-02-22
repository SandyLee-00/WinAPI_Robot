#include "pch.h"
#include "Object_TextUI.h"
#include "SelectGDI.h"

void Object_TextUI::SetText(const wchar_t* _char)
{
	m_text = _char;
}

Object_TextUI::Object_TextUI(GameProcess* gameProcess)
{

}

Object_TextUI::~Object_TextUI()
{
}


void Object_TextUI::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	Vector2 scale = GetScale();

	SelectGDI font(_dc, FONT_TYPE::DOSPILGI_FONT,RGB(255,255,0));

	RECT rc = { (long)pos.x,(long)pos.y,(long)(scale.x*2),(long)(scale.y*2) };

	DrawTextW(_dc, m_text.c_str(), -1, &rc, DT_CENTER );
}
