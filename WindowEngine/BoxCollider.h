#pragma once

class Object;
class CollisionInfo;
class EventManager;
class KeyManager;

class BoxCollider
{
private:
	static UINT g_iNextID;

	Object* m_owner;
	Vector2 m_offsetPos;
	Vector2 m_finalPos;
	Vector2 m_scale;

	UINT m_collisionCount; //충돌중인 다른 충돌체 갯수
	UINT m_ID;

	bool m_isActive;



// Getter / Setter
public:
	void SetOffsetPos(Vector2 _pos) { m_offsetPos = _pos; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }

	Vector2 GetOffsetPos() { return m_offsetPos; }
	Vector2 GetScale() { return m_scale; }
	Vector2 GetFinalPos() { return m_finalPos; }
	Object* GetOwner() { return m_owner; }
	UINT GetID() { return m_ID; }

// Game Update
public:
	void FinalUpdate();
	void Render(HDC _dc);

// Collision
public:
	void OnCollision(BoxCollider* _other,GameProcess* gameProcess);
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess);
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess);

	BoxCollider& operator=(const BoxCollider& _origin) = delete;

// Constructor / Destructor
public:
	BoxCollider();
	BoxCollider(const BoxCollider& _origin);
	~BoxCollider();

	friend class Object;
};

