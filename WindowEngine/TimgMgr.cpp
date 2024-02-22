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
	QueryPerformanceFrequency(&m_frequency); // ���μ����� ī���� ���� ��
	QueryPerformanceCounter(&m_prevCount); // ���� �������� ī���� ��
}

// deltaTime : ���� �����Ӻ��� ���� �����ӱ����� ��� �ð�
// fps : 1�ʴ� ������ ��
void TimgMgr::Update()
{
	QueryPerformanceCounter(&m_curentCount);

	// deltaTime = (���� ī���� �� - ���� ī���� ��) / ���μ��� ī���� ���� ��
	m_deltaTime = (m_curentCount.QuadPart - m_prevCount.QuadPart) / (double)m_frequency.QuadPart;
	m_prevCount = m_curentCount;

	++m_frameCount;
	m_frameTime += m_deltaTime;

	// 1�ʰ� �Ǹ� �� �� m_frameCount�� �����ߴ��� Ȯ��
	if (m_frameTime >= 1.f)
	{
		m_fps = m_frameCount;

		m_frameCount = 0;
		m_frameTime = 0.f;

	}

#ifdef _DEBUG

	// ����� ����� �� �ٸ� â ���ٿͼ� ���ƿ��� deltaTime�� �ʹ� Ŀ���� ���� ����
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

