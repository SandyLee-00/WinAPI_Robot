
#include "pch.h"
#include "SelectGDI.h"
#include "GameProcess.h"


SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _penType)
	:m_hDC(_dc)
	,m_hDefaultFont(nullptr)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HPEN hPen = GameProcess::m_pen[(UINT)_penType];
	m_hDefaultPen = (HPEN)SelectObject(_dc, hPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _brushType)
	:m_hDC(_dc)
	, m_hDefaultFont(nullptr)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	HBRUSH hBrush = GameProcess::m_brush[(UINT)_brushType];
	m_hDefaultBrush = (HBRUSH)SelectObject(_dc, hBrush);
}

SelectGDI::SelectGDI(HDC _dc, FONT_TYPE _fontType, COLORREF _textColor)
	:m_hDC(_dc)
	, m_hDefaultFont(nullptr)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
{
	SetBkMode(_dc, 0);
	SetTextColor(_dc, _textColor);
	HFONT hFont = GameProcess::m_font[(UINT)_fontType];
	m_hDefaultFont = (HFONT)SelectObject(_dc, hFont);
}

SelectGDI::~SelectGDI()
{
	SetTextColor(m_hDC, RGB(0, 0, 0));
	SelectObject(m_hDC, m_hDefaultFont);
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);
}
