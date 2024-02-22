#pragma once
#include "Object.h"
class Object_CuttingBoard :
    public Object
{
public:
	Object_CuttingBoard(GameProcess* gameProcess);
	~Object_CuttingBoard();

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

