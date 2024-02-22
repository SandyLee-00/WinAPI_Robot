#pragma once
#include "Object.h"

class SceneManager;

class Object_UI :
    public Object
{

private:
    vector<Object_UI*> m_childUI; // 자식 UI
    Object_UI*         m_parentUI; // 부모 UI
    Vector2            m_finalUIPos;

    bool               m_camAffected; // UI 카메라에 영향을 받는 UI
    bool               m_mouseOn;     // UI 위에 마우스가 있는지
    bool               m_LbtnDown;    // UI 에 왼쪽버튼이 눌린적이 있는지 

public:
    Vector2  GetFinalPos() { return m_finalUIPos; }
    Object_UI* GetParentUI() { return m_parentUI; }
    bool       IsMouseOn() { return m_mouseOn; }
    bool       IsLbtnDown() { return m_LbtnDown; }

    void AddChildUI(Object_UI* _child);
    const vector<Object_UI*>& GetChildUI() { return m_childUI; }

    void Update(GameProcess* _gameProcess) override;
    void Finalupdate(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager) override;
    void Render(HDC _dc) override;

private:
    void UpdateChildUI(GameProcess* _gameProcess);
    void FinalUpdateChildUI(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager);
    void RenderChildUI(HDC _dc);

    void MouseOnCheck(KeyManager* _keyManager);
public:
    virtual void MouseOn(GameProcess* _gameProcess);
    virtual void MouseLbtnDown(GameProcess* _gameProcess);
    virtual void MouseLbtnUp(GameProcess* _gameProcess);
    virtual void MouseLbtnClicked(GameProcess* _gameProcess);

    
    /// 생성자 소멸자
    Object_UI();
    Object_UI(bool _camAffected);
    virtual ~Object_UI() ;

    friend class UIManager;
};

