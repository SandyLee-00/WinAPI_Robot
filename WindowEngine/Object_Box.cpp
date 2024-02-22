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
	// Texture �ε�
	Texture* tex = ResMgr::GetInst()->LoadTexture(L"Player", L"\\Texture\\PlayerAnimation.bmp");
	m_tex = tex;

	// Animator ����
	CreateAnimator();

	// Animation ����
	GetAnimator()->CreateAnimation(L"Idle", tex, Vector2(0.f, 0.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"Left", tex, Vector2(0.f, 64.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	GetAnimator()->CreateAnimation(L"Top" , tex, Vector2(0.f, 128.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 6);
	GetAnimator()->CreateAnimation(L"RIGHT", tex, Vector2(0.f, 192.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	GetAnimator()->CreateAnimation(L"Down", tex, Vector2(0.f, 256.f), Vector2(64.f, 64.f), Vector2(64.f, 0.f), 0.1f, 4);
	
	// offset �� �ʱ�ȭ
	// m_mapAnim�� iter�� �����ؼ� offset�� �ѹ��� �ʱ�ȭ �� �� ���� ������?
	// �׳� CreateAnimation �ܰ迡�� offset �ʱ�ȭ�ص� ���� �ʳ�?
	Animation* anim = GetAnimator()->FindAnimation(L"Left");
	for(int i = 0; i < anim->GetMaxFrame(); ++i)
	anim->GetFrame(i).offset = Vector2(0.f, -20.f); // i�������� y���� ���� 20f �ø���

	// Start Animation ���
	GetAnimator()->Play(L"Left", true);

	CreateCollider();

	GetBoxCollider()->SetScale(Vector2(25.f, 50.f));
}

Object_Box::~Object_Box()
{
}

void Object_Box::Update()
{
	// �θ�Ŭ������ ����Լ����� �Լ��� ���ؼ� �����Ѵ�.
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

	// �θ�Ŭ������ ����Լ��� �Լ��� ���ؼ� ���� �ٲ۴�.
	SetPos(pos);

	GetAnimator()->Update();
}

void Object_Box::Render(HDC _dc)
{
	ComponentRender(_dc);


	// ������

}
