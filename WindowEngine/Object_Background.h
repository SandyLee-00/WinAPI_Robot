#pragma once
#include "Object.h"
class Object_Background :
    public Object
{
public:
    Object_Background(GameProcess* gameProcess);
    ~Object_Background();

    void Update(GameProcess* _gameProcess) override;
    void Render(HDC _dc) override;
};

