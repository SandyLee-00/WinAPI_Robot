#include "pch.h"
#include "Object_TrashCan.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "Object_Player.h"
#include "EventManager.h"
#include "KeyManager.h"
#include "Animation.h"
#include "Animator.h"
#include "SoundManager.h"

Object_TrashCan::Object_TrashCan(GameProcess* gameProcess) :
	trashInteractionCount{ 0 },
	isMemoPopUP{ false },
	memoTexture{ nullptr },
	m_animator{ nullptr },
	randomMemoIndex{ 0 }
{
	ResourceManager* resourceManager = gameProcess->GetResourceManager();
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();
	POINT resolution = gameProcess->GetResolution();

	// TrashCan Texture �ε�
	Texture* trashCanTexture = resourceManager->LoadTexture(L"Object_TrashCan", L"\\Texture\\Tile\\trash.bmp", pathManager, mainDC);
	SetTexture(trashCanTexture);

	//  TODO : ��Ʈ �޸��� ������ �����ϱ� 
	// �޸��� �ؽ��� �ε�
	memoTexture = resourceManager->LoadTexture(L"Object_Memo", L"\\Texture\\Tile\\memo.bmp", pathManager, mainDC);
	resourceManager->CreateTexture(L"MemoTexture", resolution.x, resolution.y, mainDC);
	std::srand(static_cast<unsigned>(std::time(nullptr)));


	// TrashCan Effect Animation ����
	Texture* EffectTex = resourceManager->LoadTexture(L"TrashCanEffectTex", L"\\Texture\\Animation\\trash_effect.bmp", pathManager, mainDC);

	m_animator = CreateAnimator();

	UINT frameCount = 6;
	float sliceX = 912.f / frameCount;
	float sliceY = 108.f;
	float duration = 0.1f;
	Vector2 lt_begin = Vector2(0.f, 0.f);
	Vector2 sliceSize = Vector2(sliceX, sliceY);
	Vector2 step = Vector2(sliceX, 0.f);

	GetAnimator()->CreateAnimation(L"EFFECT", EffectTex, lt_begin, sliceSize, step, duration, frameCount);

	// Animation Offset ����
	for (int i = 0; i < GetAnimator()->FindAnimation(L"EFFECT")->GetMaxFrame(); ++i)
	{
		GetAnimator()->FindAnimation(L"EFFECT")->GetFrame(i).offset = Vector2(sliceX / 2 - 15, 20.f);
	}

	// Collider ����
	CreateCollider();

	// Collider Scale / OffsetPos ����
	GetBoxCollider()->SetScale(Vector2((float)trashCanTexture->GetWidth(), (float)trashCanTexture->GetHeight()));
	GetBoxCollider()->SetOffsetPos(Vector2((float)trashCanTexture->GetWidth() / 2, (float)trashCanTexture->GetHeight() / 2));

	SetName(L"Object_TrashCan");
}

Object_TrashCan::~Object_TrashCan()
{

}

void Object_TrashCan::OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_TrashCan::OnCollision(BoxCollider* _other, GameProcess* gameProcess)
{
	Object* otherObject = _other->GetOwner();
	KeyManager* _keyManager = gameProcess->GetKeyManager();
	EventManager* _eventManager = gameProcess->GetEvenetManager();
	SoundManager* _soundManager = gameProcess->GetSoundManager();

	// Player���� Ȯ���ϰ� �б�
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);
	}

	// !Player || ![SPACE] : �ٷ� ����������
	if (otherObject->GetName() != L"Object_Player" || !_keyManager->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	// ���� || ���� -> ������
	if (otherObject->GetPizza() != nullptr || otherObject->GetTopping() != nullptr)
	{
		_soundManager->PlaySound(L"SE_Trash", SOUND_TYPE::PLAYER);

		m_animator->Play(L"EFFECT", false);
		// Player�� Food�� ���
		otherObject->DeleteFood(_eventManager);
		m_animator->FindAnimation(L"EFFECT")->SetFrame(0);

		trashInteractionCount++;
	}
	else
	{
		Object_Player* player = dynamic_cast<Object_Player*>(otherObject);
		if (player != nullptr) player->SetPopMessage(L"������ ����!");

		trashInteractionCount++;

	}

	// 3�� ������ �޸��� ����
	if (trashInteractionCount >= MAXTRASHCANINTERACTIONCOUNT)
	{
		trashInteractionCount = 0;
		isMemoPopUP = true;
		// TODO : Index 0~8���� �������� �̱�
		randomMemoIndex = rand() % MEMOCOUNT;
	}
}

void Object_TrashCan::OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess)
{

}

void Object_TrashCan::Update(GameProcess* _gameProcess)
{
	if (isMemoPopUP == false)
	{
		return;
	}

	// �����̽� ������ �޸��� ����
	else if (isMemoPopUP == true &&
		_gameProcess->GetKeyManager()->IsKeyState(KEY::SPACE, KEY_STATE::TAP))
	{
		isMemoPopUP = false;
	}
}

void Object_TrashCan::Render(HDC _dc)
{
	Vector2 pos = GetPos();
	int width = (int)m_tex->GetWidth();
	int height = (int)m_tex->GetHeight();

	TransparentBlt(_dc, (int)pos.x, (int)pos.y, (int)width, (int)height, m_tex->GetDC(), 0, 0, (int)width, (int)height, RGB(255, 0, 255));
	
	// �޸��� ������
	if (isMemoPopUP)
	{
		// TODO : ȸ�� ��� ��ƾ� �Ѵ�

		Vector2 memoPosition = Vector2(300.f, 100.f);
		int memoWidth = (int)memoTexture->GetWidth() / MEMOCOUNT;
		int memoHeight = (int)memoTexture->GetHeight();

		TransparentBlt(
			_dc, 
			(int)memoPosition.x, 
			(int)memoPosition.y, 
			(int)memoWidth,
			(int)memoHeight,
			memoTexture->GetDC(), 
			(int)memoWidth * randomMemoIndex,
			0, 
			(int)memoWidth,
			(int)memoHeight, 
			RGB(255, 0, 255));
	}

	// Collider ������
	ComponentRender(_dc);
}

