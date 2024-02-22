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

	// 1. ���Ž��� UI Ȯ��
	m_focusedUI = GetFocusedUI(keyManager, sceneManager);

	if (!m_focusedUI)
		return;

	// 2. ��Ŀ���� UI������, �θ� UI����, �ڽ�UI�� �� ���� Ÿ���õ� UI�� �����´�.
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

			// ���ʹ�ư ����, ���ȴ� üũ�� �ٽ� �����Ѵ�.
			targetUI->m_LbtnDown = false;
		}

	}

}

void UIManager::SetFocusedUI(Object_UI* _UI, SceneManager* _sceneManager)
{
	// �̹� ��Ŀ�� ���� ��� or ��Ŀ�� ���� ��û�� ���
	if (m_focusedUI == _UI || nullptr == _UI)
	{
		m_focusedUI = _UI;
		return;
	}

	m_focusedUI = _UI;

	// ������� UI�׷�
	Scene* curScene = _sceneManager->GetCurScene();
	vector<Object*>& vecUI = curScene->GetGroupUI();

	// ��Ŀ�̵� UI �迭���� Ž��
	auto iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		if (m_focusedUI == *iter)
		{
			break;
		}
	}

	vecUI.erase(iter);
	// ���� ���� UI 
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

	// ���� ��Ŀ�� ui�� �޾Ƶΰ� ����Ǿ����� Ȯ��
	Object_UI* focusedUI = m_focusedUI;
	
	// ��Ŀ���� UI ���������̶�� ����
	if (m_focusedUI && m_focusedUI->IsDead())
		focusedUI = nullptr;

	// ���콺 ���ʹ�ư�� ������ ���� ���¸� ��Ŀ�� UI�º������ �����Ƿ� ���� UI ��ȯ
	if (!LbtnTap)
	{
		return focusedUI;
	}

	// ���� ��ư�� Tap�� �߻���
	auto target_iter = vecUI.end();
	for (auto iter = vecUI.begin(); iter != vecUI.end(); ++iter)
	{
		// PauseUI �ΰ�쿡�� �ٸ� ui�̺�Ʈ���� �߻���Ű�� �ʴ´�
 		if ((*iter)->GetName() == L"PauseUI" && !(*iter)->IsDead()) 
 		{
 			target_iter = iter;
 			break;
 		}
		// ���콺���� �����鼭 �������� �ʴ� ������Ʈ
		if (((Object_UI*)*iter)->IsMouseOn() && !(*iter)->IsDead())
		{
			target_iter = iter;
		}
	}
	
	// �̹��� Focus �� UI�� ����
	if (vecUI.end() == target_iter)
	{
		return nullptr;
	}
	
	// ���� ��Ŀ�� ���� UI 
	focusedUI = (Object_UI*)*target_iter;
	
	// ���� ������ �ǵڷ� ���� ��ü
	vecUI.erase(target_iter);
	vecUI.push_back(focusedUI);

	return focusedUI;
}

Object_UI* UIManager::GetTargetedUI(Object_UI* _parentUI, KeyManager* _keyManager)
{
	bool LbtnAway = _keyManager->IsKeyState(KEY::LBTN, KEY_STATE::AWAY);

	// 1. �θ�UI ����, �ڽ� UI���� ���� Ÿ���õ� UI�� �����´�.
	Object_UI* targetUI = nullptr;

	static list<Object_UI*> queue;
	static vector<Object_UI*> noneTarget;

	queue.clear();
	noneTarget.clear();

	queue.push_back(_parentUI);

	while (!queue.empty())
	{
		// ť���� ������ �ϳ� ������
		Object_UI* frontUI = queue.front();
		queue.pop_front();

		// ť���� ������ TargetUI ���� Ȯ��
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

	// ���� ��ư ,���ȴ� üũ�� �ٽ� �����Ѵ�.
	if (LbtnAway)
	{
		for (size_t i = 0; i < noneTarget.size(); ++i)
		{
			noneTarget[i]->m_LbtnDown = false;
		}
	}

	return targetUI;
}
