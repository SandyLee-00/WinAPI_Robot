#pragma once

class Object_UI;

class KeyManager;
class SceneManager;

class UIManager
{
	UIManager();
	~UIManager();

	friend class GameProcess;

private:
	Object_UI* m_focusedUI;

public:
	void Update(GameProcess* gameProcess);
	void SetFocusedUI(Object_UI* _UI, SceneManager* _sceneManager);
	void Initialize();
private:
	Object_UI* GetFocusedUI(KeyManager* _keyManager, SceneManager* _sceneManager);
	Object_UI* GetTargetedUI(Object_UI* _parentUI, KeyManager* _keyManager); // 부모 UI 내에서 실제로 타겟팅된 UI를 찾아서 반환한다.

};

