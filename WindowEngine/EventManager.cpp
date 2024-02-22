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
    // 이전 프레임에서 등록해둔 Dead Object 들을 삭제한다.
    // ===================================================

	for (size_t i = 0; i < m_deadObj.size(); ++i)
	{
		// 죽은 오브젝트 메모리 해제
		delete m_deadObj[i];
	}
	// 데드 배열 클리어
	m_deadObj.clear();

	// ============
	//  Event 처리
	// ============
	for (size_t i = 0; i < m_event.size(); ++i)
	{
		// 이번 프레임에서 발생한 이벤트들을 처리
		ExecuteEvent(m_event[i], gmaeProcess);
	}

	// 처리한 이벤트들 클리어
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
		// lParam : 오브젝트 주소
		// wParam : 오브젝트 타입
		Object* obj = (Object*)_event.lParam;
		OBJECT_TYPE type = (OBJECT_TYPE)_event.wParam;

		sceneManager->GetCurScene()->AddObject(obj, type);
	}
		break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : 오브젝트 주소
		Object* obj = (Object*)_event.lParam;

		// 오브젝트에 대한 예외처리를 위해서 dead상태로 만든다.
		if (!obj->IsDead())
		{
			obj->SetDead();
			//m_deadObj.push_back(obj);
		}
	}

		break;

	case EVENT_TYPE::CHANGE_SCENE:

		// lParam : 다음 씬 타입

		// Scene 변경
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
