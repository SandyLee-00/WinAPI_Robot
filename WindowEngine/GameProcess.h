#pragma once

class Texture;;

class SoundManager;
class CameraManager;
class CollisionManager;
class EventManager;
class KeyManager;
class PathManager;
class ResourceManager;
class TimeManager;
class BossManager;
class SceneManager;
class UIManager;


class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	// 자주 사용하는 GDI
	static HBRUSH m_brush[(UINT)BRUSH_TYPE::END];
	static HPEN   m_pen[(UINT)PEN_TYPE::END];
	static HFONT  m_font[(UINT)FONT_TYPE::END];
private:

	/// 멤버 변수
	HWND  m_hWnd;       // 메인 윈도우 핸들
	POINT m_resolution; // 메인 윈도우 해상도
	HDC   m_hDC;        // 메인 윈도우 Draw DC

	Texture* m_memTex; // 드로우 텍스쳐

	// 매니져
	SoundManager* m_soundManager;
	CameraManager* m_cameraManager;
	CollisionManager* m_collisionManager;
	EventManager* m_eventManager;
	KeyManager* m_keyManager;
	PathManager* m_pathManager;
	ResourceManager* m_resourceManager;
	TimeManager* m_timeManager;
	BossManager* m_bossManager;
	SceneManager* m_sceneManager;
	UIManager* m_UIManager;

public:
	SoundManager* GetSoundManager() { return m_soundManager; }
	CameraManager* GetCameraManager() { return m_cameraManager; }
	CollisionManager* GetCollisionManager() { return m_collisionManager; }
	// TODO : GetEvenetManager -> GetEventManager
	EventManager* GetEvenetManager() { return m_eventManager; }
	KeyManager* GetKeyManager() { return m_keyManager; }
	PathManager* GetPathManager() { return m_pathManager; }
	ResourceManager* GetResourceManager() { return m_resourceManager; }
	TimeManager* GetTimeManager() { return m_timeManager; }
	BossManager* GetBossManager() { return m_bossManager; }
	SceneManager* GetSceneManager() { return m_sceneManager; }
	UIManager* GetUIManager() {return  m_UIManager; }


public:

	/// 멤버 함수

	// 게임 시작시에 호출하는 함수
	int Init(HWND _hWnd, POINT _resolution);

	void Exit();
	// 게임 루프
	void Progress();
	// WindowSize 조정
	void ChangeWindowSize(Vector2 _resolution, bool _menu);
	// 드로우 텍스처에서 출력텍스처로 복사
	void BufferFliping();
	//  드로우 텍스처 지우기
	void BufferClear();

	void LoadFont();

	HBRUSH GetBrush(BRUSH_TYPE _brushType) { return m_brush[(UINT)_brushType]; }
	HPEN GetPen(PEN_TYPE _penType) { return m_pen[(UINT)_penType]; }
	HFONT GetFont(FONT_TYPE _fontType) { return m_font[(UINT)_fontType]; }

private:
	void CreateBrushPen();

public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_resolution; }

};

