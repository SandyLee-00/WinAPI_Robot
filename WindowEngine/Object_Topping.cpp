#include "pch.h"
#include "Object.h"
#include "Object_Food.h"
#include "Object_Topping.h"
#include "Object_Player.h"

#include "Animator.h"
#include "Animation.h"

#include "ResourceManager.h"
#include "Texture.h"

Object_Topping::Object_Topping(TOPPING _topping, GameProcess* gameProcess)
	: m_toppingKind()
{
	// 토핑 설정
	SetToppingKind(_topping);
	// Texture 로딩
	ResourceManager* resourceManager = gameProcess->GetResourceManager();
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC = gameProcess->GetMainDC();

	m_tex = resourceManager->LoadTexture(L"Topping", L"\\Texture\\Ingredient\\Topping.bmp", pathManager, mainDC);

	m_slice = 118;
}

Object_Topping::Object_Topping()
	: m_toppingKind()
{

}

Object_Topping::~Object_Topping()
{
}

void Object_Topping::Update(GameProcess* _gameProcess)
{
	if (m_owner != nullptr)
	{
 		Object_Player* player = dynamic_cast<Object_Player*>(m_owner);
		if (player != nullptr) // m_owner가 player일때만 true
		{
			if (player->GetPlayerState() == PlayerState::IDLE)
			{
				int frame = player->GetAnimator()->GetCurAnim()->GetCurFrame();
				if (frame != player->GetAnimator()->GetCurAnim()->GetMaxFrame() - 1)
				{
					m_animOffset = 3.f * frame;
				}

			}
		}
		else m_animOffset = 0;
	}

}

void Object_Topping::OwnerRender(HDC _dc)
{
	/// position setting
	if (m_owner != nullptr)
	{
		m_pos = m_owner->GetPos();
		m_pos.x = m_pos.x - m_slice / 2.f;
		m_pos.y = m_pos.y - m_slice / 2.f;
	}

	Vector2 _toppingDir(m_owner->GetLookDir().x, m_owner->GetLookDir().y);
	LOOKDIR toppingDir = LOOKDIR::DOWN;
	if (_toppingDir.x == 1) toppingDir = LOOKDIR::RIGHT;
	else if (_toppingDir.x == -1) toppingDir = LOOKDIR::LEFT;
	else if (_toppingDir.y == 1) toppingDir = LOOKDIR::DOWN;
	else if (_toppingDir.y == -1) toppingDir = LOOKDIR::UP;

	/// offset
	switch (toppingDir)
	{
		case LOOKDIR::LEFT:
			m_pos.x += -60.f;
			m_pos.y += 50.f;
			break;
		case LOOKDIR::RIGHT:
			m_pos.x += 60.f;
			m_pos.y += 50.f;
			break;
		case LOOKDIR::UP:
			m_pos.x += 0.f;
			m_pos.y += 50.f;
			break;
		case LOOKDIR::DOWN:
			m_pos.x += 0.f;
			m_pos.y += 50.f;
			break;
		default:
			break;
	}

	/// render
	TransparentBlt(_dc
		, (int)(m_pos.x)
		, (int)(m_pos.y + m_animOffset)
		, m_slice
		, m_slice
		, m_tex->GetDC()
		, m_slice * (int)m_toppingKind
		, 0
		, m_slice
		, m_slice
		, RGB(255, 0, 255)
	);
}

void Object_Topping::Render(HDC _dc)
{
	if (nullptr == m_owner)
	{
		/// position setting
		m_pos.x = m_pos.x - m_slice / 2.f;
		m_pos.y = m_pos.y - m_slice / 2.f;

		/// render
		TransparentBlt(_dc
			, (int)(m_pos.x)
			, (int)(m_pos.y)
			, m_slice
			, m_slice
			, m_tex->GetDC()
			, m_slice * (int)m_toppingKind
			, 0
			, m_slice
			, m_slice
			, RGB(255, 0, 255)
		);
	}
}



