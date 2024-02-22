#pragma once
#include "Scene.h"

class Scene_Option:
	public Scene
{
public:
	Scene_Option();
	virtual ~Scene_Option();


	void Exit() override;
	void Enter(GameProcess* _gameProcess) override;

};

