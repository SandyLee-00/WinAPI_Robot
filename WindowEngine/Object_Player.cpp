#include "pch.h"
#include "Object_Player.h"

#include "TimeManager.h"
#include "KeyManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "EventManager.h"

#include "Animation.h"
#include "Animator.h"
#include "BoxCollider.h"

#include "GameProcess.h"

#include "Object_Food.h"
#include "Object_Pizza.h"
#include "Object_Topping.h"
#include "SoundManager.h"
#include "Object_TextureBarUI.h"

#include "SelectGDI.h"

Object_Player::Object_Player(GameProcess* gameProcess)
	: m_speed(0.f)
	, m_moveDir{ 0.f, 0.f }
	, m_curState(PlayerState::IDLE)
	, m_preState(PlayerState::IDLE)
	, m_decreaseHealthTime(0.f)
	, m_increaseHealthTime(0.f)
	, m_health(100.f)
	, m_deltaTime(0.f)
	, m_moveY(0)
	, m_popMessage()
	, m_popMessageMode(false)
	, m_popMessageTime(0.f)
{
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();

	AnimationInit(gameProcess->GetResourceManager() , pathManager, mainDC);
	CreateCollider();
	GetBoxCollider()->SetOffsetPos(Vector2(0.f, 85.f));
	GetBoxCollider()->SetScale(Vector2(70.f, 80.f));

	SetName(L"Object_Player");
}

Object_Player::~Object_Player()
{
}

void Object_Player::Update(GameProcess* _gameProcess)
{
	m_pos = GetPos();

	Vector2 dir(0.f, 0.f);

	m_deltaTime = _gameProcess->GetTimeManager()->GetDeltaTime();
	KeyManager* keyManager = _gameProcess->GetKeyManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	HDC mainDC = _gameProcess->GetMainDC();

	UpdateMove(keyManager, m_deltaTime);
	UpdateState(keyManager);

	if (!m_isWalk)
	{
		if (m_curState == PlayerState::MOVE)
		{
			//soundManager->PlaySound(L"SE_Walk", SOUND_TYPE::WALK, 0.1f);
			m_isWalk = true;
		}
	}
	else
	{
		if (m_curState == PlayerState::IDLE || m_curState == PlayerState::DEAD)
		{
			//soundManager->WalkChannelPause();
			m_isWalk = false;
		}
	}

	if (m_curState == PlayerState::REST) IncreaseHealth(m_deltaTime);
	else if (m_curState == PlayerState::MOVE) DecreaseHealth(m_deltaTime);

	/// Vector2 를 LOOKDIR 로 변환
	LOOKDIR _lookDir = LOOKDIR::DOWN;
	if		(m_lookdir.x ==  1) _lookDir = LOOKDIR::RIGHT;
	else if (m_lookdir.x == -1) _lookDir = LOOKDIR::LEFT;
	else if (m_lookdir.y ==  1) _lookDir = LOOKDIR::DOWN;
	else if (m_lookdir.y == -1) _lookDir = LOOKDIR::UP;

	/// 플레이어 상태별 애니메이션 재생
	switch (m_curState)
	{
	case PlayerState::IDLE :
	{
		if (_lookDir == LOOKDIR::RIGHT)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"IDLE_RIGHT", true);
			else											GetAnimator()->Play(L"OB_IDLE_RIGHT", true);
		}
		else if (_lookDir == LOOKDIR::LEFT)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"IDLE_LEFT", true);
			else											GetAnimator()->Play(L"OB_IDLE_LEFT", true);
		}
		else if (_lookDir == LOOKDIR::UP)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"IDLE_UP", true);
			else											GetAnimator()->Play(L"OB_IDLE_UP", true);
		}
		else if (_lookDir == LOOKDIR::DOWN)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"IDLE_DOWN", true);
			else											GetAnimator()->Play(L"OB_IDLE_DOWN", true);
		}
	}
	break;
	case PlayerState::MOVE :
	{
		if (_lookDir == LOOKDIR::RIGHT)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"MOVE_RIGHT", true);
			else											GetAnimator()->Play(L"OB_MOVE_RIGHT", true);
		}
		else if (_lookDir == LOOKDIR::LEFT)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"MOVE_LEFT", true);
			else											GetAnimator()->Play(L"OB_MOVE_LEFT", true);
		}
		else if (_lookDir == LOOKDIR::UP)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"MOVE_UP", true);
			else											GetAnimator()->Play(L"OB_MOVE_UP", true);
		}
		else if (_lookDir == LOOKDIR::DOWN)
		{
			if (nullptr == m_pizza && nullptr == m_topping) GetAnimator()->Play(L"MOVE_DOWN", true);
			else											GetAnimator()->Play(L"OB_MOVE_DOWN", true);
		}
	}
	break;
	case PlayerState::REST_BEFORE :
	{
		keyManager->DisableAllKey();
		GetAnimator()->Play(L"REST_START", false);
		if (GetAnimator()->FindAnimation(L"REST_START")->IsFinish())
		{
			GetAnimator()->FindAnimation(L"REST_START")->SetFrame(0);
			ChangeState(PlayerState::REST);
		}
	}
	break;
	case PlayerState::REST :
	{
		keyManager->DisableArrowKey();
		GetAnimator()->Play(L"REST_ING", true);
	}
	break;
	case PlayerState::REST_AFTER:
	{
		keyManager->DisableAllKey();
		GetAnimator()->Play(L"REST_END", false);
		if (GetAnimator()->FindAnimation(L"REST_END")->IsFinish())
		{
			GetAnimator()->FindAnimation(L"REST_END")->SetFrame(0);
			keyManager->ReleaseKey();
			keyManager->ActiveAllKey();
			ChangeState(PlayerState::IDLE);
		}
	}
	break;
	case PlayerState::DEAD :
	{
		GetAnimator()->Play(L"DIE", false);
		break;
	}
	default:
		break;
	}
}

void Object_Player::Render(HDC _dc)
{
#ifdef _DEBUG
	//DebugRender(_dc);
#endif

	if (m_pizza != nullptr && m_pizza->IsDead())
	{
		m_pizza = nullptr;
	}
		
	if (m_topping != nullptr && m_topping->IsDead())
	{
		m_topping = nullptr;
	}	

	if (m_lookdir.y == 1)
	{
		ComponentRender(_dc);

		if (nullptr != m_pizza)
		{
			m_pizza->SetOwner(this);
			m_pizza->OwnerRender(_dc);
		}
		else if (nullptr != m_topping)
		{
			m_topping->SetOwner(this);
			m_topping->OwnerRender(_dc);
		}
	}
	else
	{
		if (nullptr != m_pizza)
		{
			m_pizza->SetOwner(this);
			m_pizza->OwnerRender(_dc);
		}
		else if (nullptr != m_topping)
		{
			m_topping->SetOwner(this);
			m_topping->OwnerRender(_dc);
		}

		ComponentRender(_dc);
	}

	PopMessageDraw(_dc);
}

void Object_Player::DebugRender(HDC _dc)
{
	// State Name
	wstring stateName;

	// brush info
	LOGBRUSH brushInfo{};

	// player pos & scale
	Vector2 pos = GetPos();
	Vector2 scale = GetScale();

	HBRUSH newBrush = nullptr;
	HBRUSH oldBrush = nullptr;

	// rect size
	RECT rt = {
		  (LONG)(pos.x - scale.x)
		, (LONG)(pos.y - 100.f)
		, (LONG)(pos.x + scale.x)
		, (LONG)(pos.y - 80.f) };

	/// Draw Player Rect
	// 현재 State에 따른 플레이어 색상 변경
	switch (m_curState)
	{
	case PlayerState::IDLE:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x00, 0x00)); // RED
		break;
	case PlayerState::MOVE:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0x00, 0xFF, 0x00)); // GREEN
		break;
	case PlayerState::REST_BEFORE:
	case PlayerState::REST:
	case PlayerState::REST_AFTER:
		newBrush = (HBRUSH)CreateSolidBrush(RGB(0xEE, 0x82, 0xEE)); // PINK
		break;
	case PlayerState::DEAD:
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

	// Draw Player Rectangle
	Rectangle(_dc
		, rt.left
		, rt.top
		, rt.right
		, rt.bottom);

	/// Draw State Text
	// 현재 상태를 문자열로,,
	switch (m_curState)
	{
	case PlayerState::IDLE:
		stateName = L"Idle";
		break;
	case PlayerState::MOVE:
		stateName = L"MOVE";
		break;
	case PlayerState::REST_BEFORE:
		stateName = L"REST_BEFORE";
		break;
	case PlayerState::REST:
		stateName = L"REST";
		break;
	case PlayerState::REST_AFTER:
		stateName = L"REST_AFTER";
		break;
	case PlayerState::DEAD:
		stateName = L"DEAD";
		break;
	default:
		break;
	}

	// 현재 상태 텍스트 출력
	DrawText(_dc, stateName.c_str(), -1, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	/// Draw Ray
	BoxCollider* myCollider = GetBoxCollider();

	Vector2 colPos = myCollider->GetFinalPos();
	Vector2 colScale = myCollider->GetScale();

	// 바라보고 있는 방향 Ray 그리기
	Vector2 startPos = colPos + (colScale / 2.f) * m_lookdir;
	Vector2 endPos(0.f, 0.f);
	Vector2 length(100.f, 100.f);

	endPos = startPos + length * m_lookdir;

	HPEN hPen = CreatePen(PS_SOLID, 7, RGB(60, 120, 120)); // 굵기와 색상을 설정한 펜 객체 생성
	HPEN hOldPen = (HPEN)SelectObject(_dc, hPen); // 펜 객체를 디바이스 컨텍스트에 선택

	MoveToEx(_dc, (int)startPos.x, (int)startPos.y, NULL); // 시작점으로 이동
	LineTo(_dc, (int)endPos.x, (int)endPos.y); // 선 그리기

	// newBrush 삭제
	if (oldBrush != nullptr)
	{
		SelectObject(_dc, oldBrush);
		DeleteObject(newBrush);
		SelectObject(_dc, hOldPen);
		DeleteObject(hPen);
	}
}

void Object_Player::AnimationInit(ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC)
{
	// Texture 로딩
	Texture* playerWalkTex = _resourceManager->LoadTexture(L"PlayerWalkTex", L"\\Texture\\Animation\\player.bmp",_pathManager,_mainDC);
	Texture* playerRestTex = _resourceManager->LoadTexture(L"PlayerRestTex", L"\\Texture\\Animation\\rest.bmp", _pathManager, _mainDC);
	Texture* playerRestRepeatTex = _resourceManager->LoadTexture(L"PlayerRestRepeatTex", L"\\Texture\\Animation\\Repetition.bmp", _pathManager, _mainDC);
	Texture* playerDieTex = _resourceManager->LoadTexture(L"PlayerDieTex", L"\\Texture\\Animation\\die.bmp", _pathManager, _mainDC);

	// Animator 생성
	m_animator = CreateAnimator();

	// TODO : 스프라이트 시트 잘라올 때 수치 해놓고 쓰기 -- SANDY
	/*
	Vector2 lt_begin = Vector2(0.f, 0.f);
	const float gapX = 900 / 4.f;
	const float gapY = 1800 / 8.f;

	const Vector2 sliceSize = Vector2(gapX, gapY);
	const Vector2 step = Vector2(gapX, 0.f);
	const float duration = 0.2f;
	const UINT frameCount = 4;

	GetAnimator()->CreateAnimation(L"BOSS_MOVE_DOWN", BossWalkTex, lt_begin, sliceSize, step, duration, 4);
	*/

	// Animation 생성
	GetAnimator()->CreateAnimation(L"MOVE_DOWN", playerWalkTex, Vector2(0.f, 0.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"MOVE_LEFT", playerWalkTex, Vector2(0.f, 255.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"MOVE_RIGHT", playerWalkTex, Vector2(0.f, 510.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"MOVE_UP", playerWalkTex, Vector2(0.f, 765.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_MOVE_DOWN", playerWalkTex, Vector2(0.f, 1020.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_MOVE_LEFT", playerWalkTex, Vector2(0.f, 1275.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_MOVE_RIGHT", playerWalkTex, Vector2(0.f, 1530.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_MOVE_UP", playerWalkTex, Vector2(0.f, 1785.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"IDLE_DOWN", playerWalkTex, Vector2(0.f, 2040.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"IDLE_LEFT", playerWalkTex, Vector2(0.f, 2295.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"IDLE_RIGHT", playerWalkTex, Vector2(0.f, 2550.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"IDLE_UP", playerWalkTex, Vector2(0.f, 2805.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_IDLE_DOWN", playerWalkTex, Vector2(0.f, 3060.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_IDLE_LEFT", playerWalkTex, Vector2(0.f, 3315.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_IDLE_RIGHT", playerWalkTex, Vector2(0.f, 3570.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);
	GetAnimator()->CreateAnimation(L"OB_IDLE_UP", playerWalkTex, Vector2(0.f, 3825.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 4);

	GetAnimator()->CreateAnimation(L"REST_START", playerRestTex, Vector2(0.f, 0.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.3f, 6);
	GetAnimator()->CreateAnimation(L"REST_END", playerRestTex, Vector2(1275.f, 0.f), Vector2(255.f, 255.f), Vector2(-255.f, 0.f), 0.2f, 6);
	GetAnimator()->CreateAnimation(L"REST_ING", playerRestRepeatTex, Vector2(0.f, 0.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.3f, 7);

	GetAnimator()->CreateAnimation(L"DIE", playerDieTex, Vector2(0.f, 0.f), Vector2(255.f, 255.f), Vector2(255.f, 0.f), 0.2f, 21);

	GetAnimator()->Play(L"IDLE_DOWN", true);
}

void Object_Player::PopMessageDraw(HDC _dc)
{
	if (m_popMessageMode == false) return;

	SelectGDI font(_dc, FONT_TYPE::GALMURI9_FONT_SMALL, RGB(89, 33, 33));

	m_popMessageTime += m_deltaTime;

	if (m_popMessageTime >= 0.5f)
	{
		m_popMessageTime = 0;
		m_moveY = 0.f;
		m_popMessageMode = false;
		m_popMessage = L"";
	}
	else
	{
		m_moveY -= 0.3f;
	}

	RECT rc = { (long)(m_pos.x - size(m_popMessage) * 40)
			,(long)(m_pos.y - 100 + m_moveY)
			,(long)(m_pos.x + size(m_popMessage) * 40)
			,(long)(m_pos.y + 100 + m_moveY) };

	DrawTextW(_dc, m_popMessage.c_str(), -1, &rc, DT_CENTER);
}

void Object_Player::UpdateMove(KeyManager* _keyManager, double _deltaTime)
{
	Vector2 pos = GetPos();

	Vector2 mDir(0.f, 0.f);

	if (_keyManager->IsKeyState(KEY::RIGHT, KEY_STATE::HOLD))
	{
		mDir += Vector2(1.f, 0.f);
	}
	if (_keyManager->IsKeyState(KEY::LEFT, KEY_STATE::HOLD))
	{
		mDir += Vector2(-1.f, 0.f);
	}
	if (_keyManager->IsKeyState(KEY::UP, KEY_STATE::HOLD))
	{
		mDir += Vector2(0.f, -1.f);
	}
	if (_keyManager->IsKeyState(KEY::DOWN, KEY_STATE::HOLD))
	{
		mDir += Vector2(0.f, 1.f);
	}

	/// 바라보는 방향
	if (mDir != Vector2::Zero)
	{
		m_lookdir = mDir.GetPlayerDir(); // 최종 방향
	}

	/// 이동 방향
	m_moveDir = mDir.GetNormalize2(); // 단위 방향 벡터

	pos.x += m_speed * m_moveDir.x * (float)_deltaTime;
	pos.y += m_speed * m_moveDir.y * (float)_deltaTime;

	SetPos(pos);
}

void Object_Player::UpdateState(KeyManager* _keyManager)
{
	// Rest, Dead가 아닌 경우
	if (!StateIs(PlayerState::REST_BEFORE) && !StateIs(PlayerState::REST)
		&& !StateIs(PlayerState::REST_AFTER) && !StateIs(PlayerState::DEAD))
	{
		// Idle -> 방향키 입력 없는 경우
		if (m_moveDir == Vector2(0.f, 0.f))
		{
			ChangeState(PlayerState::IDLE);
		}
		// Move -> 방향키 입력 있는 경우
		else
		{
			ChangeState(PlayerState::MOVE);
		}
	}

	// Rest
	// IDLE, MOVE 상태이고 FOOD가 없을때만 휴식 가능

	if (_keyManager->IsKeyState(KEY::Z, KEY_STATE::TAP))
	{
		if (nullptr == m_pizza && nullptr == m_topping)
		{
			if (StateIs(PlayerState::IDLE) || StateIs(PlayerState::MOVE))
			{
				_keyManager->ReleaseKey();
				ChangeState(PlayerState::REST_BEFORE);
			}

			// REST 상태에서 Z키 누를 시 REST_AFTER 모드로 이동
			if (StateIs(PlayerState::REST))
			{
				_keyManager->ReleaseKey();
				ChangeState(PlayerState::REST_AFTER);
			}
		}

	}

}

void Object_Player::ChangeState(PlayerState _newState)
{
	m_preState = m_curState;
	m_curState = _newState;
}

bool Object_Player::StateIs(PlayerState _state)
{
	return m_curState == _state;
}

void Object_Player::DecreaseHealth(double _deltaTime)
{
	// 체력은 -10 이하로 깎이지 않는다
	if (m_health >= -10.f)
	{
		m_decreaseHealthTime += _deltaTime;
		if (m_decreaseHealthTime >= 0.25f)
		{
			m_health--;
			m_decreaseHealthTime = 0.f;
		}
		m_healthBar->SetBarLen(m_health);
	}
	else m_decreaseHealthTime = 0.f;
}

void Object_Player::IncreaseHealth(double _deltaTime)
{
	// 체력은 100을 넘지 않는다
	if (m_health <= 100.f)
	{
		m_increaseHealthTime += _deltaTime;
		if (m_increaseHealthTime >= 0.05f)
		{
			m_health++;
			m_increaseHealthTime = 0.f;
		}
		m_healthBar->SetBarLen(m_health);
	}
	else m_increaseHealthTime = 0.f;
}
