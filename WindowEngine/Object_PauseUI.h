#pragma once
#include "Object_UI.h"

class Texture;

class Object_PauseUI :
    public Object_UI
{

private:
    Texture* m_veilTex; // 가림막 텍스처
    SceneManager* m_sceneManager;

public:
    Object_PauseUI(GameProcess* gameProcess);
   virtual ~Object_PauseUI();

    void Render(HDC _dc) override;

};

