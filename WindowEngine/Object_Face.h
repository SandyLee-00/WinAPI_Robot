#pragma once
#include "Object.h"

enum class NAME
{
	SEOYEONG = 0,
	JUNYEOL = 1,
	YEONHA = 2,
	GITAE = 3,

	GIBIN = 4,
	SANGHYEOK = 5,

	JUHYUN = 6,
	EUNJI = 7,
	MINJI = 8,

	END = 9,
};

class Object_Face :
    public Object
{
/*
�� �ؽ��� �� ����� �°� �׸���
���� ���� �� ����� �°� �����Ѵ�
���콺 ȣ���Ǹ� ���� ����
���콺 ȣ���Ǹ� ���� �󱼷� �ٲ۴�

*/

public:
    Object_Face(GameProcess* _gameProcess, NAME _name);
	~Object_Face();

// ���� �⺻ ����
private:
	const int WIDTHTEXTURE = 360 / 2;
	const int HEIGHTTEXTURE = 1755 / 9;

// �� ����
private:
	NAME m_name;
	Vector2 m_pos;
	bool isHover = false;

// ȸ��
private:
	Vector2 center;
	float SPEED = 0.3f;
	const float DISTANCE = 400.f;
	float m_initalAngle;
	float m_angle;

// Hover �� �̸�
private:
	NAME _hoverName = NAME::END;

// �� ����
private:
	wstring info[15][3];

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC hdc) override;
};


