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
				// 충돌체크가 필요한 조합이면 충돌체크
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

// 씬에서 존재하는 각 그룹의 객체 배열 돌면서 하나씩 충돌체크
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
			// 충돌체가 없거나, 자기 자신과 충돌인 경우 스킵 
			if (nullptr == vecLeft[i]->GetBoxCollider() || nullptr == vecRight[j]->GetBoxCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			BoxCollider* leftCollider = vecLeft[i]->GetBoxCollider();
			BoxCollider* rightCollider = vecRight[j]->GetBoxCollider();

			// 두 충돌체 아이디 생성 left + right
			COLLIDER_ID colliderID = {};
			colliderID.left_ID = leftCollider->GetID();
			colliderID.right_ID = rightCollider->GetID();

			iter = m_mapCollisionInfo.find(colliderID.ID);

			// 충돌ID가 없으면 새로 생성
			if (m_mapCollisionInfo.end() == iter)
			{
				m_mapCollisionInfo.insert(make_pair(colliderID.ID, false));
				iter = m_mapCollisionInfo.find(colliderID.ID);
			}

			// 충돌했을 경우
			if (IsCollision(leftCollider, rightCollider) == true)
			{
				// 이전 프레임에서 충돌한 상태 && 삭제된 상태일 경우 : OnCollisionExit(other)
				if (iter->second && (vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollisionExit(rightCollider, gameProcess);
					rightCollider->OnCollisionExit(leftCollider, gameProcess);
					iter->second = false;
					continue;
				}

				// 이전 프레임에서 충돌한 상태 && 삭제되지 않은 경우 : OnCollision(other)
				else if (iter->second && !(vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollision(rightCollider, gameProcess);
					rightCollider->OnCollision(leftCollider, gameProcess);
					iter->second = true;
					continue;

				}

				// 이전 프레임에서 충돌 안 한 상태 && 삭제된 상태일 경우 : X 
				else if (!iter->second && (vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					continue;
				}

				// 이전 프레임에서 충돌 안 한 상태 && 삭제되지 않은 경우 : OnCollisionEnter(other)
				else if (!iter->second && !(vecLeft[i]->IsDead() || vecRight[j]->IsDead()))
				{
					leftCollider->OnCollisionEnter(rightCollider, gameProcess);
					rightCollider->OnCollisionEnter(leftCollider, gameProcess);
					iter->second = true;
					continue;
				}

			}
			// 충돌 안 한 경우 && 이전 프레임에서 충돌한 상태 : OnCollisionExit(other)
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

// AABB 충돌체크 : minXY maxXY 이용해서 충돌체크
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

// OnCollision -> 밀어내기 : X, Y 축 중 큰 축으로 밀어내기
// 왼쪽 오브젝트가 오른쪽 오브젝트를 밀어내는 것
void CollisionManager::PushBox(BoxCollider* _left, BoxCollider* _right)
{
	// Radius 구하기 : Scale의 절반씩 더하기
	float radX = _left->GetScale().x * 0.5f + _right->GetScale().x * 0.5f;
	float radY = _left->GetScale().y * 0.5f + _right->GetScale().y * 0.5f;

	// Distance 구하기 : 갖고있는 오브젝트의 위치 차이 구하기
	Vector2 leftObjPos = _left->GetOwner()->GetPos();
	Vector2 rightObjPos = _right->GetOwner()->GetPos();

	Vector2 leftFinalPos = _left->GetFinalPos();
	Vector2 RightFinalPos = _right->GetFinalPos();

	float distX = abs(leftFinalPos.x - RightFinalPos.x);
	float distY = abs(leftFinalPos.y - RightFinalPos.y);

	// 밀어낼 값
	float pushX = radX - distX;
	float pushY = radY - distY;

	//오 - 왼 위치일 때 : pushX, pushY 음수로

	// X축 밀기
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
	// Y축 밀기
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




