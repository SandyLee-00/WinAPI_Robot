#pragma once
#include "Scene.h"

class Scene_Tutorial :
    public Scene
{

public:
    void Exit() override; // �ش� Scene�� ���Խ� ȣ��
    void Enter(GameProcess* _gameProcess) override; // �ش� Scene�� Ż�� �� ȣ��

    Scene_Tutorial();
    ~Scene_Tutorial();
};

