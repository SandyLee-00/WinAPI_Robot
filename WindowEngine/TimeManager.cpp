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
	QueryPerformanceFrequency(&m_frequency); // ���μ����� ī���� ���� ��
	QueryPerformanceCounter(&m_prevCount); // ���� �������� ī���� ��
}

// deltaTime / fps Update �ϸ鼭 �����Ӹ��� ���� �ٲ�
double TimeManager::Update(HWND _main)
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

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_fps, m_deltaTime);
		SetWindowText(_main, szBuffer);

	}

#ifdef _DEBUG

	// ����� ����� �� �ٸ� â ���ٿͼ� ���ƿ��� deltaTime�� �ʹ� Ŀ���� ���� ����
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

