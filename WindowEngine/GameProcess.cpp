
#include "pch.h"
#include "GameProcess.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "PathManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "SelectGDI.h"
#include "Texture.h"
#include "CameraManager.h"
#include "BossManager.h"
#include "SoundManager.h"

HBRUSH GameProcess::m_brush[(UINT)BRUSH_TYPE::END];
HPEN GameProcess::m_pen[(UINT)PEN_TYPE::END];
HFONT  GameProcess::m_font[(UINT)FONT_TYPE::END];

GameProcess::GameProcess()
	:m_hWnd(nullptr)
	, m_resolution{}
	, m_hDC(nullptr)
	, m_soundManager(nullptr)
	, m_cameraManager(nullptr)
	, m_collisionManager(nullptr)
	, m_eventManager(nullptr)
	, m_keyManager(nullptr)
	, m_bossManager(nullptr)
	, m_pathManager(nullptr)
	, m_resourceManager(nullptr)
	, m_timeManager(nullptr)
	, m_sceneManager(nullptr)
	, m_UIManager(nullptr)
{}

GameProcess::~GameProcess()
{
	// 메인 윈도우 DC는 릴리즈
	ReleaseDC(m_hWnd, m_hDC);

	// 생성한 brush pen 해제
	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		// 윈도우 기본펜은 삭제하지 않는다.
		if (i <= (UINT)PEN_TYPE::WHITE)
			continue;


		DeleteObject(m_pen[i]);
	}

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; ++i)
	{
		// 윈도우 기본브러쉬는 삭제하지 않는다.
		if(i <= (UINT)BRUSH_TYPE::BLACK)
			continue;

		DeleteObject(m_brush[i]);
	}

	for (int i = 0; i < (UINT)FONT_TYPE::END; ++i)
	{
		if (m_font != nullptr)
		{
			DeleteObject(m_font[i]);
		}
	}

}

int GameProcess::Init(HWND _hWnd, POINT _resolution)
{
	m_hWnd = _hWnd;
	m_resolution = _resolution;

	// 해상도에 맞게 윈도우 크기 조정
	ChangeWindowSize(Vector2((float)_resolution.x, (float)_resolution.y), false);

	// 자주 사용하는 펜과 브러쉬 생성
	CreateBrushPen();

	// 메인 윈도우 DC를 받아온다.
	m_hDC = GetDC(_hWnd);

	// 매니저 힙메모리에 공간 할당
	m_soundManager = new SoundManager;
	m_cameraManager = new CameraManager;
	m_collisionManager = new CollisionManager;
	m_bossManager = new BossManager;
	m_eventManager = new EventManager;
	m_keyManager = new KeyManager;
	m_pathManager = new PathManager;
	m_resourceManager = new ResourceManager;
	m_timeManager = new TimeManager;
	m_sceneManager = new SceneManager;
	m_UIManager = new UIManager;

	// Manager 초기화 순서 중요
	m_eventManager->Initialize();
	m_UIManager->Initialize();
	m_collisionManager->Initialize();
	m_bossManager->Initialize();
	m_pathManager->Initialize();
	m_soundManager->Initialize(m_pathManager);
	m_timeManager->Initialize();
	m_keyManager->Initialize();
	m_resourceManager->Initialize();
	m_cameraManager->Initialize(m_resolution,m_resourceManager,m_pathManager,m_hDC);
	m_sceneManager->Initialize(this);

	// 이중 버퍼링 용도의 텍스처 한장을 만든다
	m_memTex = m_resourceManager->CreateTexture(L"BackBuffer", (UINT)m_resolution.x, (UINT)m_resolution.y,m_hDC);

	// 폰트 추가
	LoadFont();

	return S_OK; // 초기화 성공을 반환
}

void GameProcess::Exit()
{
	delete m_soundManager;
	delete m_cameraManager;
	delete m_collisionManager;
	delete m_eventManager;
	delete m_bossManager;
	delete m_keyManager;
	delete m_pathManager;
	delete m_resourceManager;
	delete m_timeManager;
	delete m_sceneManager;
	delete m_UIManager;
}

void GameProcess::Progress()
{
	// ============================
	//        Manager Update
    // ============================
	m_timeManager->Update(m_hWnd);
	m_keyManager->Update(m_hWnd);
	m_cameraManager->Update(m_timeManager, m_keyManager);

	// ============================
	//        Scene Update
	// ============================
	m_sceneManager->Update(this);
	m_sceneManager->FinalUpdate(m_timeManager, m_eventManager, m_keyManager);

	// ============================
	//           충돌 체크
	// ============================
	m_collisionManager->Update(this);

	// ============================
	//        UI 이벤트 체크
	// ============================
	m_UIManager->Update(this);

	// ============================
	//           랜더링
	// ============================
	BufferClear();

	m_sceneManager->Render(m_memTex->GetDC());
	m_cameraManager->Render(m_memTex->GetDC());

	BufferFliping();

	// ============================
	//        이벤트 지연 처리
	// ============================
	m_eventManager->Update(this);
}

void GameProcess::ChangeWindowSize(Vector2 _resolution ,bool _menu)
{
	/*
	이 코드는 Windows API 함수를 사용하여 윈도우의 위치와 크기를 조정하는 코드입니다.

	 AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	 이 함수는 윈도우의 크기를 조정하기 위해 사용됩니다. 인자로 전달되는 rt는 RECT 구조체로, 윈도우의 클라이언트 영역을 나타냅니다.
	 WS_OVERLAPPEDWINDOW는 윈도우의 기본 스타일을 나타내며, 윈도우 프레임, 제목 표시줄, 크기 조절 막대, 메뉴, 경계선 등이 포함됩니다.
	 _bMenu는 윈도우에 메뉴가 있는지 여부를 나타내며, TRUE이면 메뉴가 있고, FALSE이면 메뉴가 없습니다. 이 함수는 윈도우의 설정 스타일에 맞는 크기를 계산하여 RECT 구조체를 업데이트합니다.

	 SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	 이 함수는 윈도우의 위치와 크기를 조정하는 함수입니다. 인자로 전달되는 m_hWnd는 윈도우 핸들입니다. 두 번째 인자는 윈도우를 조정할 때 기준이 될 창입니다.
	 nullptr이 전달되면 현재 윈도우를 기준으로 조정됩니다. 세 번째와 네 번째 인자는 윈도우가 표시될 좌표입니다. 다섯 번째와 여섯 번째 인자는 윈도우의 폭과 높이를 나타냅니다.
	 마지막 인자는 윈도우를 조정할 때 사용할 플래그 값입니다. 0을 전달하면 기본값을 사용합니다.

	 이 함수를 호출하면, 윈도우의 위치와 크기가 조정되며, 화면에 나타납니다. 조정된 윈도우의 크기는 앞서 호출한 AdjustWindowRect 함수로 계산한 윈도우의 크기와 일치합니다.
	*/

	RECT rt = { 0, 0, (long)_resolution.x, (long)_resolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _menu);

	/// 0,0으로 돌려 놓을 것
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// 전체화면 설정
	// TODO : 밑에 padding이 생긴다
	 SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
}

void GameProcess::BufferFliping()
{
	BitBlt(m_hDC, 0, 0, m_resolution.x, m_resolution.y, m_memTex->GetDC(), 0, 0, SRCCOPY);
}

void GameProcess::BufferClear()
{
	SelectGDI gdi(m_memTex->GetDC(), BRUSH_TYPE::BLACK);

	Rectangle(m_memTex->GetDC(), -1, -1, m_resolution.x + 1, m_resolution.y);
}

void GameProcess::LoadFont()
{
	wstring DOSPilgi = m_pathManager->GetContentPath();
	DOSPilgi += L"\\Font\\DOSPilgi.ttf";

	assert(AddFontResourceW(DOSPilgi.c_str()));

	wstring Galmuri7 = m_pathManager->GetContentPath();
	Galmuri7 += L"\\Font\\Galmuri7.ttf";

	assert(AddFontResourceW(Galmuri7.c_str()));

	wstring Galmuri9 = m_pathManager->GetContentPath();
	Galmuri9 += L"\\Font\\Galmuri9.ttf";

	assert(AddFontResourceW(Galmuri9.c_str()));

}

void GameProcess::CreateBrushPen()
{
	/// 추가해서 사용하는 브러쉬

	// 윈도우가 가지는 기본 브러쉬 (스톡 오브젝트) 메모리를 해제하지 않는다.
	m_brush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_brush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// 여기부터 내가 추가해서 사용하는 브러쉬
	m_brush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_brush[(UINT)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	m_brush[(UINT)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_brush[(UINT)BRUSH_TYPE::SKKBLUE] = CreateSolidBrush(RGB(203, 219, 252));
	m_brush[(UINT)BRUSH_TYPE::BROWN] = CreateSolidBrush(RGB(143, 86, 59));
	m_brush[(UINT)BRUSH_TYPE::MAGENTA] = CreateSolidBrush(RGB(255, 0, 255));
	/// 추가해서 사용하는 펜

	// 윈도우가 가지는 기본 펜 (스톡  오브젝트) 메모리를 해제하지 않는다.
	m_pen[(UINT)PEN_TYPE::HOLLOW] = (HPEN)GetStockObject(NULL_PEN);
	m_pen[(UINT)PEN_TYPE::WHITE] = (HPEN)GetStockObject(WHITE_PEN);

	m_pen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_pen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_pen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_pen[(UINT)PEN_TYPE::SKKBLUE] = CreatePen(PS_SOLID, 1, RGB(203, 219, 252));
	m_pen[(UINT)PEN_TYPE::BROWN] = CreatePen(PS_SOLID, 1, RGB(143, 86, 59));
	m_pen[(UINT)PEN_TYPE::MAGENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	/// 추가해서 사용하는 폰트
	m_font[(UINT)FONT_TYPE::DOSPILGI_FONT] = CreateFont(25 // 폰트의 높이
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"DOSPilgi");

	m_font[(UINT)FONT_TYPE::GALMURI7_FONT] = CreateFont(70 // 폰트의 높이
		, 80
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri7 Regular");

	m_font[(UINT)FONT_TYPE::GALMURI9_FONT] = CreateFont(60 // 폰트의 높이
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri9 Regular");

	m_font[(UINT)FONT_TYPE::GALMURI9_FONT_SMALL] = CreateFont(30 // 폰트의 높이
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri9 Regular");

}
