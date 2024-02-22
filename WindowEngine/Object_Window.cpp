#include "pch.h"
#include "Object_Window.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "Animation.h"
#include "Texture.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Object_Player.h"
#include "Scene.h"
#include "Scene_Stage01.h"

Object_Window::Object_Window(GameProcess* _gameprocess) : m_state(), m_lookTime(0.f),m_sumLookTime(0.f)
{
	ResourceManager* resourceManager = _gameprocess->GetResourceManager();
	PathManager* pathManager = _gameprocess->GetPathManager();
	SoundManager* soundManager = _gameprocess->GetSoundManager();

	HDC mainDC = _gameprocess->GetMainDC();

	CreateAnimator();

	Texture* idleTex = resourceManager->LoadTexture(L"IDLE_WINDOW", L"\\Texture\\window.bmp", pathManager, mainDC);
	Texture* walkTex = resourceManager->LoadTexture(L"WALK_WINDOW", L"\\Texture\\Animation\\walk_window.bmp", pathManager, mainDC);
	Texture* upTex = resourceManager->LoadTexture(L"UP_WINDOW", L"\\Texture\\Animation\\up_window.bmp", pathManager, mainDC);

	Animator* animator = GetAnimator();
	animator->CreateAnimation(L"IDLE", idleTex, Vector2(0.f, 0.f), Vector2(227.f, 130.f), Vector2(227.f, 0.f), 0.2f, 1);
	animator->CreateAnimation(L"WALK", walkTex, Vector2(0.f, 0.f), Vector2(227.f, 130.f), Vector2(227.f, 0.f), 0.2f, 14);
	animator->CreateAnimation(L"UP", upTex, Vector2(0.f, 0.f), Vector2(227.f, 130.f), Vector2(227.f, 0.f), 0.2f, 19);
}

Object_Window::~Object_Window()
{
}

void Object_Window::Render(HDC _dc)
{
	ComponentRender(_dc);
}

void Object_Window::Update(GameProcess* _gameProcess)
{
	Animation* animation = GetAnimator()->GetCurAnim();
	if (animation == nullptr)
	{
		m_lookTime = 0.f;
		m_sumLookTime = 0.f;
		GetAnimator()->Play(L"IDLE", true);
	}
	else if (animation->GetName() == L"UP")
	{
		Scene* curScene = _gameProcess->GetSceneManager()->GetCurScene();
		Scene_Stage01* stageScene = dynamic_cast<Scene_Stage01*>(curScene);
		Object_Player* player =stageScene->GetPlayer();
		PlayerState playerState = player->GetPlayerState();

		// 플레이어가 휴식상태이면 신뢰도 깍지
		if (playerState == PlayerState::REST)
		{
			m_sumLookTime += (float)_gameProcess->GetTimeManager()->GetDeltaTime();
			m_lookTime += (float)_gameProcess->GetTimeManager()->GetDeltaTime();
			/// 0.1초당 한번씩 신뢰도를 조금씩깍음
			if (m_lookTime >= 0.1f)
			{
				m_lookTime -= 0.1f;
				stageScene->AddTrust(-1.5f);
			}
			/// 2초이상 응시하면 게임오버
			if (m_sumLookTime >= 3.0f)
			{
				stageScene->AddTrust(-100.f);
			}
		}
	}
}

void Object_Window::PlayWalkAnimaion(SoundManager* soundManager)
{
	/// 걸어가는 사운드 추가하는곳
	soundManager->PlaySound(L"SE_Window", SOUND_TYPE::BOSS, 0.1f);
	soundManager->SetLoopCount(SOUND_TYPE::BOSS, 1);
	GetAnimator()->FindAnimation(L"WALK")->SetFrame(0);
	GetAnimator()->Play(L"WALK", 0);
}

void Object_Window::PlayUpAnimaion(SoundManager* soundManager)
{
	/// 올라오는 경고 사운드 추가하는 곳
	soundManager->PlaySound(L"SE_Window", SOUND_TYPE::BOSS, 0.1f);
	GetAnimator()->FindAnimation(L"UP")->SetFrame(0);
	GetAnimator()->Play(L"UP", 0);
}

