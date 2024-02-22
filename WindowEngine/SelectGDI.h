#pragma once


/// <summary>
/// GDI ������ ���ϰ� ���ִ� Ŭ����
/// </summary>
class SelectGDI
{

private:
	/// ��� ����
	HDC  m_hDC;
	HPEN m_hDefaultPen;
	HBRUSH m_hDefaultBrush;
	HFONT m_hDefaultFont;

public:

	/// ������
	SelectGDI(HDC _dc, PEN_TYPE _penType);
	SelectGDI(HDC _dc, BRUSH_TYPE _brushType);
	SelectGDI(HDC _dc, FONT_TYPE _fontType, COLORREF _textColor);

	/// �Ҹ���
	~SelectGDI();
};

