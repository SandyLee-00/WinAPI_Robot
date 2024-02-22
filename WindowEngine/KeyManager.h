#pragma once

enum class KEY_STATE
{
	TAP, // �� ���� ����
	HOLD, // ������ �ִ�
	AWAY, // �� �� ����
	NONE, // ������ �ʾҰ�, �������� ������ ���� ����
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

	LAST, //enum�� ��

};

struct tKeyInfo
{
	KEY_STATE eState; // Ű�� ���°�
	bool      bPrevPush;  // ���� �����ӿ��� ���ȴ��� ����
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
	// ����Ű �Է� ��Ȱ��ȭ ���� ��� Ű�� NONE���� �ٲ���
	void ReleaseKey();

	// ����Ű �Է� ��Ȱ��ȭ
	void DisableArrowKey();

	// ����Ű �Է� Ȱ��ȭ
	void ActiveArrowKey();

	// ���Ű �Է� ��Ȱ��ȭ
	void DisableAllKey();

	// ���Ű �Է� Ȱ��ȭ
	void ActiveAllKey();

private:
	vector<tKeyInfo> m_vecKey;
	Vector2 m_vCurMousePos;

	bool isDisArrKey;
	bool isDisAllKey;
};