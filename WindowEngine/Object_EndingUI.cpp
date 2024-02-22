#include "pch.h"
#include "Object_EndingUI.h"
#include "ResourceManager.h"
#include "KeyManager.h"
#include "Texture.h"
#include "EventManager.h"

// ��忣�� ��������
bool Object_EndingUI::m_isBad = false;

Object_EndingUI::Object_EndingUI(GameProcess* _gameProcess)
	:m_dragDistance(0.f),m_sceneTex{},m_curSceneIndex(0),m_startDrag{0.f,0.f}
{
	SetScale(Vector2(1920.f, 1080.f));
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();

	m_tex = resourceManager->LoadTexture(L"nullscene1", L"\\Texture\\scene\\null.bmp", pathManager, mainDC);
	// ���ҷ�����
	m_sceneTex[0] = resourceManager->LoadTexture(L"cartoonscen6", L"\\Texture\\scene\\6.bmp", pathManager, mainDC);
	m_sceneTex[1] = resourceManager->LoadTexture(L"cartoonscen7", L"\\Texture\\scene\\7.bmp", pathManager, mainDC);
	m_sceneTex[2] = resourceManager->LoadTexture(L"cartoonscen8", L"\\Texture\\scene\\8.bmp", pathManager, mainDC);
}

Object_EndingUI::~Object_EndingUI()
{

}

void Object_EndingUI::MouseLbtnDown(GameProcess* _gameProcess)
{
	KeyManager* keyManager = _gameProcess->GetKeyManager();
	m_startDrag = keyManager->GetMousePos();
}

void Object_EndingUI::MouseOn(GameProcess* _gameProcess)
{
	if (IsLbtnDown())
	{
		KeyManager* keyManager = _gameProcess->GetKeyManager();

		// ����Ŭ���� �Ͼ ������ ���� ���콺 ��ǥ�� ����
		m_dragDistance = m_startDrag.x - keyManager->GetMousePos().x;
	}
}

void Object_EndingUI::MouseLbtnUp(GameProcess* _gameProcess)
{
	EventManager* eventManager = _gameProcess->GetEvenetManager();

	// �����Ÿ��̻� �巡���ϸ� ���� �ƾ����� ����
	// ���������� ���� �ѱ�
	if (m_dragDistance >= 100.f)
	{
		if (m_curSceneIndex == 0 && m_isBad)
			m_curSceneIndex = 1;
		else if (m_curSceneIndex == 0 && !m_isBad)
			m_curSceneIndex = 2;
		else
			eventManager->ChangeScene(SCENE_TYPE::OPTION);
	}
	// �������� ���� �ѱ�
	else if (m_dragDistance <= -100.f)
	{
		// ������ 0���� ����
		m_curSceneIndex = 0;
	}

	m_dragDistance = 0.f;
}

void Object_EndingUI::Update(GameProcess* _gameProcess)
{
	// Ű�Է����ε� ���� �ѱ�� ����
	KeyManager* keymanager = _gameProcess->GetKeyManager();
	EventManager* eventManager = _gameProcess->GetEvenetManager();

	if (keymanager->IsKeyState(KEY::RIGHT, KEY_STATE::TAP))
	{
		if (m_curSceneIndex > 0)
			eventManager->ChangeScene(SCENE_TYPE::OPTION);
		else if (m_isBad)
			m_curSceneIndex = 1;
		else
			m_curSceneIndex = 2;
	}
		if (keymanager->IsKeyState(KEY::LEFT, KEY_STATE::TAP))
	{
				m_curSceneIndex = 0;
	}
}

void Object_EndingUI::Render(HDC _dc)
{
	float dragLen = m_dragDistance * 1.5f;

	// ���ʹ������� �巡��
	if (m_dragDistance >= 0.f)
	{
		Texture* leftTex = m_sceneTex[m_curSceneIndex];
		int leftWidth = leftTex->GetWidth();
		int leftHeight = leftTex->GetHeight();

		BitBlt(_dc, 0, 0, leftWidth, leftHeight, leftTex->GetDC()
			, (int)dragLen, 0, SRCCOPY);

		if (m_curSceneIndex ==0 && m_isBad)
		{
			Texture* rightTex = m_sceneTex[m_curSceneIndex + 1];
			int rightWidth = rightTex->GetWidth();
			int rightHeight = rightTex->GetHeight();

			BitBlt(_dc, 1920 - (int)dragLen, 0, rightWidth, rightHeight, rightTex->GetDC(), 0, 0, SRCCOPY);
		}
		else if (m_curSceneIndex == 0 && !m_isBad)
		{
			Texture* rightTex = m_sceneTex[m_curSceneIndex + 2];
			int rightWidth = rightTex->GetWidth();
			int rightHeight = rightTex->GetHeight();

			BitBlt(_dc, 1920 - (int)dragLen, 0, rightWidth, rightHeight, rightTex->GetDC(), 0, 0, SRCCOPY);
		}
		else
		{
			int rightWidth = m_tex->GetWidth();
			int rightHeight = m_tex->GetHeight();
			BitBlt(_dc, 1920 - (int)dragLen, 0, rightWidth, rightHeight, m_tex->GetDC(), 0, 0, SRCCOPY);
		}

	}
	else // �����ʹ������� �巡��
	{
		if (m_curSceneIndex != 0)
		{
			Texture* leftTex = m_sceneTex[0];
			int leftWidth = leftTex->GetWidth();
			int leftHeight = leftTex->GetHeight();

			BitBlt(_dc, -1920 - (int)dragLen, 0, leftWidth, leftHeight, leftTex->GetDC()
				, 0, 0, SRCCOPY);
		}
		else
		{
			int leftWidth = m_tex->GetWidth();
			int leftHeight = m_tex->GetHeight();

			BitBlt(_dc, -1920 - (int)dragLen, 0, leftWidth, leftHeight, m_tex->GetDC()
				, 0, 0, SRCCOPY);
		}

		Texture* rightTex = m_sceneTex[m_curSceneIndex];
		int rightWidth = rightTex->GetWidth();
		int rightHeight = rightTex->GetHeight();

		BitBlt(_dc, (int)-dragLen, 0, rightWidth, rightHeight, rightTex->GetDC(), 0, 0, SRCCOPY);
	}
}
