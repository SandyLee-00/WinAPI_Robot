#pragma once

#include "Object.h"

enum class BossState;
enum class LOOKDIR;

/// <summary>
/// ���� : �÷��̾ �����ϴ� ����� AI
/// ������ : ���ؿ�
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

// TODO : Update / Getter/Setter / �з����� �����ؼ� ��Ƶα� -- SANDY
public:
	Animator* GetAnimator() { return m_animator; }
	void SetSpeed(float _speed) { m_speed = _speed; }

public:
	void AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC);

private:
	void UpdateAnimation();

	// -- Animation -- SANDY

// TODO : public -> private ���� �ٲٱ� ��ü���� -- SANDY
public:
	void ChangeState(BossState _newState);
	bool StateIs(BossState _state);

	void FindPlayer(SceneManager* sceneManager);

	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;

	void DebugRender(HDC _dc);

// TODO : Update___ �� �ٲٱ� / private :
// Update�ȿ� �����ִٴ� ���� ������� �Ѵ� / �ٸ� ��ü�� �� �� �ֵ��� ���� �ȵȴ� Update �ȿ��� �۵��Ѵ� -- SANDY
	void IdleUpdate(TimeManager* timeManager);
	void PatorlUpdate(TimeManager* timeManager, BossManager* bossManager);

	void OnCollision(BoxCollider* _other, GameProcess* _gameProcess) override;

	UINT GetRandomIdleTime(); // �÷��̾� Idle ��� �ð�
	UINT GetRandomDirValue(); // Idle ���¿� �θ��� �Ÿ��� ���� ��ȯ�� ���� ū ���� ����

	void SoundControl(SoundManager* soundManager); // Walk ���� ��Ʈ��

	void ChangeIdleDir(TimeManager* timeManager); // Idle ���¿��� �θ��� �θ���
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