#include "pch.h"
#include "Animator.h"
#include "Animation.h"
#include "Object.h"

Animator::Animator()
	: m_owner(nullptr)
	, m_curAnim(nullptr)
	, m_mapAnim{}
	, m_repeat(false)
{
}

Animator::~Animator()
{
	Safe_Delete_Map(m_mapAnim);
}


void Animator::CreateAnimation(const wstring& _strName, Texture* _tex, Vector2 _lt, Vector2 _sliceSize, Vector2 _step, float _duration, UINT _frameCount)
{

	Animation* anim = FindAnimation(_strName);
	assert(nullptr == anim);

	anim = new Animation;
	anim->SetName(_strName);
	anim->m_animator = this;
	anim->Create(_tex, _lt, _sliceSize, _step, _duration, _frameCount);

	m_mapAnim.insert(make_pair(_strName, anim));

}


Animation* Animator::FindAnimation(const wstring& _strName)
{
	map<wstring, Animation*>::iterator iter = m_mapAnim.find(_strName);
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void Animator::Play(const wstring& _strName, bool _repeat)
{
	m_curAnim = FindAnimation(_strName);
	m_repeat = _repeat;
}

void Animator::FinalUpdate(double _deltaTime)
{
	
	if (nullptr != m_curAnim)
	{
		m_curAnim->Update(_deltaTime);
		if (m_repeat && m_curAnim->IsFinish())
		{
			m_curAnim->SetFrame(0);
		}

		if (!m_repeat && m_curAnim->IsFinish())
		{
//			m_curAnim->SetFrame(0);
			m_curAnim = nullptr;
		}
	}
}

void Animator::Render(HDC _dc)
{
	if (nullptr != m_curAnim)
	{
		m_curAnim->Render(_dc);
	}
}


