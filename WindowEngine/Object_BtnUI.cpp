#include "pch.h"
#include "Object_BtnUI.h"
#include "EventManager.h"
#include "BoxCollider.h"
#include "Object_PauseUI.h"
#include "GameProcess.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Texture.h"

Object_BtnUI::Object_BtnUI(GameProcess* gameProcess)
	:Object_UI(false)
	,m_lParam(0)
	,m_wParam(0)
	,m_function()
	,m_hasAnimation(false)
{

}

Object_BtnUI::Object_BtnUI(GameProcess* gameProcess, bool _hasAnimation)
	:Object_UI(false)
	, m_lParam(0)
	, m_wParam(0)
	, m_function()
	, m_hasAnimation(_hasAnimation)
{}

Object_BtnUI::~Object_BtnUI()
{}


void Object_BtnUI::SetBtnFunction(SCENE_TYPE _next)
{
	m_function = BTN_FUNC::CHANGE_SCENE;
	m_lParam = (DWORD_PTR)_next;
}

void Object_BtnUI::SetBtnFunction(BTN_FUNC _func)
{
	m_function = _func;
}

void Object_BtnUI::MouseOn(GameProcess* _gameProcess)
{
}

void Object_BtnUI::MouseLbtnDown(GameProcess* _gameProcess)
{
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	soundManager->PlaySound(L"SE_UI_Touch", SOUND_TYPE::UI);
}

void Object_BtnUI::MouseLbtnUp(GameProcess* _gameProcess)
{

}

void Object_BtnUI::MouseLbtnClicked(GameProcess* _gameProcess)
{
	SceneManager* sceneManager = _gameProcess->GetSceneManager();
	EventManager* eventManager = _gameProcess->GetEvenetManager();

	switch (m_function)
	{
	case BTN_FUNC::CHANGE_SCENE:
	{
		eventManager->ChangeScene((SCENE_TYPE)m_lParam);
	}
	break;
	case BTN_FUNC::PAUSE_UI:
	{
		bool pauseGame = sceneManager->GetCurScene()->IsPauseGame();
		if (!pauseGame)
		{
			Object_PauseUI* pauseUI = new Object_PauseUI(_gameProcess);
			eventManager->CreateObject(pauseUI, OBJECT_TYPE::UI);
		}
		break;
	}
	case BTN_FUNC::DELTE_PARENT_UI:
	{
		// 부모 UI 삭제 버튼
		_gameProcess->GetSceneManager()->GetCurScene()->PlayGame();
		eventManager->DeleteObject(GetParentUI());
	}
	default:
		break;
	}
}

void Object_BtnUI::Render(HDC _dc)
{
	Vector2 pos = GetFinalPos();
	Vector2 scale = GetScale();
	if (!m_hasAnimation)
	{
		Object_UI::Render(_dc);
	}
	else if (!IsLbtnDown())
	{

		int width = (int)m_tex->GetWidth();
		int height = (int)m_tex->GetHeight();
		TransparentBlt(_dc
			, (int)(pos.x)
			, (int)(pos.y)
			, width/2, height
			, m_tex->GetDC()
			, 0, 0, width/2, height
			, RGB(255, 0, 255));
	}
	else
	{
		int width = (int)m_tex->GetWidth();
		int height = (int)m_tex->GetHeight();
		TransparentBlt(_dc
			, (int)(pos.x)
			, (int)(pos.y)
			, width / 2, height
			, m_tex->GetDC()
			, width/2, 0, width / 2, height
			, RGB(255, 0, 255));
	}
}
