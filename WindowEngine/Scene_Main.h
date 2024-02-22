#pragma once
#include "Scene.h"
class Scene_Main :
    public Scene
{


public:
    Scene_Main();
   virtual ~Scene_Main();

    //  virtual 함수는 override 키워드를 사용해서 컴파일러가 확인시키자!


    void Exit() override;
    void Enter(GameProcess* _gameProcess) override;

};

