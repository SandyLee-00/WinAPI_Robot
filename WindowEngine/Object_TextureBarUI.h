#pragma once
#include "Object_UI.h"

class Texture;

class Object_TextureBarUI :
    public Object_UI
{
private:
    // 배경바
    Texture* m_emptyBar;
    // 색깔을 채우는 바
    Texture* m_fillBar;

    float m_barLen;
    Vector2 m_offset;

    float m_flipTime;

public:
    // 배경바 텍스터 설정
    void SetEmptyBarTexture(Texture* _texture) { m_emptyBar = _texture; }
    // 색깔을 채는바
    void SetFillBarTexture(Texture* _texture) { m_fillBar = _texture; }

    Vector2 GetOffset() { return m_offset; }
    void SetOffSet(Vector2 _offset) { m_offset = _offset; }

    void SetBarLen(float _barLen) { m_barLen = _barLen; }
    float GetBarLen() { return m_barLen; }

    void Render(HDC _dc);
    void Update(GameProcess* _gameProcess);

    Object_TextureBarUI(Texture* _emptyTexture, Texture* _fillTexture);
    ~Object_TextureBarUI() override;
};

