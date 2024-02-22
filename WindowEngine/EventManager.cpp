#include "pch.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Object.h"
#include "UIManager.h"

EventManager::EventManager()
	:m_event{}
	,m_deadObj{}
{}

EventManager::~EventManager()
{}

void EventManager::Initialize()
{
}

void EventManager::Update(GameProcess* gmaeProcess)
{
	// ===================================================
    // ���� �����ӿ��� ����ص� Dead Object ���� �����Ѵ�.
    // ===================================================

	for (size_t i = 0; i < m_deadObj.size(); ++i)
	{
		// ���� ������Ʈ �޸� ����
		delete m_deadObj[i];
	}
	// ���� �迭 Ŭ����
	m_deadObj.clear();

	// ============
	//  Event ó��
	// ============
	for (size_t i = 0; i < m_event.size(); ++i)
	{
		// �̹� �����ӿ��� �߻��� �̺�Ʈ���� ó��
		ExecuteEvent(m_event[i], gmaeProcess);
	}

	// ó���� �̺�Ʈ�� Ŭ����
	m_event.clear();
}

void EventManager::ExecuteEvent(const Event& _event, GameProcess* gmaeProcess)
{
	SceneManager* sceneManager = gmaeProcess->GetSceneManager();
	UIManager* UIManager = gmaeProcess->GetUIManager();

	switch (_event.type)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : ������Ʈ �ּ�
		// wParam : ������Ʈ Ÿ��
		Object* obj = (Object*)_event.lParam;
		OBJECT_TYPE type = (OBJECT_TYPE)_event.wParam;

		sceneManager->GetCurScene()->AddObject(obj, type);
	}
		break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : ������Ʈ �ּ�
		Object* obj = (Object*)_event.lParam;

		// ������Ʈ�� ���� ����ó���� ���ؼ� dead���·� �����.
		if (!obj->IsDead())
		{
			obj->SetDead();
			//m_deadObj.push_back(obj);
		}
	}

		break;

	case EVENT_TYPE::CHANGE_SCENE:

		// lParam : ���� �� Ÿ��

		// Scene ����
		sceneManager->ChangeScene((SCENE_TYPE)_event.lParam, gmaeProcess);

		UIManager->SetFocusedUI(nullptr, sceneManager);

		break;
	}
}

void EventManager::CreateObject(Object* _obj, OBJECT_TYPE _type)
{
	Event evn = {};
	evn.type = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_obj;
	evn.wParam = (DWORD_PTR)_type;

	AddEvent(evn);
}

void EventManager::DeleteObject(Object* _obj)
{
	assert(_obj);

	Event evn = {};
	evn.type = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_obj;

	AddEvent(evn);
}

void EventManager::ChangeScene(SCENE_TYPE _type)
{
	Event evn = {};
	evn.type = EVENT_TYPE::CHANGE_SCENE;
	evn.lParam = (DWORD_PTR)_type;

	AddEvent(evn);
}
