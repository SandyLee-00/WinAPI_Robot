#pragma once

#include "Object.h"

enum class BossState;
enum class LOOKDIR;

/// <summary>
/// 역할 : 플레이어를 감시하는 사장님 AI
/// 담장자 : 공준열
/// </summary>
class Object_Boss :
	public Object
{
public:
	Object_Boss(GameProcess* _gameProcess);
	~Object_Boss() override;

private:
	float m_speed;

	Vector2 m_moveDirection;
	Vector2 m_lookDirection;

	BossState m_currentState;
	BossState m_previousState;

	double m_idleTime;
	float m_attachedDistance;
	float m_detectedDistance;

	// -- Animation -- SANDY
	Animator* m_animator;
	bool m_isDeteted;
	bool m_isCTRPressed;

	bool m_isWalkSound;

// TODO : Update / Getter/Setter / 분류별로 정리해서 모아두기 -- SANDY
public:
	Animator* GetAnimator() { return m_animator; }
	void SetSpeed(float _speed) { m_speed = _speed; }

public:
	void AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC);

private:
	void UpdateAnimation();

	// -- Animation -- SANDY

// TODO : public -> private 으로 바꾸기 객체지향 -- SANDY
public:
	void ChangeState(BossState _newState);
	bool StateIs(BossState _state);

	void FindPlayer(SceneManager* sceneManager);

	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;

	void DebugRender(HDC _dc);

// TODO : Update___ 로 바꾸기 / private :
// Update안에 속해있다는 것을 보여줘야 한다 / 다른 객체가 쓸 수 있도록 열면 안된다 Update 안에서 작동한다 -- SANDY
	void IdleUpdate(TimeManager* timeManager);
	void PatorlUpdate(TimeManager* timeManager, BossManager* bossManager);

	void OnCollision(BoxCollider* _other, GameProcess* _gameProcess) override;

	UINT GetRandomIdleTime(); // 플레이어 Idle 대기 시간
	UINT GetRandomDirValue(); // Idle 상태에 두리번 거리는 방향 전환을 위해 큰 수를 받음

	void SoundControl(SoundManager* soundManager); // Walk 사운드 컨트롤

	void ChangeIdleDir(TimeManager* timeManager); // Idle 상태에서 두리번 두리번
};

// TODO : BOSSSTATE -- SANDY
enum class BossState
{
	IDLE,
	PATROL,
	TRACE,
	FIND,
	DONE,
};