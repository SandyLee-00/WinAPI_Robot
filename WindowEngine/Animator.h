#pragma once

class Object;
class Animation;
class Texture;

class Animator
{
private:
	map<wstring, Animation*> m_mapAnim;		// 모든 Animation
	Animation*				 m_curAnim;	    // 현재 재생중인 Animation
	Object*					 m_owner;		// Animator 소유 오브젝트
	bool					 m_repeat;		// 반복재생 여부

public:
	Object* GetObj() { return m_owner; }
	Animation* GetCurAnim() { return m_curAnim; }

public:
	void CreateAnimation(const wstring& _strName, Texture* _tex, 
						 Vector2 _lt, Vector2 _sliceSize, Vector2 _step, float _duration, UINT _frameCount);
	Animation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName, bool _repeat);

	void FinalUpdate(double _deltaTime);
	void Render(HDC _dc);
	
public:
	Animator();

	~Animator();

	friend class Object;
};

