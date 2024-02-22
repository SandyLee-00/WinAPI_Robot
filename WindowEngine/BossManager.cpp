#include "pch.h"
#include "BossManager.h"
#include "GameProcess.h"
#include "Object_Boss.h"
#include "Object_Window.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Scene_Stage01.h"
#include "SoundManager.h"
#include "Scene_Stage01.h"
#include "EventManager.h"

double BossManager::m_bossTimer = 0.f;

BossManager::BossManager() : m_start{}, m_end{}, m_pathIndex(), m_destIndex(), m_destination(), m_windowPeriod(), m_disAppearPeriod(), m_appearPeriod(), m_isAppear(), m_halfPercent(), m_isUpWindow(), m_isWalkingWindow(), m_point01{}, m_point02{}, m_point03{}, m_point04{}
{
}

BossManager::~BossManager()
{
}

void BossManager::Initialize()
{
	m_windowPeriod = GetRandWindowPeriod();
	m_appearPeriod = 5; // 창문에 걸어가는 게 보이고 5초 후에 매장에 들어오심
	m_disAppearPeriod = 3;
	m_halfPercent = GetHalfPercent();

	/// 우하단, 우상단, 좌상단, 좌하단 순서로 1, 2, 3, 4
	// 기본 경로 Points
	m_point01 = PathPoint(1155, 810); // 1번 지점
	m_point02 = PathPoint(1155, 400); // 2번 지점
	m_point03 = PathPoint(310, 400); // 3번 지점
	m_point04 = PathPoint(310, 810); // 4번 지점

	// 시작 & 종료 지점
	m_start = PathPoint(1155, 1200);
	m_end = m_start;

	// 0번 경로
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_point01); // 첫번째 지점은 동일
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_point01); // 마지막 전 지점도 동일
	m_patrolPath[(int)PATH_TYPE::ZERO].push_back(m_end); // 마지막 지점도 동일

	// 1번 경로
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::ONE].push_back(m_end);

	// 2번 경로
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::TWO].push_back(m_end);

	// 3번 경로
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::THREE].push_back(m_end);

	// 4번 경로
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(1155, 565));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(800, 200));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(595, 200));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(200, 555));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(600, 700));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(PathPoint(800, 810));
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::FOUR].push_back(m_end);

	// 5번 경로
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(PathPoint(1155, 170));
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(PathPoint(695, 400));
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(PathPoint(310, 630));
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::FIVE].push_back(m_end);

	// 6번 경로
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(695, 300));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(695, 395));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(695, 200));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(695, 300));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(310, 500));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(195, 500));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(435, 500));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(310, 500));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(685, 710));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(685, 810));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(685, 660));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(PathPoint(685, 710));
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::SIX].push_back(m_end);

	// 7번 경로
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(PathPoint(1255, 180));
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(PathPoint(1155, 710));
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(PathPoint(500, 300));
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(PathPoint(900, 250));
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::SEVEEN].push_back(m_end);

	// 8번 경로
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(PathPoint(685, 710));
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_point03);
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(PathPoint(855, 200));
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(PathPoint(1055, 565));
	m_patrolPath[(int)PATH_TYPE::EIGHT].push_back(m_end);

	// 9번 경로
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(PathPoint(685, 810));
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(m_point04);
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(m_point01);
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(PathPoint(1255, 150));
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(m_point02);
	m_patrolPath[(int)PATH_TYPE::NINE].push_back(m_end);

	// 10번 경로
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(1155, 365));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(685, 265));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(705, 210));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(910, 210));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(910, 365));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(685, 365));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(310, 365));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(310, 640));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(800, 640));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(800, 780));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(550, 780));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(550, 640));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(800, 640));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(PathPoint(1275, 640));
	m_patrolPath[(int)PATH_TYPE::TEN].push_back(m_end);

	// 11번 경로
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(1155, 435));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(465, 435));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(465, 310));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(900, 310));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(900, 435));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(PathPoint(1155, 435));
	m_patrolPath[(int)PATH_TYPE::ELEVEN].push_back(m_end);

}

void BossManager::Update(double deltaTime, GameProcess* _gameProcess)
{
	/// 사장님 Logic
		// 1. 창문 등장 애니메이션 50 %
		// 2. 창문 그림자 출력 애니메이션 50 %

	SceneManager* sceneManager = _gameProcess->GetSceneManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	Scene* currentScene = sceneManager->GetCurScene();
	Scene_Stage01* stage01 = dynamic_cast<Scene_Stage01*>(currentScene);


	// 사장님이 등장하지 않은 상태면 시간이 간다
	if (!m_isAppear)
	{
		// 사장님 타이머 증가
		m_bossTimer += deltaTime;

		// m_windowPeriod 초 후 창문에 등장
		if (m_bossTimer >= m_windowPeriod)
		{
			// 50% 확률로 -> Up
			if (m_halfPercent < 0.5f)
			{
				// 한 번만 UpWindow 애니메이션 출력
				if (!m_isUpWindow)
				{
					// 사장님 UP Anim
					stage01->PlayUpWindow(soundManager);
					m_isUpWindow = true;
				}

				/// 사장님 삭제 및 타이머 다시 시작 시점
				// m_disAppearPeriod 초 후 사장님 사라짐
				if (m_bossTimer >= m_windowPeriod + m_disAppearPeriod)
				{
					m_bossTimer = 0.f; // 다시 사장님 타이머 돈다
					DisappearBoss();
				}
			}
			// 50% 확률로 -> Walking
			else
			{
				// 한 번만 WalkingWindow 애니메이션 출력
				if (!m_isWalkingWindow)
				{
					// 사장님 Walk Anim
					stage01->PlayWalkingWindow(soundManager);
					m_isWalkingWindow = true;
				}

				/// 사장님 생성 및 타이머 다시 시작 시점
				// m_appearPeriod 초 후 등장
				if (m_bossTimer >= m_windowPeriod + m_appearPeriod)
				{
					m_bossTimer = 0.f;
					m_isAppear = true; // 사장님 출현

					// 사장님 목적지 랜덤 설정
					m_pathIndex = GetRandPathIndex();
					m_destination = m_patrolPath[m_pathIndex][m_destIndex];

					// 사장님 생성
					CreateBoss(_gameProcess);
				}
			}
		}
	}
}

void BossManager::DisappearBoss()
{
	// 초기화
	m_isAppear = false;
	m_destIndex = 0;
	m_bossTimer = 0;
	m_windowPeriod = GetRandWindowPeriod();
	m_halfPercent = GetHalfPercent();
	m_isUpWindow = false;
	m_isWalkingWindow = false;
}

bool BossManager::FindNextDest()
{
	size_t destCount = m_patrolPath[m_pathIndex].size();

	if (m_destIndex == destCount - 1)
	{
		return false; // 다음 목적지 못찾음
	}

	// 다음 목적지 좌표 설정
	m_destIndex++;
	m_destination = m_patrolPath[m_pathIndex][m_destIndex];

	return true; // 다음 목적지 찾음
}

void BossManager::CreateBoss(GameProcess* _gameProcess)
{
	// 사장님 생성
	Object_Boss* boss = new Object_Boss(_gameProcess);
	boss->SetPos(Vector2(m_start.x, m_start.y)); // 시작 지점은 고정
	boss->SetScale(Vector2(70.f, 70.f));

	// 현재 씬에 사장님 배치
	EventManager* eventManager = _gameProcess->GetEvenetManager();
	eventManager->CreateObject(boss, OBJECT_TYPE::BOSS);

	Scene* curScene = _gameProcess->GetSceneManager()->GetCurScene();
	Scene_Stage01* stage01 = dynamic_cast<Scene_Stage01*>(curScene);
	stage01->RegisterBoss(boss);
}

UINT BossManager::GetRandPathIndex()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<UINT> dis(0, (int)PATH_TYPE::END - 1);

	return dis(gen);
}

UINT BossManager::GetRandWindowPeriod()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<UINT> dis(5, 7);

	return dis(gen);
}

double BossManager::GetHalfPercent()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);

	double probability = dis(gen); // 0.0부터 1.0 사이의 난수 생성

	return probability;
}
