#include "pch.h"
#include "Object_PauseUI.h"
#include "Object_BtnUI.h"

#include "GameProcess.h"
#include "Texture.h"
#include "SelectGDI.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"

Object_PauseUI::Object_PauseUI(GameProcess* gameProcess)
	:m_veilTex(nullptr)
	, m_sceneManager(nullptr)
{
	SetName(L"PauseUI");

	SceneManager* sceneManager = gameProcess->GetSceneManager();
	ResourceManager* resourceManager = gameProcess->GetResourceManager();
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();

	/// 멤버변수로 씬매니저를 가져야함
	m_sceneManager = sceneManager;

	// 현재씬 일시중지
	sceneManager->GetCurScene()->PauseGame();

	POINT resolution = gameProcess->GetResolution();

	m_veilTex =resourceManager->CreateTexture(L"veilTex", resolution.x, resolution.y, mainDC);

	SelectGDI gdi(m_veilTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_veilTex->GetDC(), 0, 0, resolution.x, resolution.y);

	SetPos(Vector2(500.f, 300.f));
	SetScale(Vector2(800.f, 500.f));

	m_tex = resourceManager->LoadTexture(L"popUpUI", L"\\Texture\\UI\\popup.bmp", pathManager, mainDC);

	/// 다시재생 UI
	Object_BtnUI* playGameBtn = new Object_BtnUI(gameProcess,true);
	playGameBtn->SetPos(Vector2(100.f, 350.f));
	playGameBtn->SetScale(Vector2(125.f, 125.f));
	playGameBtn->SetBtnFunction(BTN_FUNC::DELTE_PARENT_UI);
	Texture* playGameTex = resourceManager->LoadTexture(L"playButtonUI", L"\\Texture\\UI\\popup_button2.bmp", pathManager, mainDC);
	playGameBtn->SetTexture(playGameTex);

	AddChildUI(playGameBtn);

	// 스테이지 재시작 UI
	Object_BtnUI* restartBtn = new Object_BtnUI(gameProcess,true);
	restartBtn->SetPos(Vector2(350.f, 350.f));
	restartBtn->SetScale(Vector2(125.f, 125.f));
	restartBtn->SetBtnFunction(SCENE_TYPE::STAGE01);
	Texture* restartTex = resourceManager->LoadTexture(L"restartButtonUI", L"\\Texture\\UI\\popup_button1.bmp", pathManager, mainDC);
	restartBtn->SetTexture(restartTex);
	AddChildUI(restartBtn);

	// 메인화면으로 돌아가는 UI
	Object_BtnUI* exitBtn = new Object_BtnUI(gameProcess,true);
	exitBtn->SetPos(Vector2(600.f, 350.f));
	exitBtn->SetScale(Vector2(125.f, 125.f));
	exitBtn->SetBtnFunction(SCENE_TYPE::MAIN);
	Texture* homeTex = resourceManager->LoadTexture(L"homeButtonUI", L"\\Texture\\UI\\popup_button3.bmp", pathManager, mainDC);
	exitBtn->SetTexture(homeTex);
	AddChildUI(exitBtn);

}

Object_PauseUI::~Object_PauseUI()
{
	m_sceneManager->GetCurScene()->PlayGame();
}

void Object_PauseUI::Render(HDC _dc)
{
	Vector2 pos = GetFinalPos();
	Vector2 scale = GetScale();

	int width = m_veilTex->GetWidth();
	int height = m_veilTex->GetHeight();

	BLENDFUNCTION blendFunc{};
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = 200;  // 알파값(0~255), 0이면 완전 투명, 255이면 불투명
	blendFunc.AlphaFormat = 0; // 여기 값을 0이다!!!!!!!1

	AlphaBlend(_dc, 0, 0,
		width, height, m_veilTex->GetDC()
		, 0, 0, width, height, blendFunc);

	Object_UI::Render(_dc);
}

