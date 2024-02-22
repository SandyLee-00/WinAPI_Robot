#pragma once
#include "Object_UI.h"
class Object_BarUI :
    public Object_UI
{
private:
	float m_barLen;
    Vector2 m_offset;
    BRUSH_TYPE m_brush;
    PEN_TYPE   m_pen;

public:
    Object_BarUI(GameProcess* gameProcess);
    virtual~Object_BarUI();

    void SetBarLen(float _len) { m_barLen = _len; }
    float GetBarLen() { return m_barLen; }
    void SetPenBrushColor(PEN_TYPE _pen, BRUSH_TYPE _brush) { m_pen = _pen; m_brush = _brush; }
    void SetOffSet(Vector2 _offset) { m_offset = _offset; }

    void Update(GameProcess* _gameProcess) override;
    void Render(HDC _dc) override;
};

