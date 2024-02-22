#include "pch.h"
#include "Object_receipt.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "CollisionManager.h"

Object_receipt::Object_receipt(GameProcess* gameProcess)
{
	// Texture 로드
	ResourceManager* resourceManager = gameProcess->GetResourceManager();
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();

	Texture* texture = resourceManager->LoadTexture(L"Background_receipt", L"\\Texture\\BackGround\\Receipts_notes.bmp", pathManager, mainDC);
	SetTexture(texture);
}


Object_receipt::~Object_receipt()
{

}

void Object_receipt::Update(GameProcess* _gameProcess)
{

}

void Object_receipt::Render(HDC _dc)
{

	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();

	// 랜더링
	BitBlt(_dc, (int)pos.x, (int)pos.y, width, height, m_tex->GetDC(), 0, 0, SRCCOPY);
	ComponentRender(_dc);
}

void Object_receipt::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{
	BoxCollider* receiptBoxCollider = GetBoxCollider();
	CollisionManager::PushBox(receiptBoxCollider, _other);
}

void Object_receipt::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	BoxCollider* receiptBoxCollider = GetBoxCollider();
	CollisionManager::PushBox(receiptBoxCollider, _other);
}
