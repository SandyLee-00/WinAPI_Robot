#pragma once

// 전방선언 헤더 헤더 끼리의 참조를 방지하고 전방선언을 한다.
class Object;
class GameProcess;
class ResourceManager;
class CollisionManager;

class Scene
{
protected:
	// 오브젝트 클래스를 상속받는 클래스는 Object*로 가리킬수 있으므로
	// 오브젝트는 추가하는 형식이기 때문에 32개의 공간을 만들어도 vecrtor 자체는 메모리 공간을 별로 차지하지 않는다.
	/// 멤버 변수
	vector<Object*> m_obj[(UINT)OBJECT_TYPE::END];
	wstring         m_name; // Scene 이름

	// UI 일시정지버튼을 일때 게임 일시정지를 위함 멤버변수
	bool            m_pauseGame;

public:
	virtual void Start();
	virtual void Update(GameProcess* gameProcess);
	virtual void FinalUpdate(TimeManager* _timeManager,EventManager* _eventManager, KeyManager* _keyManager);
	virtual void Render(HDC _dc);

	virtual void Exit() = 0; // 해당 Scene에 진입시 호출
	virtual void Enter(GameProcess* _gameProcess) = 0; // 해당 Scene을 탈출 시 호출

public:
	void SetName(const wstring& _strName) { m_name = _strName; }
	const wstring& GetName() { return m_name; }

	const vector<Object*>& GetGroupObject(OBJECT_TYPE _type) { return m_obj[(UINT)_type]; }
	vector<Object*>& GetGroupUI() { return m_obj[(UINT)OBJECT_TYPE::UI];}

	void AddObject(Object* _obj, OBJECT_TYPE _type) { m_obj[(UINT)_type].push_back(_obj); }
	void DeleteGroup(OBJECT_TYPE _target);
	void DeleteAll();

	bool IsPauseGame() { return m_pauseGame; }

	void PauseGame() { m_pauseGame = true; }
	void PlayGame() { m_pauseGame = false; }

public:
	Scene();
	virtual~Scene();

};

