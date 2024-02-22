
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
	// ���� ������ DC�� ������
	ReleaseDC(m_hWnd, m_hDC);

	// ������ brush pen ����
	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		// ������ �⺻���� �������� �ʴ´�.
		if (i <= (UINT)PEN_TYPE::WHITE)
			continue;


		DeleteObject(m_pen[i]);
	}

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; ++i)
	{
		// ������ �⺻�귯���� �������� �ʴ´�.
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

	// �ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(Vector2((float)_resolution.x, (float)_resolution.y), false);

	// ���� ����ϴ� ��� �귯�� ����
	CreateBrushPen();

	// ���� ������ DC�� �޾ƿ´�.
	m_hDC = GetDC(_hWnd);

	// �Ŵ��� ���޸𸮿� ���� �Ҵ�
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

	// Manager �ʱ�ȭ ���� �߿�
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

	// ���� ���۸� �뵵�� �ؽ�ó ������ �����
	m_memTex = m_resourceManager->CreateTexture(L"BackBuffer", (UINT)m_resolution.x, (UINT)m_resolution.y,m_hDC);

	// ��Ʈ �߰�
	LoadFont();

	return S_OK; // �ʱ�ȭ ������ ��ȯ
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
	//           �浹 üũ
	// ============================
	m_collisionManager->Update(this);

	// ============================
	//        UI �̺�Ʈ üũ
	// ============================
	m_UIManager->Update(this);

	// ============================
	//           ������
	// ============================
	BufferClear();

	m_sceneManager->Render(m_memTex->GetDC());
	m_cameraManager->Render(m_memTex->GetDC());

	BufferFliping();

	// ============================
	//        �̺�Ʈ ���� ó��
	// ============================
	m_eventManager->Update(this);
}

void GameProcess::ChangeWindowSize(Vector2 _resolution ,bool _menu)
{
	/*
	�� �ڵ�� Windows API �Լ��� ����Ͽ� �������� ��ġ�� ũ�⸦ �����ϴ� �ڵ��Դϴ�.

	 AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	 �� �Լ��� �������� ũ�⸦ �����ϱ� ���� ���˴ϴ�. ���ڷ� ���޵Ǵ� rt�� RECT ����ü��, �������� Ŭ���̾�Ʈ ������ ��Ÿ���ϴ�.
	 WS_OVERLAPPEDWINDOW�� �������� �⺻ ��Ÿ���� ��Ÿ����, ������ ������, ���� ǥ����, ũ�� ���� ����, �޴�, ��輱 ���� ���Ե˴ϴ�.
	 _bMenu�� �����쿡 �޴��� �ִ��� ���θ� ��Ÿ����, TRUE�̸� �޴��� �ְ�, FALSE�̸� �޴��� �����ϴ�. �� �Լ��� �������� ���� ��Ÿ�Ͽ� �´� ũ�⸦ ����Ͽ� RECT ����ü�� ������Ʈ�մϴ�.

	 SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	 �� �Լ��� �������� ��ġ�� ũ�⸦ �����ϴ� �Լ��Դϴ�. ���ڷ� ���޵Ǵ� m_hWnd�� ������ �ڵ��Դϴ�. �� ��° ���ڴ� �����츦 ������ �� ������ �� â�Դϴ�.
	 nullptr�� ���޵Ǹ� ���� �����츦 �������� �����˴ϴ�. �� ��°�� �� ��° ���ڴ� �����찡 ǥ�õ� ��ǥ�Դϴ�. �ټ� ��°�� ���� ��° ���ڴ� �������� ���� ���̸� ��Ÿ���ϴ�.
	 ������ ���ڴ� �����츦 ������ �� ����� �÷��� ���Դϴ�. 0�� �����ϸ� �⺻���� ����մϴ�.

	 �� �Լ��� ȣ���ϸ�, �������� ��ġ�� ũ�Ⱑ �����Ǹ�, ȭ�鿡 ��Ÿ���ϴ�. ������ �������� ũ��� �ռ� ȣ���� AdjustWindowRect �Լ��� ����� �������� ũ��� ��ġ�մϴ�.
	*/

	RECT rt = { 0, 0, (long)_resolution.x, (long)_resolution.y };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _menu);

	/// 0,0���� ���� ���� ��
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	// ��üȭ�� ����
	// TODO : �ؿ� padding�� �����
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
	/// �߰��ؼ� ����ϴ� �귯��

	// �����찡 ������ �⺻ �귯�� (���� ������Ʈ) �޸𸮸� �������� �ʴ´�.
	m_brush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_brush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// ������� ���� �߰��ؼ� ����ϴ� �귯��
	m_brush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_brush[(UINT)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	m_brush[(UINT)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_brush[(UINT)BRUSH_TYPE::SKKBLUE] = CreateSolidBrush(RGB(203, 219, 252));
	m_brush[(UINT)BRUSH_TYPE::BROWN] = CreateSolidBrush(RGB(143, 86, 59));
	m_brush[(UINT)BRUSH_TYPE::MAGENTA] = CreateSolidBrush(RGB(255, 0, 255));
	/// �߰��ؼ� ����ϴ� ��

	// �����찡 ������ �⺻ �� (����  ������Ʈ) �޸𸮸� �������� �ʴ´�.
	m_pen[(UINT)PEN_TYPE::HOLLOW] = (HPEN)GetStockObject(NULL_PEN);
	m_pen[(UINT)PEN_TYPE::WHITE] = (HPEN)GetStockObject(WHITE_PEN);

	m_pen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_pen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_pen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_pen[(UINT)PEN_TYPE::SKKBLUE] = CreatePen(PS_SOLID, 1, RGB(203, 219, 252));
	m_pen[(UINT)PEN_TYPE::BROWN] = CreatePen(PS_SOLID, 1, RGB(143, 86, 59));
	m_pen[(UINT)PEN_TYPE::MAGENTA] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

	/// �߰��ؼ� ����ϴ� ��Ʈ
	m_font[(UINT)FONT_TYPE::DOSPILGI_FONT] = CreateFont(25 // ��Ʈ�� ����
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"DOSPilgi");

	m_font[(UINT)FONT_TYPE::GALMURI7_FONT] = CreateFont(70 // ��Ʈ�� ����
		, 80
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri7 Regular");

	m_font[(UINT)FONT_TYPE::GALMURI9_FONT] = CreateFont(60 // ��Ʈ�� ����
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri9 Regular");

	m_font[(UINT)FONT_TYPE::GALMURI9_FONT_SMALL] = CreateFont(30 // ��Ʈ�� ����
		, 0
		, 0
		, 0
		, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, DEFAULT_PITCH | FF_DONTCARE, L"Galmuri9 Regular");

}
