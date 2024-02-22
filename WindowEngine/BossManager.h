#pragma once

/// <summary>
/// 사장님을 관리하기 위한 메니저
/// 사장님 출현 주기, 이동 경로 등 관리
/// </summary>
class BossManager
{
	BossManager();
	~BossManager();

	// GameProcess에서 BossManager의 멤버에 접근할 수 있음
	// private 생성자도 부를 수 있음
	friend class GameProcess;

private:

	// 사장님 출현 타이머
	static double m_bossTimer;

	PathPoint m_start; // 사장님 시작 지점
	PathPoint m_end; // 사장님 최종 도착 지점
	vector<PathPoint> m_patrolPath[(int)PATH_TYPE::END]; // 순회 경로

	/// 경로는 하나의 path 목적지는 path의 points
	UINT m_pathIndex; // 경로 index
	UINT m_destIndex; // 목적지 index

	Vector2 m_destination; // 사장님 목표 지점

	// 기본 Destination Point
	PathPoint m_point01;
	PathPoint m_point02;
	PathPoint m_point03;
	PathPoint m_point04;

	UINT m_windowPeriod; // 처음 창문에 사장님이 나오기까지의 시간
	UINT m_appearPeriod; // 창문에서 보이고 나서 매장에 사장이 들어오는 시간
	UINT m_disAppearPeriod; // 창문에서 사장님이 Up 하고 사라지기 까지의 시간

	double m_halfPercent;

	bool m_isAppear; // 사장님 출현 여부

	/// 애니메이션 한 번만 재생
	bool m_isUpWindow; // 사장님이 창 위로 올라오는 애니메이션 출력 여부
	bool m_isWalkingWindow; // 사장님이 창 위로 올라오는 애니메이션 출력 여부

public:

	void Initialize();
	void Update(double deltaTime, GameProcess* _gameProcess);

	Vector2 GetDestPos() { return m_destination; }

	// 다음 목적지 탐색 및 변경
	bool FindNextDest();

	// 보스 생성
	void CreateBoss(GameProcess* _gameProcess);

	// 보스 종료
	void DisappearBoss();

	// 랜덤 경로 index 설정
	UINT GetRandPathIndex();

	// 랜덤 창문 사장님 출현 주기 설정
	UINT GetRandWindowPeriod();

	// 0.0 ~ 1.0 사이의 난수 반환 함수
	double GetHalfPercent();
};