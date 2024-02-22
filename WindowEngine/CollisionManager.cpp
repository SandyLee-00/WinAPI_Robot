#include "pch.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Object.h"
#include "BoxCollider.h"
// #include <windef.h>

CollisionManager::CollisionManager():m_arrCheck{}
{

}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Initialize()
{
}

void CollisionManager::Update(GameProcess* gameProcess)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		for (UINT j = i; j < (UINT)OBJECT_TYPE::END; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				// �浹üũ�� �ʿ��� �����̸� �浹üũ
				CollisionGroupUpdate((OBJECT_TYPE)i, (OBJECT_TYPE)j,gameProcess);
			}
		}
	}
}


void CollisionManager::CheckObjectType(OBJECT_TYPE _left, OBJECT_TYPE _right)
{
	m_arrCheck[(UINT)_left] |= (1 << (UINT)_right);
	m_arrCheck[(UINT)_right] |= (1 << (UINT)_left);
}

// ������ �����ϴ� �� �׷��� ��ü �迭 ���鼭 �ϳ��� �浹üũ
void CollisionManager::CollisionGroupUpdate(OBJECT_TYPE _left, OBJECT_TYPE _right, GameProcess* gameProcess)
{
	SceneManager* sceneManager =gameProcess->GetSceneManager();


	Scene* curScene = sceneManager->GetCurScene();
	if (nullptr == curScene)
		return;

	const vector<Object*>& vecLeft = curScene->GetGroupObject(_left);
	const vector<Object*>& vecRight = curScene->GetGroupObject(_right);
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		for (size_t j = 0; j < vecRight.size(); j++)
		{
			// �浹ü�� ���ų�, �ڱ� �ڽŰ� �浹�� ��� ��ŵ 
			if (nullptr == vecLeft[i]->GetBoxCollider() || nullptr == vecRight[j]->GetBoxCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			BoxCollider* leftCollider = vecLeft[i]->GetBoxCollider();
			BoxCollider* rightCollider = vecRight[j]->GetBoxCollider();

			// �� �浹ü ���̵� ���� left + right
			COLLIDER_ID colliderID = {};
			colliderID.left_ID = leftCollider->GetID();
			colliderID.right_ID = rightCollider->GetID();

			iter = m_mapCollisionInfo.find(colliderID.ID);

			// �浹ID�� ������ ���� ����
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(colliderID.ID, false));
				iter = m_mapCollisionInfo.find(colliderID.ID);
			}

			// �浹���� ���
			if (IsCollision(leftCollider, rightCollider) == true)
			{
				// ���� �����ӿ��� �浹�� ���� && ������ ������ ��� : OnCollisionExit(other)
				if (iter->second && (vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollisionExit(rightCollider, gameProcess);
					rightCollider->OnCollisionExit(leftCollider, gameProcess);
					iter->second = false;
					continue;
				}

				// ���� �����ӿ��� �浹�� ���� && �������� ���� ��� : OnCollision(other)
				else if (iter->second && !(vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollision(rightCollider, gameProcess);
					rightCollider->OnCollision(leftCollider, gameProcess);
					iter->second = true;
					continue;

				}

				// ���� �����ӿ��� �浹 �� �� ���� && ������ ������ ��� : X 
				else if (!iter->second && (vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					continue;
				}

				// ���� �����ӿ��� �浹 �� �� ���� && �������� ���� ��� : OnCollisionEnter(other)
				else if (!iter->second && !(vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollisionEnter(rightCollider, gameProcess);
					rightCollider->OnCollisionEnter(leftCollider, gameProcess);
					iter->second = true;
					continue;
				}

			}
			// �浹 �� �� ��� && ���� �����ӿ��� �浹�� ���� : OnCollisionExit(other)
			else if (IsCollision(leftCollider, rightCollider) == false && iter->second)
			{
				leftCollider->OnCollisionExit(rightCollider, gameProcess);
				rightCollider->OnCollisionExit(leftCollider, gameProcess);
				iter->second = false;
			}
			else
			{
				continue;
			}

		}
	}
}

// AABB �浹üũ : minXY maxXY �̿��ؼ� �浹üũ
bool CollisionManager::IsCollision(BoxCollider* _leftCollider, BoxCollider* _rightCollider)
{
	Vector2 leftPos = _leftCollider->GetFinalPos();
	Vector2 rightPos = _rightCollider->GetFinalPos();

	Vector2 leftScale = _leftCollider->GetScale();
	Vector2 rightScale = _rightCollider->GetScale();

	Vector2 leftMin = leftPos - (leftScale * 0.5f);
	Vector2 leftMax = leftPos + (leftScale * 0.5f);
	Vector2 rightMin = rightPos - (rightScale * 0.5f);
	Vector2 rightMax = rightPos + (rightScale * 0.5f);

	if ((leftMin.x <= rightMax.x && leftMax.x >= rightMin.x) &&
		(leftMin.y <= rightMax.y && leftMax.y >= rightMin.y))
	{
		return true;
	}

	return false;
}

// OnCollision -> �о�� : X, Y �� �� ū ������ �о��
// ���� ������Ʈ�� ������ ������Ʈ�� �о�� ��
void CollisionManager::PushBox(BoxCollider* _left, BoxCollider* _right)
{
	// Radius ���ϱ� : Scale�� ���ݾ� ���ϱ�
	float radX = _left->GetScale().x * 0.5f + _right->GetScale().x * 0.5f;
	float radY = _left->GetScale().y * 0.5f + _right->GetScale().y * 0.5f;

	// Distance ���ϱ� : �����ִ� ������Ʈ�� ��ġ ���� ���ϱ�
	Vector2 leftObjPos = _left->GetOwner()->GetPos();
	Vector2 rightObjPos = _right->GetOwner()->GetPos();

	Vector2 leftFinalPos = _left->GetFinalPos();
	Vector2 RightFinalPos = _right->GetFinalPos();

	float distX = abs(leftFinalPos.x - RightFinalPos.x);
	float distY = abs(leftFinalPos.y - RightFinalPos.y);

	// �о ��
	float pushX = radX - distX;
	float pushY = radY - distY;

	//�� - �� ��ġ�� �� : pushX, pushY ������

	// X�� �б�
	if (pushX < pushY)
	{
		if (leftFinalPos.x > RightFinalPos.x)
		{
			pushX *= -1.f;
		}
		rightObjPos.x += pushX;
		_right->GetOwner()->SetPos(rightObjPos);
		_right->FinalUpdate();
	}
	// Y�� �б�
	else
	{
		if (leftFinalPos.y > RightFinalPos.y)
		{
			pushY *= -1.f;
		}

		rightObjPos.y += pushY;
		_right->GetOwner()->SetPos(rightObjPos);
		_right->FinalUpdate();
	}
}




