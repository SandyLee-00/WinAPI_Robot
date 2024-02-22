#pragma once

class BoxCollider;
class Object;

// 충돌한 정보 저장 : left_ID + right_ID
union COLLIDER_ID
{
	struct
	{
		UINT left_ID;
		UINT right_ID;
	};
	ULONGLONG ID;
};

class SceneManager;
class KeyManager;
class EventManager;

class CollisionManager
{
	CollisionManager();
	~CollisionManager();

	friend class GameProcess;

private:
	map<ULONGLONG, bool> m_mapCollisionInfo;
	UINT m_arrCheck[(UINT)OBJECT_TYPE::END];

public:
	void Initialize();
	void Update(GameProcess* gameProcess);

public:
	void CheckObjectType(OBJECT_TYPE _left, OBJECT_TYPE _right);
	static void PushBox(BoxCollider* _left, BoxCollider* _right);

private:
	void CollisionGroupUpdate(OBJECT_TYPE _left, OBJECT_TYPE _right, GameProcess* gameProcess);
	bool IsCollision(BoxCollider* _left, BoxCollider* _right);
};

