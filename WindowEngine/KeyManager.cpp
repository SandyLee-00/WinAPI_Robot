#include "pch.h"
#include "KeyManager.h"
#include "GameProcess.h"

// GetAsyncKeyState() 에는 가상키값을 넣어주어야 함
// 내가 설정한 키와 실제 가상키값을 매칭시켜줌
int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, // LEFT,
	VK_RIGHT, // RIGHT,
	VK_UP, // UP,
	VK_DOWN, // DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	VK_LSHIFT, // Z키 꼼수
	'X',
	'C',
	'V',
	'B',

	VK_MENU, // ALT,
	VK_CONTROL, // CTRL,
	VK_LSHIFT, // LSHIFT,
	VK_SPACE, // SPACE,
	VK_RETURN, // ENTER,
	VK_ESCAPE, // ESC,

	VK_LBUTTON, // LBTN
	VK_RBUTTON, // RBTN

};

KeyManager::KeyManager()
	:m_vCurMousePos{}
	, m_vecKey{}
	, isDisArrKey(false)
	, isDisAllKey(false)
{
}

KeyManager::~KeyManager()
{
}

void KeyManager::Initialize()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void KeyManager::Update(HWND _mainHwnd)
{
	HWND hWnd = GetFocus(); // 현재 포커싱중인 윈도우

	if (nullptr != hWnd)
	{

		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 방향키 비활성화
			if (isDisArrKey)
			{
				if ((i == (UINT)KEY::LEFT) || (i == (UINT)KEY::RIGHT) || (i == (UINT)KEY::UP) || (i == (UINT)KEY::DOWN))
				{
					continue;
				}
			}

			// 방향키 & space & z키 비활성화
			if (isDisAllKey)
			{
				if ((i == (UINT)KEY::LEFT) || (i == (UINT)KEY::RIGHT) || (i == (UINT)KEY::UP)
					|| (i == (UINT)KEY::DOWN) || (i == (UINT)KEY::SPACE) || (i == (UINT)KEY::Z))
				{
					continue;
				}
			}

			// 키가 눌려있다
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에도 눌려있었다
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}
			else
			{
				// 키가 눌려있지 않다
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에는 눌려있었다
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 눌려있지 않았다
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		} // for

		// Mouse 위치 계산
		POINT ptPos = {};
		// 현재 마우스 위치
		GetCursorPos(&ptPos);

		//이것은 Windows API 함수 중 하나인 ScreenToClient 함수입니다.
		//이 함수는 지정된 클라이언트 영역에서의 좌표값을 스크린 좌표계에서의 좌표값으로 변환해줍니다.
		//위 코드에서는 CCore::GetInst()->GetMainHwnd()를 사용하여 윈도우 핸들을 가져오고, &ptPos는 변환된 좌표값을 받을 POINT 구조체의 포인터입니다.
		//따라서 이 코드는 해당 윈도우에서의 포인트(ptPos)를 스크린 좌표계에서의 좌표값으로 변환합니다.
		ScreenToClient(_mainHwnd, &ptPos);

		m_vCurMousePos = Vector2((float)ptPos.x, (float)ptPos.y);

	}
	else ReleaseKey();
}

bool KeyManager::IsKeyState(KEY _key, KEY_STATE _state)
{
	if (_state == GetKeyState(_key))
		return true;

	return false;
}

void KeyManager::ReleaseKey()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey[i].bPrevPush = false;
		if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
		{
			m_vecKey[i].eState = KEY_STATE::AWAY;
		}
		else if (KEY_STATE::AWAY == m_vecKey[i].eState)
		{
			m_vecKey[i].eState = KEY_STATE::NONE;
		}
	}
}

void KeyManager::DisableArrowKey()
{
	isDisArrKey = true;
	isDisAllKey = false;
}

void KeyManager::ActiveArrowKey()
{
	isDisArrKey = false;
	isDisAllKey = false;
}

void KeyManager::DisableAllKey()
{
	isDisArrKey = false;
	isDisAllKey = true;
}

void KeyManager::ActiveAllKey()
{
	isDisArrKey = false;
	isDisAllKey = false;
}
