#pragma once
#include "Object_UI.h"

class Texture;

/// <summary>
/// ������ ���� UI ������Ʈ 
///
/// �̱��� 2023/05/24
/// </summary>
class Object_EndingUI :
    public Object_UI
{
public:
	static bool  m_isBad;

private:
	// ���콺 ��Ŭ�� ������ǥ
	Vector2 m_startDrag;
	// �ƾ� �ε���
	int m_curSceneIndex;

	float m_dragDistance;
	Texture* m_sceneTex[3];
	

public:
    Object_EndingUI(GameProcess* _gameProcess);
    ~Object_EndingUI();

	void MouseLbtnDown(GameProcess* _gameProcess) override;
	void MouseOn(GameProcess* _gameProcess) override;
	void MouseLbtnUp(GameProcess* _gameProcess) override;

	void Update(GameProcess* _gameProcess);
	void Render(HDC _dc);
};

