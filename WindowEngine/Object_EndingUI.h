#pragma once
#include "Object_UI.h"

class Texture;

/// <summary>
/// 게임의 엔딩 UI 오브젝트 
///
/// 이기태 2023/05/24
/// </summary>
class Object_EndingUI :
    public Object_UI
{
public:
	static bool  m_isBad;

private:
	// 마우스 좌클릭 시작좌표
	Vector2 m_startDrag;
	// 컷씬 인덱스
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

