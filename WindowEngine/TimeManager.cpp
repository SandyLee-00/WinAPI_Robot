#include "pch.h"
#include "TimeManager.h"

TimeManager::TimeManager()
	:m_curentCount{}
	,m_prevCount{}
	,m_frequency{}
	,m_deltaTime{}
	,m_frameTime{}
	,m_frameCount{}
	, m_fps{}
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_frequency); // 프로세서의 카운터 값의 빈도
	QueryPerformanceCounter(&m_prevCount); // 이전 프레임의 카운터 값
}

// deltaTime / fps Update 하면서 프레임마다 값이 바뀜
double TimeManager::Update(HWND _main)
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

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_fps, m_deltaTime);
		SetWindowText(_main, szBuffer);

	}

#ifdef _DEBUG

	// 디버그 모드일 때 다른 창 갔다와서 돌아오면 deltaTime이 너무 커지는 현상 방지
	//if (m_deltaTime > 0.1)
	//{
	//	m_deltaTime = 0.1;
	//}

#endif

	return m_deltaTime;
}

void TimeManager::Render()
{

}

