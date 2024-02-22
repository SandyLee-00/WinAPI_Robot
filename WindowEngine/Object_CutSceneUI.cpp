#include "pch.h"
#include "Object_CutSceneUI.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "KeyManager.h"
#include "Texture.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Object_BtnUI.h"
#include "ResourceManager.h"

Object_CutSceneUI::Object_CutSceneUI(GameProcess* _gameProcess)
	:m_dragDistance(0.f), m_sceneTex{}, m_curSceneIndex(0), m_startDrag{0.f,0.f}, addBtn(false)
{
	SetScale(Vector2(1920.f, 1080.f));
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();

	m_tex = resourceManager->LoadTexture(L"nullscene1", L"\\Texture\\scene\\null.bmp", pathManager, mainDC);

	// 씬불러오기
	m_sceneTex[0] = resourceManager->LoadTexture(L"cartoonscen1", L"\\Texture\\scene\\1.bmp", pathManager, mainDC);
	m_sceneTex[1] = resourceManager->LoadTexture(L"cartoonscen2", L"\\Texture\\scene\\2.bmp", pathManager, mainDC);
	m_sceneTex[2] = resourceManager->LoadTexture(L"cartoonscen3", L"\\Texture\\scene\\3.bmp", pathManager, mainDC);
	m_sceneTex[3] = resourceManager->LoadTexture(L"cartoonscen4", L"\\Texture\\scene\\4.bmp", pathManager, mainDC);
	m_sceneTex[4] = resourceManager->LoadTexture(L"cartoonscen5", L"\\Texture\\scene\\5.bmp", pathManager, mainDC);
	m_sceneTex[5] = resourceManager->LoadTexture(L"tutorial1", L"\\Texture\\tutorial\\1.bmp", pathManager, mainDC);
	m_sceneTex[6] = resourceManager->LoadTexture(L"tutorial2", L"\\Texture\\tutorial\\2.bmp", pathManager, mainDC);
	m_sceneTex[7] = resourceManager->LoadTexture(L"tutorial3", L"\\Texture\\tutorial\\3.bmp", pathManager, mainDC);
	m_sceneTex[8] = resourceManager->LoadTexture(L"tutorial4", L"\\Texture\\tutorial\\4.bmp", pathManager, mainDC);
	m_sceneTex[9] = resourceManager->LoadTexture(L"tutorial5", L"\\Texture\\tutorial\\5.bmp", pathManager, mainDC);
	m_sceneTex[10] = resourceManager->LoadTexture(L"tutorial6", L"\\Texture\\tutorial\\6.bmp", pathManager, mainDC);
	m_sceneTex[11] = resourceManager->LoadTexture(L"tutorial7", L"\\Texture\\tutorial\\7.bmp", pathManager, mainDC);
	m_sceneTex[12] = resourceManager->LoadTexture(L"tutorial8", L"\\Texture\\tutorial\\8.bmp", pathManager, mainDC);
	m_sceneTex[13] = resourceManager->LoadTexture(L"tutorial9", L"\\Texture\\tutorial\\9.bmp", pathManager, mainDC);
	m_sceneTex[14] = resourceManager->LoadTexture(L"tutorial10", L"\\Texture\\tutorial\\10.bmp", pathManager, mainDC);
	m_sceneTex[15] = resourceManager->LoadTexture(L"tutorial11", L"\\Texture\\tutorial\\11.bmp", pathManager, mainDC);
}

Object_CutSceneUI::~Object_CutSceneUI()
{

}

void Object_CutSceneUI::MouseLbtnDown(GameProcess* _gameProcess)
{
	KeyManager* keyManager = _gameProcess->GetKeyManager();
	m_startDrag = keyManager->GetMousePos();
}

void Object_CutSceneUI::MouseOn(GameProcess* _gameProcess)
{
	if (IsLbtnDown())
	{
		KeyManager* keyManager = _gameProcess->GetKeyManager();

		// 왼쪽클릭이 일어난 시점과 지금 마우스 좌표의 차이
		m_dragDistance = m_startDrag.x - keyManager->GetMousePos().x;
	}
}

void Object_CutSceneUI::MouseLbtnUp(GameProcess* _gameProcess)
{
	EventManager* eventManager = _gameProcess->GetEvenetManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	// 일정거리이상 드래그하면 다음 컷씬으로 변경
	// 오른쪽으로 씬을 넘김
	if (m_dragDistance >= 100.f)
	{
		// 페이지 넘어가는 소리
		soundManager->PlaySound(L"SE_pages", SOUND_TYPE::PAGE);

		if (m_curSceneIndex == 15)
			eventManager->ChangeScene(SCENE_TYPE::STAGE01);
		else
			m_curSceneIndex++;
	}
	// 왼쪽으로 씬을 넘김
	else if (m_dragDistance <= -100.f)
	{
		// 페이지 넘어가는 소리
		soundManager->PlaySound(L"SE_pages", SOUND_TYPE::PAGE);

		if (m_curSceneIndex == 0)
			eventManager->ChangeScene(SCENE_TYPE::MAIN);
		else
			m_curSceneIndex--;
	}

	m_dragDistance = 0.f;
}

void Object_CutSceneUI::Update(GameProcess* _gameProcess)
{
	// 키입력으로도 씬을 넘기기 가능
	KeyManager* keymanager = _gameProcess->GetKeyManager();
	EventManager* eventManager = _gameProcess->GetEvenetManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();

	if (!addBtn && m_curSceneIndex == 5)
	{
		addBtn = true;
		Object_BtnUI* skipBtnUI = new Object_BtnUI(_gameProcess, true);
		skipBtnUI->SetPos(Vector2(1700.f, 50.f));
		skipBtnUI->SetScale(Vector2(250.f, 125.f));
		Texture* skipTex = resourceManager->LoadTexture(L"skipBtnUI", L"\\Texture\\UI\\skip_button.bmp", pathManager, mainDC);
		skipBtnUI->SetTexture(skipTex);
		skipBtnUI->SetBtnFunction(SCENE_TYPE::STAGE01);
		eventManager->CreateObject(skipBtnUI, OBJECT_TYPE::UI);
	}

	if (keymanager->IsKeyState(KEY::RIGHT, KEY_STATE::TAP))
	{
		// 페이지 넘어가는 소리
		soundManager->PlaySound(L"SE_pages", SOUND_TYPE::PAGE);

		if (m_curSceneIndex == 15)
			eventManager->ChangeScene(SCENE_TYPE::STAGE01);
		else
			m_curSceneIndex++;
	}

	if (keymanager->IsKeyState(KEY::LEFT, KEY_STATE::TAP))
	{
		// 페이지 넘어가는 소리
		soundManager->PlaySound(L"SE_pages", SOUND_TYPE::PAGE);

		if (m_curSceneIndex == 0)
			eventManager->ChangeScene(SCENE_TYPE::MAIN);
		else
			m_curSceneIndex--;
	}
}

void Object_CutSceneUI::Render(HDC _dc)
{
	float dragLen = m_dragDistance * 1.5f;

	// 왼쪽방향으로 드래그
	if (m_dragDistance >= 0.f )
	{
		Texture* leftTex = m_sceneTex[m_curSceneIndex];
		int leftWidth = leftTex->GetWidth();
		int leftHeight = leftTex->GetHeight();

		BitBlt(_dc, 0, 0, leftWidth, leftHeight, leftTex->GetDC()
			, (int)dragLen, 0, SRCCOPY);

		if (m_curSceneIndex != 15)
		{
			Texture* rightTex = m_sceneTex[m_curSceneIndex + 1];
			int rightWidth = rightTex->GetWidth();
			int rightHeight = rightTex->GetHeight();

			BitBlt(_dc, 1920 - (int)dragLen, 0, rightWidth, rightHeight, rightTex->GetDC(), 0, 0, SRCCOPY);
		}
		else
		{
			int rightWidth = m_tex->GetWidth();
			int rightHeight = m_tex->GetHeight();
			BitBlt(_dc, 1920 - (int)dragLen, 0, rightWidth, rightHeight, m_tex->GetDC(), 0, 0, SRCCOPY);
		}

	}
	else // 오른쪽방향으로 드래고
	{
		if (m_curSceneIndex != 0)
		{
			Texture* leftTex = m_sceneTex[m_curSceneIndex - 1];
			int leftWidth = leftTex->GetWidth();
			int leftHeight = leftTex->GetHeight();

			BitBlt(_dc, -1920 - (int)dragLen, 0, leftWidth, leftHeight, leftTex->GetDC()
				, 0, 0, SRCCOPY);
		}
		else
		{
			int leftWidth = m_tex->GetWidth();
			int leftHeight = m_tex->GetHeight();

			BitBlt(_dc, -1920 - (int)dragLen, 0, leftWidth, leftHeight, m_tex->GetDC()
				, 0, 0, SRCCOPY);
		}

		Texture* rightTex = m_sceneTex[m_curSceneIndex];
		int rightWidth = rightTex->GetWidth();
		int rightHeight = rightTex->GetHeight();

		BitBlt(_dc, (int)-dragLen, 0, rightWidth, rightHeight, rightTex->GetDC(), 0, 0, SRCCOPY);
	}
}
