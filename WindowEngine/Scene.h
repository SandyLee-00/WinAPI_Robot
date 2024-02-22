#pragma once

// ���漱�� ��� ��� ������ ������ �����ϰ� ���漱���� �Ѵ�.
class Object;
class GameProcess;
class ResourceManager;
class CollisionManager;

class Scene
{
protected:
	// ������Ʈ Ŭ������ ��ӹ޴� Ŭ������ Object*�� ����ų�� �����Ƿ�
	// ������Ʈ�� �߰��ϴ� �����̱� ������ 32���� ������ ���� vecrtor ��ü�� �޸� ������ ���� �������� �ʴ´�.
	/// ��� ����
	vector<Object*> m_obj[(UINT)OBJECT_TYPE::END];
	wstring         m_name; // Scene �̸�

	// UI �Ͻ�������ư�� �϶� ���� �Ͻ������� ���� �������
	bool            m_pauseGame;

public:
	virtual void Start();
	virtual void Update(GameProcess* gameProcess);
	virtual void FinalUpdate(TimeManager* _timeManager,EventManager* _eventManager, KeyManager* _keyManager);
	virtual void Render(HDC _dc);

	virtual void Exit() = 0; // �ش� Scene�� ���Խ� ȣ��
	virtual void Enter(GameProcess* _gameProcess) = 0; // �ش� Scene�� Ż�� �� ȣ��

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

