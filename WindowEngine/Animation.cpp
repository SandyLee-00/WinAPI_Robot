#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Object.h"
#include "Texture.h"
#include "TimeManager.h"

Animation::Animation()
	: m_animator(nullptr)
	, m_texture(nullptr)
	, m_strName{}
	, m_frame{}
	, m_curFrame(0)
	, m_accTime(0.f)
	, m_finish(false)
{
}

Animation::~Animation()
{
}

void Animation::Update(double _deltaTime)
{
	if (m_finish)
		return;
	// 시간에 따라 프레임을 증가시켜줘야함
	m_accTime += (float)_deltaTime;
	if (m_accTime > m_frame[m_curFrame].duration)
	{
		++m_curFrame;

		if (m_frame.size() <= m_curFrame)
		{
			m_curFrame = -1;
			m_finish = true;
			m_accTime = 0.f;
			return;
		}

		m_accTime = m_accTime - m_frame[m_curFrame].duration;
	}

}

void Animation::Render(HDC _dc)
{
	if (m_finish)
		return;

	Object* obj = m_animator->GetObj();
	Vector2 pos = obj->GetPos();
	pos += m_frame[m_curFrame].offset;

	TransparentBlt(_dc
		, (int)(pos.x - m_frame[m_curFrame].slice.x / 2.f)
		, (int)(pos.y - m_frame[m_curFrame].slice.y / 2.f)
		, (int)(m_frame[m_curFrame].slice.x)
		, (int)(m_frame[m_curFrame].slice.y)
		, m_texture->GetDC()
		, (int)(m_frame[m_curFrame].lt.x)
		, (int)(m_frame[m_curFrame].lt.y)
		, (int)(m_frame[m_curFrame].slice.x)
		, (int)(m_frame[m_curFrame].slice.y)
		, RGB(255, 0, 255)
	);
}

void Animation::Create(Texture* _tex, Vector2 _lt, Vector2 _sliceSize, Vector2 _step, float _duration, UINT _frameCount)
{
	m_texture = _tex;

	AnimFrame frame = {};
	for (UINT i = 0; i < _frameCount; ++i)
	{
		frame.duration = _duration;
		frame.slice = _sliceSize;
		frame.lt = _lt + _step * (float)i;

		m_frame.push_back(frame);
	}
}

