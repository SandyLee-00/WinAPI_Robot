#include "pch.h"
#include "Scene_Main.h"
#include "Object_Player.h"
#include "Object_BtnUI.h"
#include "Object_UI.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "Object_TextUI.h"
#include "Object_BarUI.h"
#include "Texture.h"
#include "CameraManager.h"
#include "Object_Title.h"
#include "SoundManager.h"

Scene_Main::Scene_Main()
{
}

Scene_Main::~Scene_Main()
{
	DeleteAll();
}

void Scene_Main::Exit()
{
	DeleteAll();

}

void Scene_Main::Enter(GameProcess* _gameProcess)
{
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	CameraManager* cameraManager = _gameProcess->GetCameraManager();
	HDC mainDC = _gameProcess->GetMainDC();

	Object_Title* title = new Object_Title(_gameProcess);
	AddObject(title,OBJECT_TYPE::BACKGROUND);

	Object_BtnUI* enterBtnUI = new Object_BtnUI(_gameProcess,true);
	enterBtnUI->SetPos(Vector2(1500.f, 600.f));
	enterBtnUI->SetScale(Vector2(382.f, 138.f));
	Texture* enterTex = resourceManager->LoadTexture(L"enterBtnUI", L"\\Texture\\UI\\start_button.bmp", pathManager, mainDC);
	enterBtnUI->SetTexture(enterTex);
	enterBtnUI->SetBtnFunction(SCENE_TYPE::CARTOON);
	AddObject(enterBtnUI, OBJECT_TYPE::UI);

	Object_BtnUI* creditBtnUI = new Object_BtnUI(_gameProcess,true);
	creditBtnUI->SetPos(Vector2(1500.f, 750.f));
	creditBtnUI->SetScale(Vector2(370.f, 130.f));
	Texture* creditTex = resourceManager->LoadTexture(L"creditBtnUI", L"\\Texture\\UI\\credit_button.bmp", pathManager, mainDC);
	creditBtnUI->SetTexture(creditTex);
	creditBtnUI->SetBtnFunction(SCENE_TYPE::OPTION);
	AddObject(creditBtnUI, OBJECT_TYPE::UI);

	Object_BtnUI* exitBtnUI = new Object_BtnUI(_gameProcess,true);
	exitBtnUI->SetPos(Vector2(1500.f, 900.f));
	exitBtnUI->SetScale(Vector2(370.f, 130.f));
	Texture* exitTex = resourceManager->LoadTexture(L"exitBtnUI", L"\\Texture\\UI\\end_button.bmp", pathManager, mainDC);
	exitBtnUI->SetTexture(exitTex);
	exitBtnUI->SetBtnFunction(SCENE_TYPE::EXITGAME);
	AddObject(exitBtnUI, OBJECT_TYPE::UI);

	cameraManager->AllBlack(0.5f);
	cameraManager->FadeIn(1.f);
	cameraManager->ShineLight(50.f);

	// 타이틀 브금 삽입
	soundManager->PlaySound(L"BGM_Title", SOUND_TYPE::BGM);
}
