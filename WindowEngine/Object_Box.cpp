#include "pch.h"
#include "Object_Box.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Animator.h"
#include "Animation.h"
#include "BoxCollider.h"

Object_Box::Object_Box()
{
	// Texture 로딩
	Texture* tex = ResMgr::GetInst()->LoadTexture(L"Player", L"\\Texture\\PlayerAnimation.bmp");
	m_tex = tex;

	// Animator 생성
	CreateAnimator();

	// Animation 생성
	GetAnimator()->CreateAnimation(L"Idle", tex, Vector2(0.f, 0.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"Left", tex, Vector2(0.f, 64.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	GetAnimator()->CreateAnimation(L"Top" , tex, Vector2(0.f, 128.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 6);
	GetAnimator()->CreateAnimation(L"RIGHT", tex, Vector2(0.f, 192.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	GetAnimator()->CreateAnimation(L"Down", tex, Vector2(0.f, 256.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	
	// offset 값 초기화
	// m_mapAnim을 iter로 접근해서 offset을 한번에 초기화 할 수 있지 않을까?
	// 그냥 CreateAnimation 단계에서 offset 초기화해도 되지 않나?
	Animation* anim = GetAnimator()->FindAnimation(L"Left");
	for(int i = 0; i < anim->GetMaxFrame(); ++i)
	anim->GetFrame(i).offset = Vector2(0.f, -20.f); // i프레임의 y값만 위로 20f 올리기

	// Start Animation 재생
	GetAnimator()->Play(L"Left", true);

	CreateCollider();

	GetBoxCollider()->SetScale(Vector2(25.f, 50.f));
}

Object_Box::~Object_Box()
{
}

void Object_Box::Update()
{
	// 부모클래스의 멤버함수또한 함수를 통해서 접근한다.
	Vector2 pos = GetPos();

	if (KeyMgr::GetInst()->GetKeyState(KEY::A) == KEY_STATE::HOLD)
	{
		pos.x -= 700.f * (float)DT;
	}

	if (KeyMgr::GetInst()->GetKeyState(KEY::D) == KEY_STATE::HOLD)
	{
		pos.x += 700.f * (float)DT;
	}

	if (KeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::HOLD)
	{
		pos.y -= 700.f * (float)DT;
	}

	if (KeyMgr::GetInst()->GetKeyState(KEY::S) == KEY_STATE::HOLD)
	{
		pos.y += 700.f * (float)DT;
	}

	// 부모클래스의 멤버함수를 함수를 통해서 값을 바꾼다.
	SetPos(pos);

	GetAnimator()->Update();
}

void Object_Box::Render(HDC _dc)
{
	ComponentRender(_dc);


	// 랜더링

}
