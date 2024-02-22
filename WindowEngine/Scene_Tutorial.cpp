#include "pch.h"
#include "Scene_Tutorial.h"
#include "Object_Player.h"


Scene_Tutorial::Scene_Tutorial()
{
}

Scene_Tutorial::~Scene_Tutorial()
{
}

void Scene_Tutorial::Exit()
{

}

void Scene_Tutorial::Enter(GameProcess* _gameProcess)
{
	Object_Player* player = new Object_Player(_gameProcess);
	player->SetPos(Vector2(910.f, 540.f));

	AddObject(player, OBJECT_TYPE::PLAYER);
}