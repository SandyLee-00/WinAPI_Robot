#pragma once
#include "Scene.h"
class Scene_Main :
    public Scene
{


public:
    Scene_Main();
   virtual ~Scene_Main();

    //  virtual �Լ��� override Ű���带 ����ؼ� �����Ϸ��� Ȯ�ν�Ű��!


    void Exit() override;
    void Enter(GameProcess* _gameProcess) override;

};

