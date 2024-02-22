#pragma once
#include "Scene.h"

/// <summary>
///  Creditæ¿¿∏∑Œ ∫Ø∞Ê
/// </summary>
class Scene_Credit:
	public Scene
{
public:
	Scene_Credit();
	virtual ~Scene_Credit();

	void Exit() override;
	void Enter(GameProcess* _gameProcess) override;

};

