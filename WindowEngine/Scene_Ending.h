#pragma once
#include "Scene.h"

class Texture;

class Scene_Ending :
    public Scene
{
public:
    Scene_Ending();
    ~Scene_Ending();

    void Exit() override;
    void Enter(GameProcess* _gameProcess) override;
};

