#pragma once
#include "Object.h"

enum class PlayerState;
enum class LOOKDIR;

class Object_Food;
class Object_TextureBarUI;
class Animator;

class Object_Player :
    public Object
{
private:
    float m_speed;
    Vector2 m_pos;
    Vector2 m_moveDir;

    PlayerState m_curState;
    PlayerState m_preState;

    Object_TextureBarUI* m_healthBar;
    Object_TextureBarUI* m_trustBar;

    float m_health;

    double m_decreaseHealthTime;
    double m_increaseHealthTime;

    double m_deltaTime;
    double m_popMessageTime;
    wstring m_popMessage;
    bool m_popMessageMode;
    double m_moveY;

	Animator* m_animator;

    bool m_isWalk;

// Getter/Setter
public:
    Vector2 GetDir() { return m_moveDir; }
    PlayerState GetPlayerState() { return m_curState; }
    float GetPlayerHealth() { return m_health; }

    Animator* GetAnimator() { return m_animator; }

    void SetSpeed(float _speed) { m_speed = _speed; }
    void SetHeartBar(Object_TextureBarUI* _healthBar) { m_healthBar = _healthBar; }

public:
	void AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager,HDC _mainDC);
	void PopMessageDraw(HDC _dc);
    void SetPopMessage(wstring _message) { m_popMessage = _message; m_popMessageMode = true; }

    void UpdateMove(KeyManager* _keyManager, double _deltaTime);
    void UpdateState(KeyManager* _keyManager);

    void ChangeState(PlayerState _newState);
    bool StateIs(PlayerState _state);

    void IncreaseHealth(double _deltaTime);
    void DecreaseHealth(double _deltaTime);

public:
    void Update(GameProcess* _gameProcess) override;
    void Render(HDC _dc) override;
    void DebugRender(HDC _dc);

public:
    Object_Player(GameProcess* gameProcess);
    ~Object_Player();

};

enum class PlayerState
{
    IDLE,
    MOVE,
    REST_BEFORE,
    REST,
    REST_AFTER,
    DEAD
};