#pragma once

class Scene;
class GameProcess;

class SceneManager
{
	SceneManager();
	~SceneManager();

	friend class GameProcess;

private:
	Scene* m_scene[(UINT)SCENE_TYPE::END]; // ¸ðµç ¾À ¸ñ·Ï
	Scene* m_curScene;                       // ÇöÀç ¾À

public:
	void Initialize(GameProcess* gameProcess);
	void Update(GameProcess* _gameProcess);
	void FinalUpdate(TimeManager* timeManager, EventManager* eventManager, KeyManager* _keyManager);

	void Render(HDC _dc);

	Scene* GetCurScene() { return m_curScene; }

	void ChangeScene(SCENE_TYPE _next, GameProcess* _gameProcess);

};

