#include "pch.h"
#include "TimgMgr.h"

TimgMgr::TimgMgr()
	:m_curentCount{}
	,m_prevCount{}
	,m_frequency{}
	,m_deltaTime{}
	,m_frameTime{}
	,m_frameCount{}
	, m_fps{}
{
}

TimgMgr::~TimgMgr()
{
}

void TimgMgr::Init()
{
	QueryPerformanceFrequency(&m_frequency); // 프로세서의 카운터 값의 빈도
	QueryPerformanceCounter(&m_prevCount); // 이전 프레임의 카운터 값
}

// deltaTime : 이전 프레임부터 현재 프레임까지의 경과 시간
// fps : 1초당 프레임 수
void TimgMgr::Update()
{
	QueryPerformanceCounter(&m_curentCount);

	// deltaTime = (현재 카운터 값 - 현재 카운터 값) / 프로세서 카운터 값의 빈도
	m_deltaTime = (m_curentCount.QuadPart - m_prevCount.QuadPart) / (double)m_frequency.QuadPart;
	m_prevCount = m_curentCount;

	++m_frameCount;
	m_frameTime += m_deltaTime;

	// 1초가 되면 몇 번 m_frameCount가 증가했는지 확인
	if (m_frameTime >= 1.f)
	{
		m_fps = m_frameCount;

		m_frameCount = 0;
		m_frameTime = 0.f;

	}

#ifdef _DEBUG

	// 디버그 모드일 때 다른 창 갔다와서 돌아오면 deltaTime이 너무 커지는 현상 방지
	if (m_deltaTime > 0.1)
	{
		m_deltaTime = 0.1;
	}

#endif
}

void TimgMgr::Render()
{

}

/*
FORCEINLINE static double GetDeltaTime()
	{
		return DeltaTime;
	}

static void SetDeltaTime(double Seconds)
	{
		DeltaTime = Seconds;
	}

	FApp::SetDeltaTime(FApp::GetCurrentTime() - LastRealTime);

// This is always in realtime and is not adjusted by fixed framerate. Start slightly below current real time
	static double LastRealTime = FPlatformTime::Seconds() - 0.0001;

// Updates logical last time to match logical current time from last tick
	FApp::UpdateLastTime();

static FORCEINLINE double Seconds()
	{
		struct timeval tv;
		gettimeofday( &tv, NULL );
		return ((double) tv.tv_sec) + (((double) tv.tv_usec) / 1000000.0);
	}

*/

