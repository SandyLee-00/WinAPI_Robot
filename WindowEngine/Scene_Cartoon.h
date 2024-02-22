#pragma once

#include "Scene.h"

class Scene_Cartoon :
    public Scene
{

public:
    Scene_Cartoon();
    ~Scene_Cartoon();

    void Enter(GameProcess* _gameProcess);
    void Exit();
};

