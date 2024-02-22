#pragma once

// deltaTime / FPS 관리 클래스
class TimeManager
{
	TimeManager();
	~TimeManager();

	friend class GameProcess;

public:
	void Initialize();
	double Update(HWND _main);
	void Render();

	UINT GetFps() { return m_fps; }
	double GetDeltaTime() { return m_deltaTime; }

private:
	LARGE_INTEGER m_curentCount;
	LARGE_INTEGER m_prevCount;
	LARGE_INTEGER m_frequency;

	double m_deltaTime; // 이전 프레임부터 현재 프레임까지의 경과 시간
	double m_frameTime; // 1초마다 초기화

	UINT m_frameCount; // 프레임 수
	UINT m_fps; // 1초당 프레임 수

};

