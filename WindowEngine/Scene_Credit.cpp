#include "pch.h"
#include "Scene_Credit.h"
#include "Object_UI.h"
#include "Object_BtnUI.h"
#include "Object_Credit.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "Object_Face.h"
#include "SoundManager.h"

Scene_Credit::Scene_Credit()
{

}

Scene_Credit::~Scene_Credit()
{

}

void Scene_Credit::Exit()
{
	DeleteAll();
}

void Scene_Credit::Enter(GameProcess* _gameProcess)
{
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	CameraManager* cameraManager = _gameProcess->GetCameraManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	HDC mainDC = _gameProcess->GetMainDC();

	/// 크래딧 이미지 띄우기
	Object_Credit* credit = new Object_Credit(_gameProcess);
	AddObject(credit, OBJECT_TYPE::BACKGROUND);

	/// 메인화면 돌아가는 버튼
	Object_BtnUI* returnBtn = new Object_BtnUI(_gameProcess,true);
	returnBtn->SetPos(Vector2(1740.f, 50.f));
	returnBtn->SetScale(Vector2(80.f, 90.f));
	returnBtn->SetBtnFunction(SCENE_TYPE::MAIN);
	Texture* returnTexture = resourceManager->LoadTexture(L"homebutton", L"\\Texture\\UI\\popup_button3.bmp", pathManager, mainDC);
	returnBtn->SetTexture(returnTexture);
	AddObject(returnBtn, OBJECT_TYPE::UI);

	/// Programmer
	Object_Face* seoyeong = new Object_Face(_gameProcess, NAME::SEOYEONG);
	Object_Face* junyeol = new Object_Face(_gameProcess, NAME::JUNYEOL);
	Object_Face* yeonha = new Object_Face(_gameProcess, NAME::YEONHA);
	Object_Face* gitae = new Object_Face(_gameProcess, NAME::GITAE);

	// 원 모양으로 배치하기
	seoyeong->SetPos(Vector2(100.f, 100.f));
	junyeol->SetPos(Vector2(100.f, 300.f));
	yeonha->SetPos(Vector2(100.f, 500.f));
	gitae->SetPos(Vector2(100.f, 700.f));

	seoyeong->SetScale(Vector2(100.f, 100.f));
	junyeol->SetScale(Vector2(100.f, 100.f));
	yeonha->SetScale(Vector2(100.f, 100.f));
	gitae->SetScale(Vector2(100.f, 100.f));

	AddObject(seoyeong, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(junyeol, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(yeonha, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(gitae, OBJECT_TYPE::INTERACTABLE_OBJECT);

	/// Designer
	Object_Face* gibin = new Object_Face(_gameProcess, NAME::GIBIN);
	Object_Face* sanghyeok = new Object_Face(_gameProcess, NAME::SANGHYEOK);

	gibin->SetPos(Vector2(300.f, 100.f));
	sanghyeok->SetPos(Vector2(300.f, 300.f));

	gibin->SetScale(Vector2(100.f, 100.f));
	sanghyeok->SetScale(Vector2(100.f, 100.f));

	AddObject(gibin, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(sanghyeok, OBJECT_TYPE::INTERACTABLE_OBJECT);

	/// Artist
	Object_Face* jyhyun = new Object_Face(_gameProcess, NAME::JUHYUN);
	Object_Face* eunji = new Object_Face(_gameProcess, NAME::EUNJI);
	Object_Face* minji = new Object_Face(_gameProcess, NAME::MINJI);

	jyhyun->SetPos(Vector2(500.f, 100.f));
	eunji->SetPos(Vector2(500.f, 300.f));
	minji->SetPos(Vector2(500.f, 500.f));

	jyhyun->SetScale(Vector2(100.f, 100.f));
	eunji->SetScale(Vector2(100.f, 100.f));
	minji->SetScale(Vector2(100.f, 100.f));

	AddObject(jyhyun, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(eunji, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(minji, OBJECT_TYPE::INTERACTABLE_OBJECT);

	/// 카메라 설정
	cameraManager->AllBlack(1.f);
	cameraManager->FadeIn(2.f);
	//cameraManager->ShineLight(155.f);

	soundManager->PlaySound(L"BGM_Credit", SOUND_TYPE::BGM);
}
