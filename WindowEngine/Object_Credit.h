#pragma once
#include "Object.h"
class Object_Credit :
    public Object
{
public:
    Object_Credit(GameProcess* gameProcess);
    ~Object_Credit();

    void Render(HDC _dc) override;
    void Update(GameProcess* _gameProcess) override;
};

