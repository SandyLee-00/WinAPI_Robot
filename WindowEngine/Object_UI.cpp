#include "pch.h"
#include "Object_UI.h"
#include "CameraManager.h"
#include "SelectGDI.h"
#include "KeyManager.h"
#include "Texture.h"
#include "SoundManager.h"

Object_UI::Object_UI()
	:m_childUI{}
	,m_finalUIPos{}
	,m_LbtnDown(false)
	,m_mouseOn(false)
	,m_parentUI(nullptr)
	,m_camAffected(false)
{
}

Object_UI::Object_UI(bool _camAffected)
	:m_childUI{}
	, m_finalUIPos{}
	, m_LbtnDown(false)
	, m_mouseOn(false)
	, m_parentUI(nullptr)
	, m_camAffected(_camAffected)
{
}

Object_UI::~Object_UI()
{
	Safe_Delete_Vec(m_childUI);
}

void Object_UI::AddChildUI(Object_UI* _child)
{
	m_childUI.push_back(_child); _child->m_parentUI = this;
}

void Object_UI::Update(GameProcess* _gameProcess)
{
	UpdateChildUI(_gameProcess);
}

void Object_UI::Finalupdate(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager)
{
	double volatile deltaTime = _deltaTime; // 컴파일러가 변수를 최적화 하는것을 막음

	// 부모 오브젝트 FinalUpdate();
	Object::Finalupdate(deltaTime, _eventManager, _keyManager);

	// UI의 최종 좌표를 구한다.
	m_finalUIPos = GetPos();

	if (GetParentUI())
	{
		Vector2 parentPos = GetParentUI()->GetFinalPos();
		m_finalUIPos += parentPos;
	}

	// UI Mouse 체크
	MouseOnCheck(_keyManager);

	FinalUpdateChildUI(deltaTime, _eventManager, _keyManager);
}

void Object_UI::Render(HDC _dc)
{
	Vector2 pos = GetFinalPos();
	Vector2 scale = GetScale();

	if (nullptr != m_tex)
	{
		int width = (int)m_tex->GetWidth();
		int height = (int)m_tex->GetHeight();
		TransparentBlt(_dc
			, (int)(pos.x)
			, (int)(pos.y)
			, width, height
			, m_tex->GetDC()
			, 0, 0, width, height
			, RGB(255, 0, 255));
	}

	RenderChildUI(_dc);
}

void Object_UI::UpdateChildUI(GameProcess* _gameProcess)
{
	for (size_t i = 0; i < m_childUI.size(); ++i)
	{
		m_childUI[i]->Update(_gameProcess);
	}
}

void Object_UI::FinalUpdateChildUI(double _deltaTime, EventManager* _eventManager, KeyManager* _keyManager)
{
	for (size_t i = 0; i < m_childUI.size(); ++i)
	{
		m_childUI[i]->Finalupdate(_deltaTime,_eventManager,_keyManager);
	}
}

void Object_UI::RenderChildUI(HDC _dc)
{
	for (size_t i = 0; i < m_childUI.size(); ++i)
	{
		m_childUI[i]->Render(_dc);
	}
}

void Object_UI::MouseOnCheck(KeyManager* _keyManager)
{
	Vector2 mousePos = _keyManager->GetMousePos();
	Vector2 scale = GetScale();

	/*if (m_camAffected)
		mousePos = CameraMgr::GetInst()->GetRealPos(mousePos);
*/

	// 마우스 위치정보 받아서 UI 위에있는지 체크 하는 로직
	if (m_finalUIPos.x <= mousePos.x && mousePos.x <= m_finalUIPos.x + scale.x
		&& m_finalUIPos.y <= mousePos.y && mousePos.y <= m_finalUIPos.y + scale.y)
		m_mouseOn = true;
	else
		m_mouseOn = false;


}

void Object_UI::MouseOn(GameProcess* _gameProcess)
{
}

void Object_UI::MouseLbtnDown(GameProcess* _gameProcess)
{

}

void Object_UI::MouseLbtnUp(GameProcess* _gameProcess)
{
}

void Object_UI::MouseLbtnClicked(GameProcess* _gameProcess)
{

}
