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

	// ������ �ϴ� �ʱ�ȭ �۾� ����� �̵� / �������� ������Ʈ ����� �ʼ����� �۾��� ���� �ʴ´�.
	// TODO : �ٸ� �������� �� �ʿ��� �ʱ�ȭ �۾� ��ü���� �ϵ��� �ϱ�
	SetPos(Vector2(0.f, 0.f));
	SetScale((Vector2(100.f, 100.f)));
	m_name = _name;

	Texture* m_AllfaceTexture = resourceManager->LoadTexture(L"m_AllfaceTexture", L"\\Texture\\Credit\\cha_tile.bmp", pathManager, mainDC);
	SetTexture(m_AllfaceTexture);

	info[(int)NAME::SEOYEONG][0] = L"�̼���";
	info[(int)NAME::SEOYEONG][1] = L"Collider/ Interactable Objects/ Credit";
	info[(int)NAME::SEOYEONG][2] = L"�ѵ�~ �Ǳ���!";

	info[(int)NAME::JUNYEOL][0] = L"���ؿ�";
	info[(int)NAME::JUNYEOL][1] = L"Texture/ Player/ Boss/ Sound";
	info[(int)NAME::JUNYEOL][2] = L"���׾��� ���α׷����� �ϴ� �������\n �ΰ����� ������, \n����° ��ĸ��� �����Ѵ�";

	info[(int)NAME::YEONHA][0] = L"�ڿ���";
	info[(int)NAME::YEONHA][1] = L"Animation/ Player/ Food, OrderSystem";
	info[(int)NAME::YEONHA][2] = L"\"���...\" \n\n���ξ��� ���� ��Ƽ�� ���մϴ�";

	info[(int)NAME::GITAE][0] = L"�̱���";
	info[(int)NAME::GITAE][1] = L"FrameWork/ UI/ CameraEffect";
	info[(int)NAME::GITAE][2] = L"���ٽ���";

	info[(int)NAME::GIBIN][0] = L"�ڱ��";
	info[(int)NAME::GIBIN][1] = L"���α�ȹ";
	info[(int)NAME::GIBIN][2] = L"�κ��� �� ������ �ʾ�";

	info[(int)NAME::SANGHYEOK][0] = L"�ڻ���";
	info[(int)NAME::SANGHYEOK][1] = L"�����ȹ";
	info[(int)NAME::SANGHYEOK][2] = L"���� ������";

	info[(int)NAME::JUHYUN][0] = L"������";
	info[(int)NAME::JUHYUN][1] = L"�ִϸ��̼�/ ���/ \n������Ʈ/ ĳ����/ Effect";
	info[(int)NAME::JUHYUN][2] = L"������� ���ְ� ���ڰ� ģ���ؿ�\n �׸��� ������ �����ϼ���!";

	info[(int)NAME::EUNJI][0] = L"������";
	info[(int)NAME::EUNJI][1] = L"��Ʈ UI";
	info[(int)NAME::EUNJI][2] = L"���۷δ� ���ڰ� ���ƿ�";

	info[(int)NAME::MINJI][0] = L"�ڹ���";
	info[(int)NAME::MINJI][1] = L"��Ʈ (������Ʈ)";
	info[(int)NAME::MINJI][2] = L"����ְ� �۾��߽��ϴ� :)";

	info[(int)NAME::END][0] = L"";
	info[(int)NAME::END][1] = L"";
	info[(int)NAME::END][2] = L"";


	// Collider ���� : ���콺 ��ġ ���� ���ϰ� �ϱ�
	CreateCollider();
	GetBoxCollider()->SetScale(Vector2(WIDTHTEXTURE, HEIGHTTEXTURE));
	GetBoxCollider()->SetOffsetPos(Vector2(WIDTHTEXTURE / 2, HEIGHTTEXTURE / 2));

	SetName(L"Object_Face");

	POINT resolution = _gameProcess->GetResolution();

	// ȸ���ϴ� ������Ʈ�� ���� ��ġ ���ϱ�
	// �߽� ��ġ / �߽ɿ��� ������ �Ÿ� / �ʱ� ����

	// 1. �߽� ��ġ : resolution / 2
	// 2. �߽ɿ��� ������ �Ÿ� : const�� �ΰ� ���� ���ּ� ���� �����ϱ�
	// 3. �ʱ� ���� : 2PI / 9

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

	// ���ÿ��� speed �� ���� ����
	m_angle = m_angle + deltaTime * SPEED;

	// ���� ������ ������ �ʱ� ������ �����ϱ� : ���� Ŀ���� ������
	const float ERROR_RANGE = 0.001f;
	if (m_angle >= m_initalAngle && m_angle <= m_initalAngle + ERROR_RANGE)
	{
		m_angle = m_initalAngle;
	}

	Vector2 posPoloarToVector = Vector2(center.x + DISTANCE * cos(m_angle), center.y + DISTANCE * sin(m_angle));
	SetPos(posPoloarToVector);

	// ȸ���ϴ� ������Ʈ�� ���� ��ġ ���� �ؽ��� ũ�⸸ŭ Hover �ν��ϱ�
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

