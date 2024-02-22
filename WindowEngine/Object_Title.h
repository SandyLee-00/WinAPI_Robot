#pragma once
#include "Object_UI.h"

class Object_Title :
	public Object_UI
{
	Texture* m_subTitle;
	Vector2 m_mousePos;

public:
	Object_Title(GameProcess* _gameProcess);
	~Object_Title();

	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

