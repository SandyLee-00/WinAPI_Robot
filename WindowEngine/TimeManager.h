#pragma once

// deltaTime / FPS ���� Ŭ����
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

	double m_deltaTime; // ���� �����Ӻ��� ���� �����ӱ����� ��� �ð�
	double m_frameTime; // 1�ʸ��� �ʱ�ȭ

	UINT m_frameCount; // ������ ��
	UINT m_fps; // 1�ʴ� ������ ��

};

