#pragma once
#include "Object.h"
class Object_TrashCan :
    public Object
{
private:
	int trashInteractionCount;
	const int MAXTRASHCANINTERACTIONCOUNT = 3;

	bool isMemoPopUP;
	int randomMemoIndex;
	const int MEMOCOUNT = 9;

	Texture* memoTexture;
	// TODO : 아트 리소스 받으면 값 수정하기

	Animator* m_animator;

public:
	Object_TrashCan(GameProcess* gameProcess);
	~Object_TrashCan();

// Getter / Setter
public:

	int GetTrashInteractionCount() { return trashInteractionCount; }

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;

public:

};

