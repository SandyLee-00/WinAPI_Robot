#pragma once
#include "Object_UI.h"

class Object_TextUI;
struct Order;
class Animator;

class Object_OrderSheetUI :
    public Object_UI
{
private:
    float           m_sheetInterval;       // 주문서 글자 간격
    Animator*       m_animator;
    vector<Order*>*  m_orderList;

public:
    Object_OrderSheetUI(GameProcess* gameProcess);
    ~Object_OrderSheetUI() override ;
    void SetOrderList(vector<Order*>* _orderList) { m_orderList = _orderList; }

public:
    bool AddOrder();     // 피자주문을 주문서에 추가
    bool ClearOrder();  // 피자 서빙을 완료함 

public:
	void AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC);
    void AnimationSetting(wstring _animName, int index);
    Animator* GetAnimator() { return m_animator; }

public:
    void Render(HDC _dc) override;
    void Update(GameProcess* _gameProcess) override;
};

