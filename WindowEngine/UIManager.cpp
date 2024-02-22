#include "pch.h"

#include "Scene.h"
#include "Object_UI.h"

#include "UIManager.h"
#include "SceneManager.h"
#include "KeyManager.h"

UIManager::UIManager()
	:m_focusedUI(nullptr)
{}

UIManager::~UIManager()
{}

void UIManager::Update(GameProcess* gameProcess)
{
	SceneManager* sceneManager = gameProcess->GetSceneManager();
	EventManager* eventManager =gameProcess->GetEvenetManager();
	KeyManager* keyManager = gameProcess->GetKeyManager();

	// 1. 포거스된 UI 확인
	m_focusedUI = GetFocusedUI(keyManager, sceneManager);

	if (!m_focusedUI)
		return;

	// 2. 포커스된 UI내에서, 부모 UI포함, 자식UI들 중 실제 타겟팅된 UI를 가져온다.
	Object_UI* targetUI = GetTargetedUI(m_focusedUI, keyManager);

	bool LbtnAway = keyManager->IsKeyState(KEY::LBTN, KEY_STATE::AWAY);
	bool LbttnTap = keyManager->IsKeyState(KEY::LBTN, KEY_STATE::TAP);

	if (nullptr != targetUI)
	{
		targetUI->MouseOn(gameProcess);

		if (LbttnTap)
		{
			targetUI->MouseLbtnDown(gameProcess);
			targetUI->m_LbtnDown = true;
		}
		else if (LbtnAway)
		{
			targetUI->MouseLbtnUp(gameProcess);

			if (targetUI->m_LbtnDown)
			{
				targetUI->MouseLbtnClicked(gameProcess);
			}

			// 왼쪽버튼 떼며, 눌렸던 체크를 다시 해제한다.
			targetUI->m_LbtnDown = false;
		}

	}

}

void UIManager::SetFocusedUI(Object_UI* _UI, SceneManager* _sceneManager)
{
	// 이미 포커싱 중인 경우 or 포커싱 해제 요청인 경우
	if (m_focusedUI == _UI || nullptr == _UI)
	{
		m_focusedUI = _UI;
		return;
	}

	m_focusedUI = _UI;

	// 현재씬의 UI그룹
	Scene* curScene = _sceneManager->GetCurScene();
	vector<Object*>& vecUI = curScene->GetGroupUI();

	// 포커싱된 UI 배열에서 탐색
	auto iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		if (m_focusedUI == *iter)
		{
			break;
		}
	}

	vecUI.erase(iter);
	// 가장 상위 UI 
	vecUI.push_back(m_focusedUI);

}

void UIManager::Initialize()
{
}

Object_UI* UIManager::GetFocusedUI(KeyManager* _keyManager, SceneManager* _sceneManager)
{
	Scene* curScene = _sceneManager->GetCurScene();
	vector<Object*>& vecUI = curScene->GetGroupUI();
	bool LbtnTap = _keyManager->IsKeyState(KEY::LBTN, KEY_STATE::TAP);

	// 기존 포커싱 ui를 받아두고 변경되었는지 확인
	Object_UI* focusedUI = m_focusedUI;
	
	// 포커스된 UI 삭제예정이라면 해제
	if (m_focusedUI && m_focusedUI->IsDead())
		focusedUI = nullptr;

	// 마우스 왼쪽버튼이 눌리지 않은 상태면 포커스 UI는변경되지 않으므로 기존 UI 반환
	if (!LbtnTap)
	{
		return focusedUI;
	}

	// 왼쪽 버튼이 Tap이 발생함
	auto target_iter = vecUI.end();
	for (auto iter = vecUI.begin(); iter != vecUI.end(); ++iter)
	{
		// PauseUI 인경우에는 다른 ui이벤트들을 발생시키지 않는다
 		if ((*iter)->GetName() == L"PauseUI" && !(*iter)->IsDead()) 
 		{
 			target_iter = iter;
 			break;
 		}
		// 마우스위에 있으면서 삭제되지 않는 오브젝트
		if (((Object_UI*)*iter)->IsMouseOn() && !(*iter)->IsDead())
		{
			target_iter = iter;
		}
	}
	
	// 이번에 Focus 된 UI가 없다
	if (vecUI.end() == target_iter)
	{
		return nullptr;
	}
	
	// 현재 포커스 중인 UI 
	focusedUI = (Object_UI*)*target_iter;
	
	// 백터 내에서 맨뒤로 순번 교체
	vecUI.erase(target_iter);
	vecUI.push_back(focusedUI);

	return focusedUI;
}

Object_UI* UIManager::GetTargetedUI(Object_UI* _parentUI, KeyManager* _keyManager)
{
	bool LbtnAway = _keyManager->IsKeyState(KEY::LBTN, KEY_STATE::AWAY);

	// 1. 부모UI 포함, 자식 UI들중 실제 타겟팅된 UI를 가져온다.
	Object_UI* targetUI = nullptr;

	static list<Object_UI*> queue;
	static vector<Object_UI*> noneTarget;

	queue.clear();
	noneTarget.clear();

	queue.push_back(_parentUI);

	while (!queue.empty())
	{
		// 큐에서 데이터 하나 꺼내기
		Object_UI* frontUI = queue.front();
		queue.pop_front();

		// 큐에서 꺼내온 TargetUI 인지 확인
		if (frontUI->IsMouseOn())
		{
			if (nullptr != targetUI)
			{
				noneTarget.push_back(targetUI);
			}

			targetUI = frontUI;
		}
		else
		{
			noneTarget.push_back(frontUI);
		}

		const vector<Object_UI*> child = frontUI->GetChildUI();
		for (size_t i = 0; i < child.size(); ++i)
		{
			queue.push_back(child[i]);
		}
	}

	// 왼쪽 버튼 ,눌렸던 체크를 다시 해제한다.
	if (LbtnAway)
	{
		for (size_t i = 0; i < noneTarget.size(); ++i)
		{
			noneTarget[i]->m_LbtnDown = false;
		}
	}

	return targetUI;
}
