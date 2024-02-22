#include "pch.h"
#include "Object_OrderSheetUI.h"
#include "Object_Order.h"

#include "TimeManager.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "SelectGDI.h"
#include "GameProcess.h"
#include "EventManager.h"

#include "Animator.h"
#include "Animation.h"

Object_OrderSheetUI::Object_OrderSheetUI(GameProcess* gameProcess)
	: m_sheetInterval(90.f)
	, m_animator(nullptr)
{
	SetPos(Vector2(1560.f, 180.f));
	SetScale(Vector2(260.f, 100.f));

	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();

	AnimationInit(gameProcess->GetResourceManager(), pathManager, mainDC);
}

Object_OrderSheetUI::~Object_OrderSheetUI()
{

}

bool Object_OrderSheetUI::AddOrder()
{
	return false;
}

bool Object_OrderSheetUI::ClearOrder()
{
	return false;
}


void Object_OrderSheetUI::AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC)
{
	// Texture 肺爹
	Texture* SuccessTex = _resourceManager->LoadTexture(L"SuccessTex", L"\\Texture\\Animation\\Painting_Effect_2.bmp", _pathManager, _mainDC);
	Texture* FailTex = _resourceManager->LoadTexture(L"FailTex", L"\\Texture\\Animation\\Painting_Effect_1.bmp", _pathManager, _mainDC);

	// Animator 积己
	m_animator = CreateAnimator();

	UINT frameCount = 11;
	float sliceX = 2706.f / frameCount;
	float sliceY = 232.f;
	float duration = 0.1f;
	Vector2 lt_begin = Vector2(0.f, 0.f);
	Vector2 sliceSize = Vector2(sliceX, sliceY);
	Vector2 step = Vector2(sliceX, 0.f);

	// Animation 积己
	GetAnimator()->CreateAnimation(L"OrderSuccess", SuccessTex, lt_begin, sliceSize, step, duration, frameCount);
	GetAnimator()->CreateAnimation(L"OrderFail", FailTex, lt_begin, sliceSize, step, duration, frameCount);
}

void Object_OrderSheetUI::AnimationSetting(wstring _animName, int index)
{
	for (int i = 0; i < GetAnimator()->FindAnimation(_animName)->GetMaxFrame(); ++i)
		GetAnimator()->FindAnimation(_animName)->GetFrame(i).offset
		= Vector2(135.f, 10.f + m_sheetInterval * index);
}

void Object_OrderSheetUI::Render(HDC _dc)
{

	Vector2 pos = GetPos();
	Vector2 scale = GetScale();

	SelectGDI font(_dc, FONT_TYPE::DOSPILGI_FONT, RGB(89, 33, 33));

	for (int i = 0; i < (int)m_orderList->size(); ++i)
	{
		if ((*m_orderList)[i]->orderTime >= 40) SetTextColor(_dc, RGB(192, 0, 0));
		else if ((*m_orderList)[i]->orderTime >= 25) SetTextColor(_dc, RGB(255, 192, 0));
		else SetTextColor(_dc, RGB(89, 33, 33));

		RECT rc = { (long)pos.x
			,(long)pos.y + (long)(m_sheetInterval * i)
			,(long)(pos.x + scale.x)
			,(long)(pos.y + (long)(m_sheetInterval * i) + scale.y) };

		DrawTextW(_dc, (*m_orderList)[i]->name.c_str(), -1, &rc, DT_CENTER);

	}

	ComponentRender(_dc);
}

void Object_OrderSheetUI::Update(GameProcess* _gameProcess)
{
}
