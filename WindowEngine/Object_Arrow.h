#pragma once
#include "Object.h"
class Object_Arrow :
    public Object
{
private:
    float m_addtime;

public:
    Object_Arrow(GameProcess* _gameProcess);
    ~Object_Arrow();

    void Update(GameProcess* _gameProcess);
    void Render(HDC _dc);

};

