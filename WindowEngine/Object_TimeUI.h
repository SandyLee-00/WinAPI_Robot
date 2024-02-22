#pragma once
#include "Object_UI.h"
class Object_TimeUI :
	public Object_UI
{
	/// <summary>
	/// 시간 표시하는 UI 
	/// </summary>
private:
	int m_hour;
	int m_minute;
public:
	/// <summary>
	/// 분을 넣어면 UI가 업데이트 단계에서 자동으로 시간 분으로 환산해줌
	/// </summary>
	/// <param name="_minute"></param>
	void SetMinute(int _minute);
	int GetHour() { return m_hour; }

	void Render(HDC _dc) override;
	void Update(GameProcess* _gameProcess) override;

	Object_TimeUI(GameProcess* gameProcess);
	virtual~Object_TimeUI();
};

