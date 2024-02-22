#include "pch.h"
#include "Object_Face.h"
#include "ResourceManager.h"
#include "GameProcess.h"
#include "PathManager.h"
#include "KeyManager.h"
#include "BoxCollider.h"
#include "Texture.h"
#include "SelectGDI.h"
#include "TimeManager.h"

using namespace std;

Object_Face::Object_Face(GameProcess* _gameProcess, NAME _name)
{
	ResourceManager* resourceManager = _gameProcess->GetResourceManager();
	PathManager* pathManager = _gameProcess->GetPathManager();
	HDC mainDC = _gameProcess->GetMainDC();

	// 씬에서 하던 초기화 작업 여기로 이동 / 씬에서는 오브젝트 만들기 필수적인 작업을 하지 않는다.
	// TODO : 다른 씬에서도 꼭 필요한 초기화 작업 객체에서 하도록 하기
	SetPos(Vector2(0.f, 0.f));
	SetScale((Vector2(100.f, 100.f)));
	m_name = _name;

	Texture* m_AllfaceTexture = resourceManager->LoadTexture(L"m_AllfaceTexture", L"\\Texture\\Credit\\cha_tile.bmp", pathManager, mainDC);
	SetTexture(m_AllfaceTexture);

	info[(int)NAME::SEOYEONG][0] = L"이서영";
	info[(int)NAME::SEOYEONG][1] = L"Collider/ Interactable Objects/ Credit";
	info[(int)NAME::SEOYEONG][2] = L"뿌듯~ 의기양양!";

	info[(int)NAME::JUNYEOL][0] = L"공준열";
	info[(int)NAME::JUNYEOL][1] = L"Texture/ Player/ Boss/ Sound";
	info[(int)NAME::JUNYEOL][2] = L"버그없는 프로그래밍을 하는 방법에는\n 두가지가 있으머, \n세번째 방식만이 성공한다";

	info[(int)NAME::YEONHA][0] = L"박연하";
	info[(int)NAME::YEONHA][1] = L"Animation/ Player/ Food, OrderSystem";
	info[(int)NAME::YEONHA][2] = L"\"념념...\" \n\n파인애플 피자 파티원 구합니다";

	info[(int)NAME::GITAE][0] = L"이기태";
	info[(int)NAME::GITAE][1] = L"FrameWork/ UI/ CameraEffect";
	info[(int)NAME::GITAE][2] = L"젤다시작";

	info[(int)NAME::GIBIN][0] = L"박기빈";
	info[(int)NAME::GIBIN][1] = L"메인기획";
	info[(int)NAME::GIBIN][2] = L"로봇은 억까에 꺾이지 않아";

	info[(int)NAME::SANGHYEOK][0] = L"박상혁";
	info[(int)NAME::SANGHYEOK][1] = L"서브기획";
	info[(int)NAME::SANGHYEOK][2] = L"기빈님 나빠요";

	info[(int)NAME::JUHYUN][0] = L"남주현";
	info[(int)NAME::JUHYUN][1] = L"애니메이션/ 배경/ \n오브젝트/ 캐릭터/ Effect";
	info[(int)NAME::JUHYUN][2] = L"사장님이 맛있고 피자가 친절해요\n 그리고 파피협 가입하세요!";

	info[(int)NAME::EUNJI][0] = L"김은지";
	info[(int)NAME::EUNJI][1] = L"아트 UI";
	info[(int)NAME::EUNJI][2] = L"페퍼로니 피자가 좋아요";

	info[(int)NAME::MINJI][0] = L"박민지";
	info[(int)NAME::MINJI][1] = L"아트 (오브젝트)";
	info[(int)NAME::MINJI][2] = L"재미있게 작업했습니다 :)";

	info[(int)NAME::END][0] = L"";
	info[(int)NAME::END][1] = L"";
	info[(int)NAME::END][2] = L"";


	// Collider 생성 : 마우스 위치 보기 편하게 하기
	CreateCollider();
	GetBoxCollider()->SetScale(Vector2(WIDTHTEXTURE, HEIGHTTEXTURE));
	GetBoxCollider()->SetOffsetPos(Vector2(WIDTHTEXTURE / 2, HEIGHTTEXTURE / 2));

	SetName(L"Object_Face");

	POINT resolution = _gameProcess->GetResolution();

	// 회전하는 오브젝트의 현재 위치 구하기
	// 중심 위치 / 중심에서 떨어진 거리 / 초기 각도

	// 1. 중심 위치 : resolution / 2
	// 2. 중심에서 떨어진 거리 : const로 두고 접시 위애서 보고 설정하기
	// 3. 초기 각도 : 2PI / 9

	center = Vector2(resolution.x / 2.f - 50.f, resolution.y / 2.f - 100.f);
	const float PI = 3.141592f;

	m_initalAngle = (2 * PI) / (9.f) * (int)m_name;
	m_angle = m_initalAngle;
}

Object_Face::~Object_Face()
{

}

void Object_Face::Update(GameProcess* _gameProcess)
{
	float deltaTime = (float)_gameProcess->GetTimeManager()->GetDeltaTime();
	Vector2 m_mousePos = _gameProcess->GetKeyManager()->GetMousePos();

	// 접시에서 speed 에 따라 돌기
	m_angle = m_angle + deltaTime * SPEED;

	// 오차 범위에 들어오면 초기 각도로 설정하기 : 숫자 커지면 터진다
	const float ERROR_RANGE = 0.001f;
	if (m_angle >= m_initalAngle && m_angle <= m_initalAngle + ERROR_RANGE)
	{
		m_angle = m_initalAngle;
	}

	Vector2 posPoloarToVector = Vector2(center.x + DISTANCE * cos(m_angle), center.y + DISTANCE * sin(m_angle));
	SetPos(posPoloarToVector);

	// 회전하는 오브젝트의 현재 위치 에서 텍스쳐 크기만큼 Hover 인식하기
	Vector2 m_pos = GetPos();

	if (m_mousePos.x >= m_pos.x &&
		m_mousePos.x <= m_pos.x + WIDTHTEXTURE &&
		m_mousePos.y >= m_pos.y &&
		m_mousePos.y <= m_pos.y + HEIGHTTEXTURE
		)
	{
		isHover = true;
		_hoverName = m_name;
	}
	else
	{
		isHover = false;
		_hoverName = NAME::END;
	}
}

void Object_Face::Render(HDC _dc)
{
	Vector2 m_pos = GetPos();

	TransparentBlt(_dc
		, (int)m_pos.x
		, (int)m_pos.y
		, WIDTHTEXTURE
		, HEIGHTTEXTURE
		, m_tex->GetDC()
		, WIDTHTEXTURE * isHover
		, HEIGHTTEXTURE * (int)m_name
		, WIDTHTEXTURE
		, HEIGHTTEXTURE
		, RGB(255, 0, 255));

	SelectGDI font(_dc, FONT_TYPE::GALMURI9_FONT, RGB(45, 41, 37));

	wstring m_nameText = info[(int)_hoverName][0];
	wstring m_roleText = info[(int)_hoverName][1];
	wstring m_commentText = info[(int)_hoverName][2];

	float posY = 350.f;
	float gap = 150.f;

	Vector2 m_namepos = Vector2(10.f, posY);
	Vector2 m_rolepos = Vector2(10.f, posY + gap);
	Vector2 m_commentpos = Vector2(10.f, posY + gap * 2);

	Vector2 m_namescale = Vector2(1000.f, 1000.f);
	Vector2 m_rolescale = Vector2(1000.f, 1000.f);
	Vector2 m_commentscale = Vector2(1000.f, 1000.f);

	RECT rc = { (long)m_namepos.x,(long)m_namepos.y,(long)(m_namescale.x * 2),(long)(m_namescale.y * 2) };
	RECT rc2 = { (long)m_rolepos.x,(long)m_rolepos.y,(long)(m_rolescale.x * 2),(long)(m_rolescale.y * 2) };
	RECT rc3 = { (long)m_commentpos.x,(long)m_commentpos.y,(long)(m_commentscale.x * 2),(long)(m_commentscale.y * 2) };

	DrawTextW(_dc, m_nameText.c_str(), -1, &rc, DT_CENTER);
	SelectGDI font2(_dc, FONT_TYPE::GALMURI9_FONT_SMALL, RGB(45, 41, 37));


	DrawTextW(_dc, m_roleText.c_str(), -1, &rc2, DT_CENTER);
	DrawTextW(_dc, m_commentText.c_str(), -1, &rc3, DT_CENTER);

	ComponentRender(_dc);
}

