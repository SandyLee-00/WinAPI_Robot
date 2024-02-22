#include "pch.h"
#include "Scene_Option.h"
#include "Object_UI.h"
#include "Object_BtnUI.h"
#include "Object_Credit.h"

Scene_Option::Scene_Option()
{

}

Scene_Option::~Scene_Option()
{

}

void Scene_Option::Exit()
{
	DeleteAll();
}

void Scene_Option::Enter(GameProcess* _gameProcess)
{
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	CameraManager* cameraManager = _gameProcess->GetCameraManager();
	HDC mainDC = _gameProcess->GetMainDC();


	Object_Credit* credit = new Object_Credit(_gameProcess);
	AddObject(credit, OBJECT_TYPE::BACKGROUND);

}
