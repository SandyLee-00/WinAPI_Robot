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
	Texture*		   m_texture;		// Animation �� ����ϴ� �ؽ���
	vector<AnimFrame>  m_frame;			// ��� ������ ����
	int				   m_curFrame;		// ���� ������
	float			   m_accTime;		// �ð� ����
	bool			   m_finish;		// ��� ���� ���� ����

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

