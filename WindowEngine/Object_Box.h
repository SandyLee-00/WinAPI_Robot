#pragma once
#include "Object.h"

class Texture;

class Object_Box
	:public Object
{
public:
	// ������
	Object_Box();

	// �Ҹ���
	~Object_Box();

	void Update() override;
	void Render(HDC _dc) override;

private:
};

