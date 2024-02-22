#pragma once
#include "Object.h"
class Object_Table :
    public Object
{

public:
    Object_Table(GameProcess* gameProcess, Texture* textureTable);
	~Object_Table();

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

