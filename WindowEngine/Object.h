#pragma once

class Animator;
class Texture;
class BoxCollider;
class Object_Food;
class Object_Pizza;
class Object_Topping;
class GameProcess;
class EventManager;
class KeyManager;

enum class TOPPING;

class Object
{

private:
	wstring m_name;

	Vector2 m_pos;
	Vector2 m_scale;

	bool m_alive;
 
	BoxCollider* m_boxCollider;
	Animator* m_animator;

protected:
	Texture* m_tex; // ������Ʈ �ؽ�ó
	Vector2 m_lookdir; // ������Ʈ ����

	Object_Pizza* m_pizza; // ������Ʈ�� ����ִ� ����
	Object_Topping* m_topping; // ������Ʈ�� ����ִ� ����

// TODO : ���� �������� ��Ƶα�
// TODO : SetScale ó�� ���� ���ϸ� ������Ʈ�� �������� �ʴ� �͵� �����ڷ� �س���
// Getter/Setter
public:
	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	Vector2 GetPos() { return m_pos; }
	Vector2 GetScale() { return m_scale; }
	Texture* GetTexture() { return m_tex; }
	Vector2 GetLookDir() { return m_lookdir; }

	void SetTexture(Texture* _tex) { m_tex = _tex; }
	void SetName(const wstring& _name) { m_name = _name; }
	const wstring& GetName() { return m_name; }

	void SetTopping(Object_Topping* _topping) { m_topping = _topping; }
	void SetPizza(Object_Pizza* _pizza) { m_pizza = _pizza; }
	Object_Pizza* GetPizza() { return m_pizza; }
	Object_Topping* GetTopping() { return m_topping; }

public:
	void DeleteFood(EventManager* _eventManager);

public:
	Animator* CreateAnimator();
	void CreateCollider();
	virtual Animator* GetAnimator() { return m_animator; }
	BoxCollider* GetBoxCollider() { return m_boxCollider; }
	
	bool IsDead() { return !m_alive; }
	void SetDead() { m_alive = false; }

public:
	virtual void Start() {};
	virtual void Update(GameProcess* _gameProcess) = 0;
	virtual void Finalupdate(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager);
	virtual void Render(HDC _dc);

	void ComponentRender(HDC _dc);

	virtual void OnCollision(BoxCollider* _other, GameProcess* _gameProcess){}
	virtual void OnCollisionEnter(BoxCollider* _other,  GameProcess* _gameProcess) {}
	virtual void OnCollisionExit(BoxCollider* _other, GameProcess* _gameProcess) {}


public:
	/// ������
	Object();
	Object(const Object& _origin);

	/// �Ҹ���
	virtual ~Object();

};

