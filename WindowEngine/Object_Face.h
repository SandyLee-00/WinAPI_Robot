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
얼굴 텍스쳐 각 사람에 맞게 그린다
각자 정보 각 사람에 맞게 세팅한다
마우스 호버되면 정보 띄운다
마우스 호버되면 웃는 얼굴로 바꾼다

*/

public:
    Object_Face(GameProcess* _gameProcess, NAME _name);
	~Object_Face();

// 공통 기본 정보
private:
	const int WIDTHTEXTURE = 360 / 2;
	const int HEIGHTTEXTURE = 1755 / 9;

// 내 정보
private:
	NAME m_name;
	Vector2 m_pos;
	bool isHover = false;

// 회전
private:
	Vector2 center;
	float SPEED = 0.3f;
	const float DISTANCE = 400.f;
	float m_initalAngle;
	float m_angle;

// Hover 된 이름
private:
	NAME _hoverName = NAME::END;

// 내 정보
private:
	wstring info[15][3];

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC hdc) override;
};


