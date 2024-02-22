#pragma once

class Object;

struct Event
{
	/// �̺�Ʈ ����
	EVENT_TYPE type;

	/// lParam, wParam �� ������ ���Ͻ��� �Լ�ó�� ���ڿ��� �پ��� ������ �ް� 
	/// �̺�Ʈ ������ ���� ������ ĳ�����ؼ� ����ϴ� �뵵�̴�.
	DWORD_PTR  lParam;
	DWORD_PTR  wParam;
};

class SceneManager;
class UIManager;
/// <summary>
/// �̺�Ʈ �Ŵ��� 
/// �̺�Ʈ ����ó���� ����Ѵ�. �̺�Ʈ ����ó���� �ϴ�����?  
/// ���� �����ӿ�ũ������ �پ��� �̺�Ʈ(ex. ������Ʈ �߰� ����, ai ����)���� �߻��ϴµ� �̷��� ������ ������ �������� �߻���Ű�Ѽ�
/// ������ ��� �ϰ� �̺�Ʈ���� �� �������� ó���ϱ� ���ؼ��̴�.
/// �������ʹ� �̺�Ʈ �Ŵ��� �ν��Ͻ��� �����ͼ� �̺�Ʈ���� �����Ѵ�.
/// </summary>

class EventManager
{
private:
	EventManager();
	~EventManager();
	
	friend class GameProcess;

private:
	/// ��� ����
	vector<Event> m_event;
	vector<Object*> m_deadObj;

public:
	/// ��� �Լ�
	void Initialize();
	void Update(GameProcess* gmaeProcess);
	void CreateObject(Object* _obj, OBJECT_TYPE _type);
	void DeleteObject(Object* _obj);
	void ChangeScene(SCENE_TYPE _type);

private:

	/// �ܺο����� �ʿ����� �ʴ� �Լ��̹Ƿ� private ����
	void ExecuteEvent(const Event& _event, GameProcess* gmaeProcess);
	void AddEvent(const Event& _event) { m_event.push_back(_event); }
};

