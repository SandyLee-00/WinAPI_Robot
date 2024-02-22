#pragma once
#include "Object.h"
class Object_receipt :
    public Object
{
public:
    Object_receipt(GameProcess* gameProcess);
    ~Object_receipt();

    void Update(GameProcess* _gameProcess) override;
    void Render(HDC _dc) override;
    void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
    void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;


};

