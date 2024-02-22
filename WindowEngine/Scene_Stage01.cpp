#include "pch.h"
#include "Scene_Stage01.h"
// SYSTEM -------------------------------------

#include "Texture.h"
#include "BoxCollider.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "CameraManager.h"
#include "Animator.h"
#include "Animation.h"
#include "SoundManager.h"
#include "BossManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
// OBJECT -------------------------------------
#include "Object.h"
#include "Object_Order.h"
#include "Object_receipt.h"
#include "Object_Background.h"
#include "Object_Player.h"
#include "Object_Wall.h"
#include "Object_TrashCan.h"
#include "Object_Oven.h"
#include "Object_CuttingBoard.h"
#include "Object_Dish.h"
#include "Object_FoodBox.h"
#include "Object_Table.h"
#include "Object_Food.h"
#include "Object_Window.h"
#include "Object_Arrow.h"
// UI -------------------------------------
#include "Object_BarUI.h"
#include "Object_EndingUI.h"
#include "Object_BtnUI.h"
#include "Object_TimeUI.h"
#include "Object_PauseUI.h"
#include "Object_Boss.h"
#include "Object_OrderSheetUI.h"
#include "Object_TextureBarUI.h"
#include "SceneManager.h"

Scene_Stage01::Scene_Stage01()
	:m_rightWindow(nullptr)
	, m_player(nullptr)
	, m_trustBar(nullptr)
	, m_boss(nullptr)
	, m_order(nullptr)
	, m_timeUI(nullptr)
	, m_isGameOverBGM()
{

}

Scene_Stage01::~Scene_Stage01()
{
	DeleteAll();
}

void Scene_Stage01::AddTrust(float _value)
{
	float barLen = m_trustBar->GetBarLen();
	barLen += _value;
	m_trustBar->SetBarLen(barLen);
}

void Scene_Stage01::Update(GameProcess* gameProcess)
{
	Scene::Update(gameProcess);

	BossManager* bossManager = gameProcess->GetBossManager();
	TimeManager* timeManager = gameProcess->GetTimeManager();
	SceneManager* sceneManager = gameProcess->GetSceneManager();
	EventManager* eventManager = gameProcess->GetEvenetManager();
	KeyManager* keyManager = gameProcess->GetKeyManager();
	SoundManager* soundManager = gameProcess->GetSoundManager();

	double deltaTime = timeManager->GetDeltaTime();

	// Boss Update // 퍼즈게임 할때는 보스매니져 업데이트 중단
	if (!m_pauseGame)
		bossManager->Update(deltaTime, gameProcess);

	float trustBarLen = m_trustBar->GetBarLen();


	// esc 누르면 게임 퍼즈
	if (keyManager->IsKeyState(KEY::ESC, KEY_STATE::TAP))
	{
		// 퍼즈 UI가 있는지 확인한다
		auto iter = m_obj[(UINT)OBJECT_TYPE::UI].begin();
		while (iter != m_obj[(UINT)OBJECT_TYPE::UI].end())
		{
			if ((*iter)->GetName() == L"PauseUI")
				break;
			++iter;
		}

		soundManager->PlaySound(L"SE_UI_Touch", SOUND_TYPE::UI);
		// 퍼즈 UI 가 없는경우
		if (iter == m_obj[(UINT)OBJECT_TYPE::UI].end())
		{
			Object_PauseUI* pauseUI = new Object_PauseUI(gameProcess);
			eventManager->CreateObject(pauseUI, OBJECT_TYPE::UI);
		}
		else// 퍼즈 UI를 닫는다
		{
			sceneManager->GetCurScene()->PlayGame();
			eventManager->DeleteObject((*iter));
		}


	}

	/// 게임 오버 ------------------------------------------------------

	if (trustBarLen <= 0.00f || m_player->GetPlayerHealth() <= -10.f)
	{
		CameraManager* cameraManager = gameProcess->GetCameraManager();
		Vector2 playerPos = m_player->GetPos();

		if (!m_isGameOverBGM)
		{
			// 게임 오버 사운드
			soundManager->BGM_Pause();
			soundManager->PlaySound(L"SE_Gameover", SOUND_TYPE::GAMEOVER); // 게임 오버

			m_isGameOverBGM = true;
		}

		keyManager->ReleaseKey();
		keyManager->DisableAllKey();
		m_player->ChangeState(PlayerState::DEAD);
		cameraManager->FocusedOn(4.2f, playerPos);

		if (m_player->GetPlayerState() == PlayerState::DEAD
			&& m_player->GetAnimator()->FindAnimation(L"DIE")->IsFinish())
		{
			keyManager->ReleaseKey();
			keyManager->ActiveAllKey();
			eventManager->ChangeScene(SCENE_TYPE::STAGE01);
		}
	}

	/// 일정 체력/신뢰도 이하일 시 이동속도 변경 ---------------------------

	if (m_player->GetPlayerHealth() <= 20.f) m_player->SetSpeed(500.f);
	else m_player->SetSpeed(700.f);

	if (m_boss != nullptr)
	{
		if (trustBarLen <= 50.f) m_boss->SetSpeed(500.f);
		else m_boss->SetSpeed(300.f);
	}

	/// 게임 클리어 ------------------------------------------------------
	int hour = m_timeUI->GetHour();
	// 현재는 5분을 버티면 게임 클리어
	const int CLEAR_TIME = 1;
	if (hour >= CLEAR_TIME)
	{
		// 배드앤딩
		int orderCount = m_order->GetOrderCount();

		const int clearOrderCount = 2;
		///  TODO:: if (배드앤딩인 경우)
		if (orderCount < clearOrderCount)
			Object_EndingUI::m_isBad = true;
		/// else (해피엔딩인 경우)
		else
			Object_EndingUI::m_isBad = false;
		eventManager->ChangeScene(SCENE_TYPE::ENDING);
	}


}

void Scene_Stage01::PlayWalkingWindow(SoundManager* soundManager)
{
	m_rightWindow->PlayWalkAnimaion(soundManager);
}

void Scene_Stage01::PlayUpWindow(SoundManager* soundManager)
{
	m_rightWindow->PlayUpAnimaion(soundManager);
}

/// <summary>
/// 오브젝트렌더 사장님과 플레이어 레이어설정을 하기위해
/// 랜더부분을 다시 설정
/// </summary>
/// <param name="_dc"></param>
void Scene_Stage01::Render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	{
		// 보스가 있으면서 플레이어 위치가 보스보다 아래인 경우
		if (i == (UINT)OBJECT_TYPE::PLAYER && !m_obj[(UINT)OBJECT_TYPE::BOSS].empty())
		{
			Vector2 playerPos = m_player->GetPos();
			Vector2 bossPos = m_obj[(UINT)OBJECT_TYPE::BOSS][0]->GetPos();

			// 플레이어를 나중에 풀력해야하는 경우
			if (playerPos.y >= bossPos.y)
			{
				auto iterBoss = m_obj[(UINT)OBJECT_TYPE::BOSS].begin();
				for (; iterBoss != m_obj[(UINT)OBJECT_TYPE::BOSS].end();)
				{
					if (!(*iterBoss)->IsDead())
					{
						(*iterBoss)->Render(_dc);
						++iterBoss;
					}
					else
					{
						iterBoss = m_obj[i].erase(iterBoss);
					}
				}
				auto iterPlayer = m_obj[(UINT)OBJECT_TYPE::PLAYER].begin();
				for (; iterPlayer != m_obj[i].end();)
				{
					if (!(*iterPlayer)->IsDead())
					{
						(*iterPlayer)->Render(_dc);
						++iterPlayer;
					}
					else
					{
						iterPlayer = m_obj[i].erase(iterPlayer);
					}
				}

				++i;
				continue;
			}
		}

		// iterator 로 순회
		auto iter = m_obj[i].begin();
		for (; iter != m_obj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(_dc);
				++iter;
			}
			else
			{
				iter = m_obj[i].erase(iter);
			}
		}
	}
}

void Scene_Stage01::Exit()
{
	DeleteAll();
}

void Scene_Stage01::Enter(GameProcess* _gameProcess)
{
	PathManager* pathManager = _gameProcess->GetPathManager();
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	SoundManager* soundManager = _gameProcess->GetSoundManager();

	HDC mainDc = _gameProcess->GetMainDC();

	float firstPosX = 0.0f;
	float firstPosY = 0.0f;
	float gap = 0.0f;

	// 게임 오버 BGM 초기화
	m_isGameOverBGM = false;

	/// ============================
	///             UI
	/// ============================

	// UI 배치
	Object_BtnUI* pauseUI = new Object_BtnUI(_gameProcess, true);
	pauseUI->SetPos(Vector2(1350.f, 20.f));
	pauseUI->SetScale(Vector2(80.f, 90.f));
	pauseUI->SetBtnFunction(BTN_FUNC::PAUSE_UI);
	Texture* pauseTexture = resourceManager->LoadTexture(L"GearUI", L"\\Texture\\UI\\gear_rere.bmp", pathManager, mainDc);
	pauseUI->SetTexture(pauseTexture);
	AddObject(pauseUI, OBJECT_TYPE::UI);

	// 신뢰도UI
	Texture* trustEmptyTexture = resourceManager->LoadTexture(L"bar_empty_green", L"\\Texture\\UI\\bar_empty_green.bmp", pathManager, mainDc);
	Texture* trustFillTexture = resourceManager->LoadTexture(L"bar_fill_green", L"\\Texture\\UI\\bar_green.bmp", pathManager, mainDc);

	Object_TextureBarUI* trustBar = new Object_TextureBarUI(trustEmptyTexture, trustFillTexture);
	trustBar->SetPos(Vector2(1580.f, 800.f));
	trustBar->SetPos(Vector2(1590.f, 810.f));
	trustBar->SetOffSet(Vector2(1.f, 1.f));
	trustBar->SetBarLen(100.f);
	AddObject(trustBar, OBJECT_TYPE::UI);

	// 신뢰도 UI ICON
	Object_BtnUI* trustBarIcon = new Object_BtnUI(_gameProcess);
	trustBarIcon->SetPos(Vector2(1540.f, 775.f));
	trustBarIcon->SetPos(Vector2(1540.f, 780.f));
	Texture* trustIcon = resourceManager->LoadTexture(L"sajangIcon", L"\\Texture\\UI\\sajang.bmp", pathManager, mainDc);
	trustBarIcon->SetTexture(trustIcon);
	AddObject(trustBarIcon, OBJECT_TYPE::UI);

	// 신뢰도 바 등록
	RegisterTrustBar(trustBar);

	// 체력UI
	Texture* heartEmptyTexture = resourceManager->LoadTexture(L"bar_empty_red", L"\\Texture\\UI\\bar_empty_red.bmp", pathManager, mainDc);
	Texture* heartFillTexture = resourceManager->LoadTexture(L"bar_fill_red", L"\\Texture\\UI\\bar_red.bmp", pathManager, mainDc);

	Object_TextureBarUI* heartBar = new Object_TextureBarUI(heartEmptyTexture, heartFillTexture);
	heartBar->SetPos(Vector2(1580.f, 710.f));
	heartBar->SetPos(Vector2(1590.f, 710.f));
	heartBar->SetOffSet(Vector2(1.f, 1.f));
	heartBar->SetBarLen(100.f);
	AddObject(heartBar, OBJECT_TYPE::UI);

	// 체력 UI ICON
	Object_BtnUI* heartBarIcon = new Object_BtnUI(_gameProcess);
	heartBarIcon->SetPos(Vector2(1555.f, 695.f));
	heartBarIcon->SetPos(Vector2(1550.f, 695.f));
	Texture* heartIcon = resourceManager->LoadTexture(L"heartIcon", L"\\Texture\\UI\\heart.bmp", pathManager, mainDc);
	heartBarIcon->SetTexture(heartIcon);
	AddObject(heartBarIcon, OBJECT_TYPE::UI);

	// 타이머UI
	Object_TimeUI* timeUI = new Object_TimeUI(_gameProcess);
	timeUI->SetPos(Vector2(1565, 860));
	AddObject(timeUI, OBJECT_TYPE::SYSTEM);
	RegisterTimeUI(timeUI);

	// 주문서UI
	Object_OrderSheetUI* orderSheetUI = new Object_OrderSheetUI(_gameProcess);
	AddObject(orderSheetUI, OBJECT_TYPE::UI);

	// 주문서SYSTEM
	Object_Order* orderSystem = new Object_Order(orderSheetUI, _gameProcess);
	RegisterOrder(orderSystem);
	AddObject(orderSystem, OBJECT_TYPE::SYSTEM);
	orderSheetUI->SetOrderList(orderSystem->GetOrderList());

	// 오븐 BAR UI
	Texture* ovenBarEmptyTexture = resourceManager->LoadTexture(L"ovenBarEmptyTexture", L"\\Texture\\UI\\fire_gauge_1.bmp", pathManager, mainDc);
	Texture* ovenBarFillTexture = resourceManager->LoadTexture(L"ovenBarFillTexture", L"\\Texture\\UI\\fire_gauge_2.bmp", pathManager, mainDc);

	Object_TextureBarUI* oven1Bar = new Object_TextureBarUI(ovenBarEmptyTexture, ovenBarFillTexture);
	Object_TextureBarUI* oven2Bar = new Object_TextureBarUI(ovenBarEmptyTexture, ovenBarFillTexture);
	Object_TextureBarUI* oven3Bar = new Object_TextureBarUI(ovenBarEmptyTexture, ovenBarFillTexture);

	firstPosX = 410.f;
	gap = 200.f;

	oven1Bar->SetPos(Vector2(firstPosX, 60.f));
	oven2Bar->SetPos(Vector2(firstPosX + gap, 60.f));
	oven3Bar->SetPos(Vector2(firstPosX + gap * 2, 60.f));

	oven1Bar->SetScale(Vector2(100.f, 20.f));
	oven2Bar->SetScale(Vector2(100.f, 20.f));
	oven3Bar->SetScale(Vector2(100.f, 20.f));

	oven1Bar->SetOffSet(Vector2(1.f, 1.f));
	oven2Bar->SetOffSet(Vector2(1.f, 1.f));
	oven3Bar->SetOffSet(Vector2(1.f, 1.f));

	oven1Bar->SetBarLen(0.f);
	oven2Bar->SetBarLen(0.f);
	oven3Bar->SetBarLen(0.f);

	AddObject(oven1Bar, OBJECT_TYPE::UI);
	AddObject(oven2Bar, OBJECT_TYPE::UI);
	AddObject(oven3Bar, OBJECT_TYPE::UI);


	/// ============================
	///            Player
	/// ============================

	Object_Player* player = new Object_Player(_gameProcess);
	player->SetPos(Vector2(960.f, 540.f));
	player->SetScale(Vector2(60.f, 60.f));
	player->SetSpeed(700.f);
	player->SetHeartBar(heartBar);
	AddObject(player, OBJECT_TYPE::PLAYER);

	RegisterPlayer(player);

	/// ============================
	///          Background
	/// ============================

	Object_Background* background = new Object_Background(_gameProcess);
	background->SetPos(Vector2(0, 0));
	AddObject(background, OBJECT_TYPE::BACKGROUND);

	// 영수증 배치
	Object_receipt* recipt = new Object_receipt(_gameProcess);
	recipt->SetPos(Vector2(1455.f, 0.f));
	AddObject(recipt, OBJECT_TYPE::BACKGROUND);

	// 벽 배치
	Object_Wall* wallLeft = new Object_Wall(_gameProcess);
	Object_Wall* wallRight = new Object_Wall(_gameProcess);
	Object_Wall* wallUp = new Object_Wall(_gameProcess);
	Object_Wall* wallDown = new Object_Wall(_gameProcess);

	// Collider SetScale / Object SetPos 설정하기->texture가 없어서 씬에서 해주기
	wallLeft->SetPos(Vector2(-50.f, 540.f));
	wallRight->SetPos(Vector2(1510.f, 540.f));
	wallUp->SetPos(Vector2(960.f, 110.f));
	wallDown->SetPos(Vector2(960.f, 1130.f));

	wallLeft->GetBoxCollider()->SetScale(Vector2(100.f, 1080.f));
	wallRight->GetBoxCollider()->SetScale(Vector2(100.f, 1080.f));
	wallUp->GetBoxCollider()->SetScale(Vector2(1920.f, 100.f));
	wallDown->GetBoxCollider()->SetScale(Vector2(1920.f, 100.f));

	AddObject(wallLeft, OBJECT_TYPE::BACKGROUND);
	AddObject(wallRight, OBJECT_TYPE::BACKGROUND);
	AddObject(wallUp, OBJECT_TYPE::BACKGROUND);
	AddObject(wallDown, OBJECT_TYPE::BACKGROUND);

	Object_Arrow* arrow = new Object_Arrow(_gameProcess);
	arrow->SetPos(Vector2(1220.f, 330.f));
	AddObject(arrow, OBJECT_TYPE::BACKGROUND);

	/// ============================
	///            Object
	/// ============================

	// Table 배치
	Texture* texturetableCenter = _gameProcess->GetResourceManager()->LoadTexture(L"texturetableCenter", L"\\Texture\\Tile\\center_table.bmp", _gameProcess->GetPathManager(), _gameProcess->GetMainDC());
	Texture* texturetableEdge1 = _gameProcess->GetResourceManager()->LoadTexture(L"texturetableEdge1", L"\\Texture\\Tile\\edge_table1.bmp", _gameProcess->GetPathManager(), _gameProcess->GetMainDC());
	Texture* texturetableEdge2 = _gameProcess->GetResourceManager()->LoadTexture(L"texturetableEdge2", L"\\Texture\\Tile\\edge_table2.bmp", _gameProcess->GetPathManager(), _gameProcess->GetMainDC());
	Texture* texturetableCounter = _gameProcess->GetResourceManager()->LoadTexture(L"texturetableCounter", L"\\Texture\\Tile\\counter_table_resize.bmp", _gameProcess->GetPathManager(), _gameProcess->GetMainDC());

	Object_Table* tableCenter = new Object_Table(_gameProcess, texturetableCenter);
	Object_Table* tableEdge1 = new Object_Table(_gameProcess, texturetableEdge1);
	Object_Table* tableEdge2 = new Object_Table(_gameProcess, texturetableEdge2);
	Object_Table* tableCounter = new Object_Table(_gameProcess, texturetableCounter);

	tableCenter->SetPos(Vector2(500.f, 560.f));
	tableEdge1->SetPos(Vector2(0.f, 300.f));
	tableEdge2->SetPos(Vector2(130.f, 950.f));
	tableCounter->SetPos(Vector2(1315.f, 300.f));

	tableCenter->SetScale(Vector2(100.f, 100.f));
	tableEdge1->SetScale(Vector2(100.f, 100.f));
	tableEdge2->SetScale(Vector2(100.f, 100.f));
	tableCounter->SetScale(Vector2(100.f, 100.f));

	BoxCollider* collidertableCenter = tableCenter->GetBoxCollider();
	// 	BoxCollider* collidertableEdge1 = tableEdge1->GetBoxCollider();
	// 	BoxCollider* collidertableEdge2 = tableEdge2->GetBoxCollider();
	// 	BoxCollider* collidertableCounter = tableCounter->GetBoxCollider();

	collidertableCenter->SetScale(Vector2((float)texturetableCenter->GetWidth() * 0.5f, (float)texturetableCenter->GetHeight() * 0.5f));
	// 	collidertableEdge1->SetScale(Vector2((float)texturetableEdge1->GetWidth() * 0.95f, (float)texturetableEdge1->GetHeight() * 0.95f));
	// 	collidertableEdge2->SetScale(Vector2((float)texturetableEdge2->GetWidth() * 0.95f, (float)texturetableEdge2->GetHeight() * 0.95f));
	// 	collidertableCounter->SetScale(Vector2((float)texturetableCounter->GetWidth() * 0.95f, (float)texturetableCounter->GetHeight() * 0.95f));

	AddObject(tableCenter, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(tableEdge1, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(tableEdge2, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(tableCounter, OBJECT_TYPE::INTERACTABLE_OBJECT);

	// 오븐 배치
	Object_Oven* oven1 = new Object_Oven(_gameProcess, oven1Bar, 1); // 1번 Type
	Object_Oven* oven2 = new Object_Oven(_gameProcess, oven2Bar, 2); // 2번 Type
	Object_Oven* oven3 = new Object_Oven(_gameProcess, oven3Bar, 3); // 3번 Type

	// 가로로 위쪽에 쭉 놓기
	oven1->SetPos(Vector2(400.f, 0.f));
	oven2->SetPos(Vector2(600.f, 0.f));
	oven3->SetPos(Vector2(800.f, 0.f));

	oven1->SetScale(Vector2(100.f, 100.f));
	oven2->SetScale(Vector2(100.f, 100.f));
	oven3->SetScale(Vector2(100.f, 100.f));

	AddObject(oven1, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(oven2, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(oven3, OBJECT_TYPE::INTERACTABLE_OBJECT);

	oven1->SetOvenTimerBar(oven1Bar);
	oven2->SetOvenTimerBar(oven2Bar);
	oven3->SetOvenTimerBar(oven3Bar);

	// Dish 놓기
	Object_Dish* dish1 = new Object_Dish(_gameProcess, orderSystem);
	Object_Dish* dish2 = new Object_Dish(_gameProcess, orderSystem);
	Object_Dish* dish3 = new Object_Dish(_gameProcess, orderSystem);

	firstPosY = 300.f;
	gap = 159.f;

	dish1->SetPos(Vector2(1315.f, firstPosY));
	dish2->SetPos(Vector2(1315.f, firstPosY + gap));
	dish3->SetPos(Vector2(1315.f, firstPosY + gap * 2));

	dish1->SetScale(Vector2(100.f, 100.f));
	dish2->SetScale(Vector2(100.f, 100.f));
	dish3->SetScale(Vector2(100.f, 100.f));

	AddObject(dish1, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(dish2, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(dish3, OBJECT_TYPE::INTERACTABLE_OBJECT);

	// FoodBox
	Object_FoodBox* foodBox1 = new Object_FoodBox(TOPPING::OLIVE, _gameProcess);
	Object_FoodBox* foodBox2 = new Object_FoodBox(TOPPING::PEPPERONI, _gameProcess);
	Object_FoodBox* foodBox3 = new Object_FoodBox(TOPPING::PINEAPPLE, _gameProcess);
	Object_FoodBox* foodBox4 = new Object_FoodBox(TOPPING::POTATO, _gameProcess);
	Object_FoodBox* foodBox5 = new Object_FoodBox(TOPPING::SHIRIMP, _gameProcess);
	Object_FoodBox* foodBox6 = new Object_FoodBox(TOPPING::DOUGH, _gameProcess);
	Object_FoodBox* foodBox7 = new Object_FoodBox(TOPPING::SOURCE, _gameProcess);
	Object_FoodBox* foodBox8 = new Object_FoodBox(TOPPING::CHEEZE, _gameProcess);

	firstPosX = 180.f;
	gap = 180.0f;

	foodBox1->SetPos(Vector2(firstPosX, 965.f));
	foodBox2->SetPos(Vector2(firstPosX + gap, 965.f));
	foodBox3->SetPos(Vector2(firstPosX + gap * 2, 965.f));
	foodBox4->SetPos(Vector2(firstPosX + gap * 3, 965.f));
	foodBox5->SetPos(Vector2(firstPosX + gap * 4, 965.f));

	firstPosY = 400.f;

	foodBox6->SetPos(Vector2(15.f, firstPosY));
	foodBox7->SetPos(Vector2(15.f, firstPosY + gap));
	foodBox8->SetPos(Vector2(15.f, firstPosY + gap * 2));

	foodBox1->SetScale(Vector2(100.f, 100.f));
	foodBox2->SetScale(Vector2(100.f, 100.f));
	foodBox3->SetScale(Vector2(100.f, 100.f));
	foodBox4->SetScale(Vector2(100.f, 100.f));
	foodBox5->SetScale(Vector2(100.f, 100.f));
	foodBox6->SetScale(Vector2(100.f, 100.f));
	foodBox7->SetScale(Vector2(100.f, 100.f));
	foodBox8->SetScale(Vector2(100.f, 100.f));

	Texture* textureFood = _gameProcess->GetResourceManager()->LoadTexture(L"textureFood", L"\\Texture\\Tile\\Box_cheese.bmp", _gameProcess->GetPathManager(), _gameProcess->GetMainDC());

	foodBox1->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox2->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox3->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox4->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox5->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox6->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox7->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));
	foodBox8->GetBoxCollider()->SetScale(Vector2((float)textureFood->GetWidth() * 1.3f, (float)textureFood->GetHeight() * 1.3f));

	AddObject(foodBox1, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox2, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox3, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox4, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox5, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox6, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox7, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(foodBox8, OBJECT_TYPE::INTERACTABLE_OBJECT);

	// Cutting Board 놓기
	Object_CuttingBoard* cuttingBoard1 = new Object_CuttingBoard(_gameProcess);
	Object_CuttingBoard* cuttingBoard2 = new Object_CuttingBoard(_gameProcess);
	Object_CuttingBoard* cuttingBoard3 = new Object_CuttingBoard(_gameProcess);

	firstPosX = 500.f;
	gap = 145.0f;

	cuttingBoard1->SetPos(Vector2(firstPosX, 560.f));
	cuttingBoard2->SetPos(Vector2(firstPosX + gap, 560.f));
	cuttingBoard3->SetPos(Vector2(firstPosX + gap * 2, 560.f));

	cuttingBoard1->SetScale(Vector2(100.f, 100.f));
	cuttingBoard2->SetScale(Vector2(100.f, 100.f));
	cuttingBoard3->SetScale(Vector2(100.f, 100.f));

	AddObject(cuttingBoard1, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(cuttingBoard2, OBJECT_TYPE::INTERACTABLE_OBJECT);
	AddObject(cuttingBoard3, OBJECT_TYPE::INTERACTABLE_OBJECT);

	/// 사장님 윈도우

	// 사장님이 지나다니는 윈도우
	Object_Window* rightWindow = new Object_Window(_gameProcess);
	rightWindow->SetPos(Vector2(1200.f, 70.f));
	AddObject(rightWindow, OBJECT_TYPE::INTERACTABLE_OBJECT);

	// 오른족 윈도우 씬에 등록
	RegisterRightWindow(rightWindow);

	// 쓰레기통 배치
	Object_TrashCan* trashCan = new Object_TrashCan(_gameProcess);
	trashCan->SetPos(Vector2(5.f, 130.f));
	trashCan->SetScale(Vector2(100.f, 100.f));
	AddObject(trashCan, OBJECT_TYPE::INTERACTABLE_OBJECT);


	CollisionManager* collisionManager = _gameProcess->GetCollisionManager();

	// 충돌정보 설정
	collisionManager->CheckObjectType(OBJECT_TYPE::PLAYER, OBJECT_TYPE::BACKGROUND);
	collisionManager->CheckObjectType(OBJECT_TYPE::PLAYER, OBJECT_TYPE::INTERACTABLE_OBJECT);
	collisionManager->CheckObjectType(OBJECT_TYPE::PLAYER, OBJECT_TYPE::BOSS);

	/// ============================
	///            Sound
	/// ============================

	soundManager->PlaySound(L"BGM_In_game", SOUND_TYPE::BGM);

	/// 카메라 효과 설정
	_gameProcess->GetCameraManager()->AllBlack(0.5f);
	_gameProcess->GetCameraManager()->FocusedOut(2.f, player->GetPos());
}


