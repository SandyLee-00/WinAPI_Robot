#pragma once
#include "Object.h"

class Object_Order;

class Object_Dish :
    public Object
{

public:
	Object_Dish(GameProcess* gameProcess, Object_Order* _order);
	~Object_Dish();

private:
	Object_Order* m_orderSystem;
	EventManager* m_eventManager;

public:
	void CheckOk();

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

