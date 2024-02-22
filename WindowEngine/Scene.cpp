#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "EventManager.h"
#include "TimeManager.h"

Scene::Scene()
	:m_pauseGame(false)
{
}

Scene::~Scene()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_obj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 j 물체 삭제
			delete m_obj[i][j];
		}

	}
}


void Scene::Start()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_obj[i].size(); ++j)
		{
			m_obj[i][j]->Start();
		}

	}
}

void Scene::Update(GameProcess* gameProcess)
{
	//  UI만을 업데이트해서 게임을 정지한 것 처럼 구현
	if (m_pauseGame)
	{
		for (size_t i = 0; i < m_obj[(UINT)OBJECT_TYPE::UI].size(); ++i)
		{
			m_obj[(UINT)OBJECT_TYPE::UI][i]->Update(gameProcess);
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
		{
			for (size_t j = 0; j < m_obj[i].size(); ++j)
			{
				m_obj[i][j]->Update(gameProcess);
			}

		}
	}
}

void Scene::FinalUpdate(TimeManager* _timeManager,EventManager* _eventManager, KeyManager* _keyManager)
{
	float deltaTime = (float)_timeManager->GetDeltaTime();

	if (m_pauseGame)
	{
		for (size_t i = 0; i < m_obj[(UINT)OBJECT_TYPE::UI].size(); ++i)
		{
			m_obj[(UINT)OBJECT_TYPE::UI][i]->Finalupdate(deltaTime, _eventManager, _keyManager);
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
		{
			for (size_t j = 0; j < m_obj[i].size(); ++j)
			{
				m_obj[i][j]->Finalupdate(deltaTime, _eventManager, _keyManager);
			}

		}
	}
}

void Scene::Render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		// iterator 로 순회
		auto iter = m_obj[i].begin();
		for (; iter != m_obj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(_dc);
				++iter;
			}
			else
			{
				iter = m_obj[i].erase(iter);
			}
		}
	}
}

void Scene::DeleteGroup(OBJECT_TYPE _target)
{
   Safe_Delete_Vec<Object*>(m_obj[(UINT)_target]);
}

void Scene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
		DeleteGroup((OBJECT_TYPE)i);
}


