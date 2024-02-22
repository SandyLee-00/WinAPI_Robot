#pragma once

enum class KEY_STATE
{
	TAP, // 막 누른 시점
	HOLD, // 누르고 있는
	AWAY, // 막 뗀 시점
	NONE, // 눌리지 않았고, 이전에도 눌리지 않은 상태
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST, //enum의 끝

};

struct tKeyInfo
{
	KEY_STATE eState; // 키의 상태값
	bool      bPrevPush;  // 이전 프레임에서 눌렸는지 여부
};

class KeyManager
{
	KeyManager();
	~KeyManager();

	friend class GameProcess;

public:
	void Initialize();
	void Update(HWND _mainHwnd);
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vector2 GetMousePos() { return m_vCurMousePos; }

	bool IsKeyState(KEY _key, KEY_STATE _state);
	// 방향키 입력 비활성화 전에 모든 키를 NONE으로 바꿔줌
	void ReleaseKey();

	// 방향키 입력 비활성화
	void DisableArrowKey();

	// 방향키 입력 활성화
	void ActiveArrowKey();

	// 모든키 입력 비활성화
	void DisableAllKey();

	// 모든키 입력 활성화
	void ActiveAllKey();

private:
	vector<tKeyInfo> m_vecKey;
	Vector2 m_vCurMousePos;

	bool isDisArrKey;
	bool isDisAllKey;
};