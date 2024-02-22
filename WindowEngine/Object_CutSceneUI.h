#pragma once
#include "Object_UI.h"

class Texture;

class Object_CutSceneUI :
    public Object_UI
{
private:
    // ���콺 ��Ŭ�� ������ǥ
    Vector2 m_startDrag;
	
    // �ƾ� �ε���
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

