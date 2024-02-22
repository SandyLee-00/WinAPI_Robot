#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Scene_Main.h"
#include "Scene_Credit.h"
#include "Scene_Stage01.h"
#include "Scene_Tutorial.h"
#include "Scene_Cartoon.h"
#include "Scene_Ending.h"

#include "KeyManager.h"
#include "CameraManager.h"
#include "BossManager.h"

#include "SoundManager.h"

SceneManager::SceneManager()
	:m_curScene(nullptr)
	,m_scene{}
{}

SceneManager::~SceneManager()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_scene[i])
			delete m_scene[i];
	}
}

void SceneManager::Initialize(GameProcess* _gmaeProcess)
{
	// Scene ����
	m_scene[(UINT)SCENE_TYPE::MAIN] = new Scene_Main;
	m_scene[(UINT)SCENE_TYPE::OPTION] = new Scene_Credit;
	m_scene[(UINT)SCENE_TYPE::TUTORIAL] = new Scene_Tutorial;
	m_scene[(UINT)SCENE_TYPE::STAGE01] = new Scene_Stage01;
	m_scene[(UINT)SCENE_TYPE::CARTOON] = new Scene_Cartoon;
	m_scene[(UINT)SCENE_TYPE::ENDING] = new Scene_Ending;
	// Start Scene -> ��ŸƮ�� ����
	m_curScene = m_scene[(UINT)SCENE_TYPE::MAIN];
	m_curScene->Enter(_gmaeProcess);
}

void SceneManager::Update(GameProcess* _gameProcess)
{
	m_curScene->Update(_gameProcess);
}

void SceneManager::FinalUpdate(TimeManager* timeManager, EventManager* eventManager, KeyManager* _keyManager)
{
	m_curScene->FinalUpdate(timeManager, eventManager, _keyManager);
}

void SceneManager::Render(HDC _dc)
{
	m_curScene->Render(_dc);
}

void SceneManager::ChangeScene(SCENE_TYPE _next, GameProcess* _gameProcess)
{
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	KeyManager* keyManager = _gameProcess->GetKeyManager();
	BossManager* bossManager = _gameProcess->GetBossManager();

	keyManager->ActiveAllKey();
	bossManager->DisappearBoss();

	// ���� �ٲ�� ��� ä���� �� ���ش�
	soundManager->Player_Pause();
	soundManager->Boss_Pause();
	soundManager->Order_Pause();
	soundManager->Oven1_Pause();
	soundManager->Oven2_Pause();
	soundManager->Oven3_Pause();
	soundManager->BGM_Pause();
	soundManager->UI_Pause();
	soundManager->Page_Pause();
	//soundManager->GameOver_Pause();

	// ���� ������ ���� ���¶�� �ٽ� �÷��� ���·� ��ȯ
	m_curScene->PlayGame();

	// ī�޶� ȿ������Ʈ Ŭ����
	_gameProcess->GetCameraManager()->ClearCameraEffectList();

	// ��������
	if (_next == SCENE_TYPE::EXITGAME)
	{
		PostQuitMessage(0);
		return;
	}

	// ����� ������ Exit ȣ��
	m_curScene->Exit();

	// ������ ����
	m_curScene = m_scene[(UINT)_next];

	// �������� ���� �����Լ� ȣ��
	m_curScene->Enter(_gameProcess);
}
