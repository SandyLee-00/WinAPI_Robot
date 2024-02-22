#pragma once
#include "Object_UI.h"

#include "Scene.h"

enum  class BTN_FUNC
{
    CHANGE_SCENE,
    PAUSE_UI,
    DELTE_PARENT_UI,
};

class Object_BtnUI :
    public Object_UI
{
private:
    bool m_hasAnimation;

    BTN_FUNC   m_function;
    DWORD_PTR  m_lParam;
    DWORD_PTR  m_wParam;

public:
    void SetBtnFunction(SCENE_TYPE _next);
    void SetBtnFunction(BTN_FUNC _func);    

    void MouseOn(GameProcess* _gameProcess) override;
    void MouseLbtnDown(GameProcess* _gameProcess) override;
    void MouseLbtnUp(GameProcess* _gameProcess) override;
    void MouseLbtnClicked(GameProcess* _gameProcess) override;
    
    void Render(HDC _dc) override;

    Object_BtnUI(GameProcess* gameProcess, bool _hasAnimation);
    Object_BtnUI(GameProcess* gameProcess);
    virtual~Object_BtnUI();

};

