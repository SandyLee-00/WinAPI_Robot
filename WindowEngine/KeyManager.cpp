#include "pch.h"
#include "KeyManager.h"
#include "GameProcess.h"

// GetAsyncKeyState() ���� ����Ű���� �־��־�� ��
// ���� ������ Ű�� ���� ����Ű���� ��Ī������
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
	VK_LSHIFT, // ZŰ �ļ�
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
	HWND hWnd = GetFocus(); // ���� ��Ŀ������ ������

	if (nullptr != hWnd)
	{

		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// ����Ű ��Ȱ��ȭ
			if (isDisArrKey)
			{
				if ((i == (UINT)KEY::LEFT) || (i == (UINT)KEY::RIGHT) || (i == (UINT)KEY::UP) || (i == (UINT)KEY::DOWN))
				{
					continue;
				}
			}

			// ����Ű & space & zŰ ��Ȱ��ȭ
			if (isDisAllKey)
			{
				if ((i == (UINT)KEY::LEFT) || (i == (UINT)KEY::RIGHT) || (i == (UINT)KEY::UP)
					|| (i == (UINT)KEY::DOWN) || (i == (UINT)KEY::SPACE) || (i == (UINT)KEY::Z))
				{
					continue;
				}
			}

			// Ű�� �����ִ�
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPush)
				{
					// �������� �����־���
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}
			else
			{
				// Ű�� �������� �ʴ�
				if (m_vecKey[i].bPrevPush)
				{
					// �������� �����־���
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �������� �ʾҴ�
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}
		} // for

		// Mouse ��ġ ���
		POINT ptPos = {};
		// ���� ���콺 ��ġ
		GetCursorPos(&ptPos);

		//�̰��� Windows API �Լ� �� �ϳ��� ScreenToClient �Լ��Դϴ�.
		//�� �Լ��� ������ Ŭ���̾�Ʈ ���������� ��ǥ���� ��ũ�� ��ǥ�迡���� ��ǥ������ ��ȯ���ݴϴ�.
		//�� �ڵ忡���� CCore::GetInst()->GetMainHwnd()�� ����Ͽ� ������ �ڵ��� ��������, &ptPos�� ��ȯ�� ��ǥ���� ���� POINT ����ü�� �������Դϴ�.
		//���� �� �ڵ�� �ش� �����쿡���� ����Ʈ(ptPos)�� ��ũ�� ��ǥ�迡���� ��ǥ������ ��ȯ�մϴ�.
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
