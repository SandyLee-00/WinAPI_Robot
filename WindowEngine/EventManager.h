#pragma once

class Object;

struct Event
{
	/// 이벤트 종류
	EVENT_TYPE type;

	/// lParam, wParam 은 윈도우 프록시저 함수처럼 인자에서 다양한 값들을 받고 
	/// 이벤트 종류에 따라서 인자을 캐스팅해서 사용하는 용도이다.
	DWORD_PTR  lParam;
	DWORD_PTR  wParam;
};

class SceneManager;
class UIManager;
/// <summary>
/// 이벤트 매니져 
/// 이벤트 지연처리를 담당한다. 이벤트 지연처리를 하는이유?  
/// 게임 프레임워크에서는 다양한 이벤트(ex. 오브젝트 추가 삭제, ai 변경)등이 발생하는데 이러한 시점을 동일한 시점에서 발생시키켜서
/// 프레임 제어를 하고 이벤트들을 한 시점에서 처리하기 위해서이다.
/// 이제부터는 이벤트 매니져 인스턴스를 가져와서 이벤트들을 관리한다.
/// </summary>

class EventManager
{
private:
	EventManager();
	~EventManager();
	
	friend class GameProcess;

private:
	/// 멤버 변수
	vector<Event> m_event;
	vector<Object*> m_deadObj;

public:
	/// 멤버 함수
	void Initialize();
	void Update(GameProcess* gmaeProcess);
	void CreateObject(Object* _obj, OBJECT_TYPE _type);
	void DeleteObject(Object* _obj);
	void ChangeScene(SCENE_TYPE _type);

private:

	/// 외부에서는 필요하지 않는 함수이므로 private 선언
	void ExecuteEvent(const Event& _event, GameProcess* gmaeProcess);
	void AddEvent(const Event& _event) { m_event.push_back(_event); }
};

