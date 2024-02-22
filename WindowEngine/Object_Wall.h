#pragma once
#include "Object.h"
class Object_Wall :
    public Object
{

public:
	Object_Wall(GameProcess* gameProcess);
	~Object_Wall();

	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;

};

