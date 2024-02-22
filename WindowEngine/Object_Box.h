#pragma once
#include "Object.h"

class Texture;

class Object_Box
	:public Object
{
public:
	// 持失切
	Object_Box();

	// 社瑚切
	~Object_Box();

	void Update() override;
	void Render(HDC _dc) override;

private:
};

