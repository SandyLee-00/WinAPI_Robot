#include "pch.h"
#include "Scene_Ending.h"
#include "Object_EndingUI.h"
#include "CameraManager.h"
#include "SoundManager.h"

Scene_Ending::Scene_Ending()
{
}

Scene_Ending::~Scene_Ending()
{

}

void Scene_Ending::Exit()
{

}

void Scene_Ending::Enter(GameProcess* _gameProcess)
{
	Object_EndingUI* endingSceneUI = new Object_EndingUI(_gameProcess);
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	endingSceneUI->SetPos(Vector2(0.f, 0.f));
	AddObject(endingSceneUI, OBJECT_TYPE::UI);

	_gameProcess->GetCameraManager()->FadeIn(0.5f);


	soundManager->PlaySound(L"BGM_Ending", SOUND_TYPE::BGM);
}
