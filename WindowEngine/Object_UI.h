#pragma once
#include "Object.h"

class SceneManager;

class Object_UI :
    public Object
{

private:
    vector<Object_UI*> m_childUI; // �ڽ� UI
    Object_UI*         m_parentUI; // �θ� UI
    Vector2            m_finalUIPos;

    bool               m_camAffected; // UI ī�޶� ������ �޴� UI
    bool               m_mouseOn;     // UI ���� ���콺�� �ִ���
    bool               m_LbtnDown;    // UI �� ���ʹ�ư�� �������� �ִ��� 

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

    
    /// ������ �Ҹ���
    Object_UI();
    Object_UI(bool _camAffected);
    virtual ~Object_UI() ;

    friend class UIManager;
};

