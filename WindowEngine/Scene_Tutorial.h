#pragma once
#include "Scene.h"

class Scene_Tutorial :
    public Scene
{

public:
    void Exit() override; // 해당 Scene에 진입시 호출
    void Enter(GameProcess* _gameProcess) override; // 해당 Scene을 탈출 시 호출

    Scene_Tutorial();
    ~Scene_Tutorial();
};

