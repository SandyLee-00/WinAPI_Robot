#include "pch.h"
#include "Scene_Cartoon.h"
#include "Object_CutSceneUI.h"
#include "CameraManager.h"
#include "SoundManager.h"

Scene_Cartoon::Scene_Cartoon()
{

}

Scene_Cartoon::~Scene_Cartoon()
{

}

void Scene_Cartoon::Enter(GameProcess* _gameProcess)
{
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	Object_CutSceneUI* cutSceneUI = new Object_CutSceneUI(_gameProcess);
	cutSceneUI->SetPos(Vector2(0.f, 0.f));
	AddObject(cutSceneUI,OBJECT_TYPE::UI);

	_gameProcess->GetCameraManager()->FadeIn(0.5f);

	soundManager->PlaySound(L"BGM_start_comic", SOUND_TYPE::BGM);
}

void Scene_Cartoon::Exit()
{
	DeleteAll();
}
