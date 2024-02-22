#pragma once
#include "Object.h"

class Object_TextureBarUI;

enum class OVENSTATE
{
	EMPTY,
	BAKING_UNBAKED,
	BAKING_BAKED,
	OVERCOOKING
};

class Object_Oven :
    public Object
{
public:
	Object_Oven(GameProcess* gameProcess, Object_TextureBarUI* _ovenTimerBar, int _type);
	~Object_Oven();

private:
	float m_ovenTime;
	int m_type;

	const float BAKINGTIME = 10.f;
	const float OVERCOOKINGTIME = 20.f;
	const int SLICE = 150;

	OVENSTATE m_ovenState;

	Object_TextureBarUI* m_ovenTimerBar;

	bool m_isBaking;
	bool m_isBaked;
	bool m_isOverCooked;

// Getter/Setter
public:
	OVENSTATE GetOvenState() { return m_ovenState; }
	void SetOvenState(OVENSTATE _newState) { m_ovenState = _newState; }
	void SetOvenTimerBar(Object_TextureBarUI* _ovenTimerBar) { m_ovenTimerBar = _ovenTimerBar; }

public:
	void UpdateTime(GameProcess* _gameProcess);

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

