#pragma once
#include "Scene.h"

class Object_Player;
class Object_TextureBarUI;
class Object_Window;
class Object_Boss;
class Object_TimeUI;
class Object_Order;

class Scene_Stage01 :
    public Scene
{
private:
	Object_Player* m_player;
	Object_TextureBarUI* m_trustBar;
	Object_Boss* m_boss;
	Object_Window* m_rightWindow;
	Object_TimeUI* m_timeUI;
	Object_Order* m_order;

	bool m_isGameOverBGM;

	void RegisterOrder(Object_Order* _order) { m_order = _order; }
	void RegisterTimeUI(Object_TimeUI* _timeUI) { m_timeUI = _timeUI; }
	void RegisterPlayer(Object_Player* _player) { m_player = _player; }
	void RegisterTrustBar(Object_TextureBarUI* _trustBar) { m_trustBar = _trustBar; }
	void RegisterRightWindow(Object_Window* _rightWindow) { m_rightWindow = _rightWindow; }
public:
	Object_Player* GetPlayer() { return m_player; }

public:
    Scene_Stage01();
    ~Scene_Stage01() override;

    // 신뢰도 수치를 조정하는 함수
	void AddTrust(float _value);
	void Update(GameProcess* gameProcess) override;

	//// 사장님
	void PlayWalkingWindow(SoundManager* soundManager);
	void PlayUpWindow(SoundManager* soundManager);
	void RegisterBoss(Object_Boss* _boss) { m_boss = _boss; }
	void DisappearBoss() { m_boss = nullptr; }

	void Render(HDC _dc) override;

    void Exit() override;
    void Enter(GameProcess* _gameProcess) override;
};

