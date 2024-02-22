#include "pch.h"
#include "Object_Boss.h"
#include "BossManager.h"

#include "TimeManager.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "KeyManager.h"

#include "BoxCollider.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Animation.h"
#include "Animator.h"
#include "Object_Food.h"
#include "Scene_Stage01.h"
#include "Object_Player.h"
#include "SoundManager.h"
#include "SelectGDI.h"

Object_Boss::Object_Boss(GameProcess* _gameProcess)
	: m_speed(500.f)
	, m_moveDirection{}
	, m_lookDirection{}
	, m_currentState(BossState::IDLE)
	, m_previousState(BossState::IDLE)
	, m_idleTime(1.2f)
	, m_attachedDistance(50.f)
	, m_detectedDistance(400.f)
	, m_animator(nullptr),
	m_isDeteted(false),
	m_isCTRPressed(true)
{
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();

	AnimationInit(_gameProcess->GetResourceManager(), pathManager, mainDC);

	CreateCollider();
	GetBoxCollider()->SetOffsetPos(Vector2(0.f, 50.f));
	GetBoxCollider()->SetScale(Vector2(70.f, 100.f));
}

Object_Boss::~Object_Boss()
{
	int a = 0;
}

// -- Animation -- SANDY
void Object_Boss::AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC)
{
	// Texture �ε�
	Texture* BossWalkTex = _resourceManager->LoadTexture(L"BossWalkTex", L"\\Texture\\Animation\\chef_01.bmp", _pathManager, _mainDC);
	Texture* BossFindTex = _resourceManager->LoadTexture(L"BossFindTex", L"\\Texture\\Animation\\chef_02.bmp", _pathManager, _mainDC);

	// Animator ����
	m_animator = CreateAnimator();

	// Animation ����
	Vector2 lt_begin = Vector2(0.f, 0.f);
	const float GAPX = 900 / 4.f;
	const float GAPY = 1800 / 8.f;

	const Vector2 SLICESIZE = Vector2(GAPX, GAPY);
	const Vector2 STEP = Vector2(GAPX, 0.f);
	const float DURATION = 0.2f;
	const UINT FRAMECOUNT = 4;

	GetAnimator()->CreateAnimation(L"BOSS_MOVE_DOWN", BossWalkTex, lt_begin, SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_MOVE_LEFT", BossWalkTex, lt_begin + Vector2(0.f, GAPY), SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_MOVE_RIGHT", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 2.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_MOVE_UP", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 3.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);

	GetAnimator()->CreateAnimation(L"BOSS_IDLE_DOWN", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 4.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_IDLE_LEFT", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 5.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_IDLE_RIGHT", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 6.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);
	GetAnimator()->CreateAnimation(L"BOSS_IDLE_UP", BossWalkTex, lt_begin + Vector2(0.f, GAPY * 7.f), SLICESIZE, STEP, DURATION, FRAMECOUNT);

	const Vector2 SLICESIZE_ANGRY = Vector2(225.f, 225.f);
	const Vector2 STEP_ANGRY = Vector2(225.f, 0.f);

	GetAnimator()->CreateAnimation(L"BOSS_FIND", BossFindTex, lt_begin, SLICESIZE_ANGRY, STEP_ANGRY, DURATION, 23);

}

void Object_Boss::UpdateAnimation()
{
	// TODO : {} �� �ֱ� �ڵ� ������ ���缭 ���� -- SANDY
	LOOKDIR _lookDir = LOOKDIR::DOWN;
	if (m_lookDirection.x == 1) _lookDir = LOOKDIR::RIGHT;
	else if (m_lookDirection.x == -1) _lookDir = LOOKDIR::LEFT;
	else if (m_lookDirection.y == 1) _lookDir = LOOKDIR::DOWN;
	else if (m_lookDirection.y == -1) _lookDir = LOOKDIR::UP;

	switch (m_currentState)
	{
	case BossState::IDLE:
	{
		if (_lookDir == LOOKDIR::RIGHT)
		{
			GetAnimator()->Play(L"BOSS_IDLE_RIGHT", true);
		}
		else if (_lookDir == LOOKDIR::LEFT)
		{
			GetAnimator()->Play(L"BOSS_IDLE_LEFT", true);
		}
		else if (_lookDir == LOOKDIR::UP)
		{
			GetAnimator()->Play(L"BOSS_IDLE_UP", true);
		}
		else if (_lookDir == LOOKDIR::DOWN)
		{
			GetAnimator()->Play(L"BOSS_IDLE_DOWN", true);
		}
	}
	break;

	case BossState::PATROL:
	{
		if (_lookDir == LOOKDIR::RIGHT)
		{
			GetAnimator()->Play(L"BOSS_MOVE_RIGHT", true);
		}
		else if (_lookDir == LOOKDIR::LEFT)
		{
			GetAnimator()->Play(L"BOSS_MOVE_LEFT", true);
		}
		else if (_lookDir == LOOKDIR::UP)
		{
			GetAnimator()->Play(L"BOSS_MOVE_UP", true);
		}
		else if (_lookDir == LOOKDIR::DOWN)
		{
			GetAnimator()->Play(L"BOSS_MOVE_DOWN", true);
		}
	}
	break;

	case BossState::TRACE:
	{
		if (_lookDir == LOOKDIR::RIGHT)
		{
			GetAnimator()->Play(L"BOSS_MOVE_RIGHT", true);
		}
		else if (_lookDir == LOOKDIR::LEFT)
		{
			GetAnimator()->Play(L"BOSS_MOVE_LEFT", true);
		}
		else if (_lookDir == LOOKDIR::UP)
		{
			GetAnimator()->Play(L"BOSS_MOVE_UP", true);
		}
		else if (_lookDir == LOOKDIR::DOWN)
		{
			GetAnimator()->Play(L"BOSS_MOVE_DOWN", true);
		}
	}
	break;
	// TODO : Naming ANGRY�� �ٲٱ� -- SANDY
	case BossState::FIND:
	{
		GetAnimator()->Play(L"BOSS_FIND", true);
	}
	break;
	case BossState::DONE:
	{

	}
	break;

	default:
		break;
	}
}
// -- Animation -- SANDY

void Object_Boss::ChangeState(BossState _newState)
{
	m_previousState = m_currentState;
	m_currentState = _newState;

	m_isWalkSound = false;
}

bool Object_Boss::StateIs(BossState _state)
{
	return m_currentState == _state;
}

void Object_Boss::Update(GameProcess* _gameProcess)
{
	TimeManager* timeManager = _gameProcess->GetTimeManager();
	BossManager* bossManager = _gameProcess->GetBossManager();
	SceneManager* sceneManager = _gameProcess->GetSceneManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	KeyManager* keyManager = _gameProcess->GetKeyManager();

	// ����� ���� ��Ʈ��
	SoundControl(soundManager);

	// �÷��̾� Ž��
	FindPlayer(sceneManager);

	// [F1]Ű ������ ������ �þ� ���� ǥ��
	if (keyManager->IsKeyState(KEY::CTRL, KEY_STATE::TAP) && m_isCTRPressed == false)
	{
		m_isCTRPressed = true;
	}
	else if (keyManager->IsKeyState(KEY::CTRL, KEY_STATE::TAP) && m_isCTRPressed == true)
	{
		m_isCTRPressed = false;
	}

	switch (m_currentState)
	{
	case BossState::IDLE:
		IdleUpdate(timeManager);
		break;
	case BossState::PATROL:
		PatorlUpdate(timeManager, bossManager);
		break;
	case BossState::TRACE:
		break;
	case BossState::FIND:
		break;
	case BossState::DONE:
		bossManager->DisappearBoss();
		if (!IsDead()) // nullptr �ǵ���� ����?
			_gameProcess->GetEvenetManager()->DeleteObject(this);
		break;
	default:
		break;
	}

	UpdateAnimation();

}

void Object_Boss::Render(HDC _dc)
{
	if (m_isCTRPressed)
	{
		// TODO : �������� �� �׸���
		SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);
		Vector2 pos = GetPos();

		Vector2 lefttop{ pos.x - m_detectedDistance, pos.y - m_detectedDistance };
		Vector2 rightbottom{ pos.x + m_detectedDistance, pos.y + m_detectedDistance };

		Ellipse(_dc, (int)lefttop.x, (int)lefttop.y, (int)rightbottom.x, (int)rightbottom.y);
	}


	ComponentRender(_dc);

#ifdef _DEBUG
	//DebugRender(_dc);
#endif

}

void Object_Boss::DebugRender(HDC _dc)
{
	// State Name
	wstring stateName;
	// brush info
	LOGBRUSH brushInfo;

	// player pos & scale
	Vector2 pos = GetPos();
	Vector2 scale = GetScale();

	HBRUSH newBrush = nullptr;
	HBRUSH oldBrush = nullptr;

	// my rect size
	RECT rt = {
		  (LONG)(pos.x - scale.x)
		, (LONG)(pos.y - scale.y / 2.f)
		, (LONG)(pos.x + scale.x)
		, (LONG)(pos.y + scale.y / 2.f) };

	/// Draw Player Rect
	// ���� State�� ���� ���� ����
	// TODO : FIND ���� -- SANDY
	switch (m_currentState)
	{
	case BossState::IDLE:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x00, 0x00)); // RED
		break;
	case BossState::PATROL:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0x00, 0xFF, 0x00)); // GREEN
		break;
	case BossState::TRACE:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0xEE, 0x82, 0xEE)); // PINK
		break;
	case BossState::DONE:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0x00, 0x00, 0x00)); // BLACK
		break;
	default:
		break;
	}

	// Brush Select
	if (newBrush != nullptr)
	{
		oldBrush = (HBRUSH)SelectObject(_dc, newBrush);
		GetObject(newBrush, sizeof(LOGBRUSH), &brushInfo);
		SetBkColor(_dc, brushInfo.lbColor);
	}

	// Draw Boss
	Ellipse(_dc, rt.left
		, rt.top
		, rt.right
		, rt.bottom);

	/// Draw State Text
	switch (m_currentState)
	{
	case BossState::IDLE:
		stateName = L"IDLE";
		break;
	case BossState::PATROL:
		stateName = L"PATROL";
		break;
	case BossState::TRACE:
		stateName = L"TRACE";
		break;
	case BossState::DONE:
		stateName = L"DONE";
		break;
	default:
		break;
	}

	if (m_isDeteted)
	{
		stateName += L"\n[O]Deteted";
	}
	else
	{
		stateName += L"\\[X]Detected";
	}

	// ���� ���� �ؽ�Ʈ ���
	DrawText(_dc, stateName.c_str(), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	/// Draw Ray
	// �ٶ󺸰� �ִ� ���� Ray �׸���
	Vector2 startPos = pos + (scale / 2.f) * m_lookDirection;
	Vector2 endPos(0.f, 0.f);
	Vector2 length(20.f, 20.f);

	endPos = startPos + length * m_lookDirection;

	MoveToEx(_dc, (int)startPos.x, (int)startPos.y, NULL); // ���������� �̵�
	LineTo(_dc, (int)endPos.x, (int)endPos.y); // �� �׸���

	// newBrush ����
	if (oldBrush != nullptr)
	{
		SelectObject(_dc, oldBrush);
		DeleteObject(newBrush);
	}

	// TODO : �������� �� �׸���
	SelectGDI brush(_dc, BRUSH_TYPE::HOLLOW);
	Vector2 pos1 = GetPos();

	Vector2 lefttop{ pos1.x - m_detectedDistance, pos1.y - m_detectedDistance };
	Vector2 rightbottom{ pos1.x + m_detectedDistance, pos1.y + m_detectedDistance };

	Ellipse(_dc, (int)lefttop.x, (int)lefttop.y, (int)rightbottom.x, (int)rightbottom.y);
}

// TODO : UpdateDetectPlayer $ Update�� �����ϴ� ���̿���, find�� ã�ٿ��� Ž���ϴٰ� �� ���� -- SANDY
void Object_Boss::FindPlayer(SceneManager* sceneManager)
{
	Scene* currentScene = sceneManager->GetCurScene(); // ���� ��

	// ����� ��ġ
	Vector2 pos = GetPos();

	// dynamic_cast<Object_Player*>(m_owner);
	Scene_Stage01* stage01 = dynamic_cast<Scene_Stage01*>(currentScene);

	// �÷��̾�
	Object_Player* player = stage01->GetPlayer();

	// �÷��̾� ��ġ
	Vector2 dest = stage01->GetPlayer()->GetPos();

	// �÷��̾� ���� - ����� ����
	Vector2 mDir = dest - pos;

	// ����԰� �÷��̾� ������ �Ÿ�
	float length = mDir.Size();

	// TODO : ���� if�� ���̱� -- SANDY
	if (length <= m_detectedDistance)
	{
		m_isDeteted = true;

		// �÷��̾ ���� �����϶� �߰�
		if (player->GetPlayerState() == PlayerState::REST_BEFORE ||
			player->GetPlayerState() == PlayerState::REST ||
			player->GetPlayerState() == PlayerState::REST_AFTER)
		{
			ChangeState(BossState::FIND);
		}
	}
	else
	{
		m_isDeteted = false;
	}

	// TODO : GetCurFrame()�� ���Ǵޱ� X -- SANDY ���� state��� �۵�
	if (m_animator->FindAnimation(L"BOSS_FIND")->GetCurFrame() == 18)
	{
		/// �ŷڵ� -100
		stage01->AddTrust(-100.f);
	}
}

void Object_Boss::IdleUpdate(TimeManager* timeManager)
{
	double deltaTime = timeManager->GetDeltaTime();

	// TODO : static �� ����? -- SANDY
	// ��� Update���� �����ϸ鼭 0���� ������ش�...?
	static double bossTime = 0.f;

	bossTime += deltaTime;

	ChangeIdleDir(timeManager);

	// 1.5�� ���� Idle ���¿��ٰ� Patrol�� �ٲ�� -- SANDY �̰� �ƴѵ�..?
	if (bossTime >= m_idleTime)
	{
		// Idle �ð� �ʱ�ȭ ������ Idle �ð��� ����  -- SANDY
		// TODO : �� �ʵ��� Idle ���¿��ٰ� Patrol�� �ٲ����? -- SANDY
		bossTime = 0.f;
		m_idleTime = (double)GetRandomIdleTime();

		ChangeState(BossState::PATROL);
	}
}

void Object_Boss::PatorlUpdate(TimeManager* timeManager, BossManager* bossManager)
{
	double deltaTime = timeManager->GetDeltaTime();

	// ����� ��ġ
	Vector2 pos = GetPos();

	// ��ǥ ��ġ
	Vector2 dest = bossManager->GetDestPos();

	// ������� ������ ����
	Vector2 mDir = dest - pos;

	float length = mDir.Size();

	// �������� �����ϸ� (�Ÿ��� ���������)
		// ***************** Frame�� "9"�ϱ� ���� ������ �ִ�
		// �ڲ� length�� ���� ���� ���Ϸ� �������� ����,, ���е�??
		// deltaTime�� ����??
	if (length <= m_attachedDistance)
	{
		/// ���� �������� ����
		if (bossManager->FindNextDest())
		{
			// Idle�� ����ٰ� �ٽ� Patrol�� ������ ����
			ChangeState(BossState::IDLE);
		}
		/// ���� ������ ����
		else
		{
			// ����� ��
			ChangeState(BossState::DONE);
		}
	}
	// �������� �̵� ��
	else
	{
		// ������� ���� ����
		m_lookDirection = mDir.GetBossDir();

		// �̵� ����
		m_moveDirection = mDir.GetNormalize2();

		pos.x += m_speed * m_moveDirection.x * (float)deltaTime;
		pos.y += m_speed * m_moveDirection.y * (float)deltaTime;

		SetPos(pos);
	}
}

UINT Object_Boss::GetRandomIdleTime()
{
	std::random_device rd;

	std::mt19937 gen(rd());

	std::uniform_int_distribution<UINT> dis(0, 3);

	return dis(gen);
}

UINT Object_Boss::GetRandomDirValue()
{
	std::random_device rd;

	std::mt19937 gen(rd());

	std::uniform_int_distribution<UINT> dis(0, 65599);

	return dis(gen);
}

void Object_Boss::SoundControl(SoundManager* soundManager)
{
	switch (m_currentState)
	{
	case BossState::PATROL:
		// PATROL �� �߼Ҹ� ���� ���
		if (!m_isWalkSound)
		{
			soundManager->PlaySound(L"SE_Walk", SOUND_TYPE::BOSS, 0.1f);
			m_isWalkSound = true;
		}
		break;
	default:
		soundManager->Boss_Pause();
		m_isWalkSound = false;
		break;
	}
}

void Object_Boss::ChangeIdleDir(TimeManager* timeManager)
{
	double deltaTime = timeManager->GetDeltaTime();

	// 1�ʿ� �ѹ��� ������ �ٲ��ش�
	static double changeDirTime = 0.f;

	changeDirTime += deltaTime;

	// 1.2�ʸ��� ����
	if (changeDirTime >= 1.3f)
	{
		// �������� ����
		changeDirTime = 0.f;

		UINT value = GetRandomDirValue();

		// �� �� �� ��
		// 4�� �������� �������� ����
		// (1,0), (-1,0), (0,1), (0,-1)
		if (value % 4 == 0)
		{
			m_lookDirection = Vector2(1, 0);
		}
		else if (value % 4 == 1)
		{
			m_lookDirection = Vector2(-1, 0);
		}
		else if (value % 4 == 2)
		{
			m_lookDirection = Vector2(0, 1);
		}
		else if (value % 4 == 3)
		{
			m_lookDirection = Vector2(0, -1);
		}
	}
}

void Object_Boss::OnCollision(BoxCollider* _other, GameProcess* _gameProcess)
{

	Object* otherObject = _other->GetOwner();

	// Player���� Ȯ���ϰ� �б�
	if (otherObject->GetName() == L"Object_Player")
	{
		CollisionManager::PushBox(GetBoxCollider(), _other);

		// �÷��̾� ������ �ȳ������ϱ�

		Vector2 otherPos = otherObject->GetPos();
		if (otherPos.y >= 950.f)
			otherPos.y = 950.f;
		otherObject->SetPos(otherPos);
	}
}

