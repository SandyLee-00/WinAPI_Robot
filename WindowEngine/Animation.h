#pragma once

class Animator;
class Texture;

struct AnimFrame
{
	Vector2 lt;
	Vector2 slice;
	Vector2 offset;
	float duration;
};

class Animation
{
private:
	wstring			   m_strName;
	Animator*		   m_animator;
	Texture*		   m_texture;		// Animation 이 사용하는 텍스쳐
	vector<AnimFrame>  m_frame;			// 모든 프레임 정보
	int				   m_curFrame;		// 현재 프레임
	float			   m_accTime;		// 시간 누적
	bool			   m_finish;		// 재생 끝에 도달 여부

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish(){ return m_finish; }
	void SetFrame(int _frameIndex)
	{
		m_finish = false;
		m_curFrame = _frameIndex;
		m_accTime = 0.f;
	}

	AnimFrame& GetFrame(int _index) { return m_frame[_index]; }
	int GetMaxFrame() { return (int)(m_frame.size()); }
	int GetCurFrame() { return m_curFrame; }

private:
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void Update(double _deltaTime);
	void Render(HDC _dc);
	void Create(Texture* _tex, Vector2 _lt, Vector2 _sliceSize, Vector2 _step, float _duration, UINT _frameCount);

public:
	Animation();
	~Animation();

	friend class Animator;
};

