#pragma once
#include "Object_UI.h"
class Object_TimeUI :
	public Object_UI
{
	/// <summary>
	/// �ð� ǥ���ϴ� UI 
	/// </summary>
private:
	int m_hour;
	int m_minute;
public:
	/// <summary>
	/// ���� �־�� UI�� ������Ʈ �ܰ迡�� �ڵ����� �ð� ������ ȯ������
	/// </summary>
	/// <param name="_minute"></param>
	void SetMinute(int _minute);
	int GetHour() { return m_hour; }

	void Render(HDC _dc) override;
	void Update(GameProcess* _gameProcess) override;

	Object_TimeUI(GameProcess* gameProcess);
	virtual~Object_TimeUI();
};

