#pragma once
#include "Object_UI.h"

class Texture;

class Object_CutSceneUI :
    public Object_UI
{
private:
    // ¸¶¿ì½º ÁÂÅ¬¸¯ ½ÃÀÛÁÂÇ¥
    Vector2 m_startDrag;
	
    // ÄÆ¾À ÀÎµ¦½º
    int m_curSceneIndex;
    bool addBtn;

	float  m_dragDistance;
	Texture* m_sceneTex[16];

public:
    Object_CutSceneUI(GameProcess* _gameProcess);
    ~Object_CutSceneUI();

    void MouseLbtnDown(GameProcess* _gameProcess) override;
    void MouseOn(GameProcess* _gameProcess) override;
    void MouseLbtnUp(GameProcess* _gameProcess) override;

    void Update(GameProcess* _gameProcess);
    void Render(HDC _dc);
};

