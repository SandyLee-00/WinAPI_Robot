#include "pch.h"
#include "Object.h"
#include "Object_Food.h"
#include "Object_Pizza.h"
#include "Object_Player.h"
#include "Object_Order.h"

#include "Animator.h"
#include "Animation.h"

#include "ResourceManager.h"
#include "Texture.h"

Object_Pizza::Object_Pizza(GameProcess* gameProcess)
	: m_pizzaTopping()
	, m_pizzaBaked(PIZZABAKE::UNBAKED)
	, m_pizzaKind(PIZZAKIND::NONE)
	, m_slicePos()
	, m_pizzaTopTex(nullptr)
	, m_pizzaSideTex(nullptr)
{
	SetName(L"Object_Pizza");
	// Texture 로딩
	ResourceManager* resourceManager = gameProcess->GetResourceManager();
	PathManager* pathManager = gameProcess->GetPathManager();
	HDC mainDC =gameProcess->GetMainDC();


	m_pizzaTopTex = resourceManager->LoadTexture(L"PizzaTop", L"\\Texture\\Ingredient\\PizzaTop.bmp", pathManager, mainDC);
	m_pizzaSideTex = resourceManager->LoadTexture(L"PizzaSide", L"\\Texture\\Ingredient\\PizzaSide.bmp", pathManager, mainDC);
	// 도우를 가진채로 생성
	m_pizzaTopping.push_back(TOPPING::DOUGH);
	m_pizzaTopping.push_back(TOPPING::NONE);
	m_pizzaTopping.push_back(TOPPING::NONE);
}

const int TOPPING_END = 5;

Object_Pizza::~Object_Pizza()
{
	if (m_pizzaTopping.size() == 4)
	{
		m_pizzaKind = PIZZAKIND::SINGLE;
	}
}

void Object_Pizza::Update(GameProcess* _gameProcess)
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

	if (m_pizzaTopping.size() >= 4)
	{
		PizzaNameCreate();
	}
}

BASETOPPINGCHECK Object_Pizza::AddTopping(TOPPING _topping)
{
	// 피자 토핑의 최대 갯수가 넘지 않을 때
	if (m_pizzaTopping.size() < TOPPING_END)
	{
		// 기본재료가 다 들어가있지 않으면
		if (m_pizzaTopping[1] != TOPPING::SOURCE || m_pizzaTopping[2] != TOPPING::CHEEZE)
		{
			if (_topping == TOPPING::SOURCE)
			{
				if (m_pizzaTopping[1] == TOPPING::SOURCE)
				{
					return BASETOPPINGCHECK::STAND;
				}
				else 
				{
					m_pizzaTopping[1] = _topping;
					return BASETOPPINGCHECK::OK;
				}
			}
			else if (_topping == TOPPING::CHEEZE)
			{
				if (m_pizzaTopping[2] == TOPPING::CHEEZE)
				{
					return BASETOPPINGCHECK::STAND;
				}
				else
				{
					m_pizzaTopping[2] = _topping;
					return BASETOPPINGCHECK::OK;
				}
			}
			else return BASETOPPINGCHECK::EMPTY;
		}
		else if (m_pizzaTopping[1] == TOPPING::SOURCE && m_pizzaTopping[2] == TOPPING::CHEEZE)
		{
			if (_topping == TOPPING::SOURCE || _topping == TOPPING::CHEEZE || _topping == TOPPING::DOUGH)
				return BASETOPPINGCHECK::FULL;
			else
			{
				m_pizzaTopping.push_back(_topping);
				return BASETOPPINGCHECK::OK;
			}
		}
		else return BASETOPPINGCHECK::END;
	}
	else return BASETOPPINGCHECK::ALLFULL;
}

void Object_Pizza::PizzaRenderCheck()
{
	/// owner가 플레이어인지 체크
	if (m_owner->GetName() == L"Object_Player")
	{
		m_tex = m_pizzaSideTex;
		m_slice = 90;

		/// position setting
		if (m_owner != nullptr)
		{
			m_pos = m_owner->GetPos();
			m_pos.x = m_pos.x - m_slice / 2.f;
			m_pos.y = m_pos.y - m_slice / 2.f;
		}

		Vector2 _pizzaDir(m_owner->GetLookDir().x, m_owner->GetLookDir().y);
		LOOKDIR pizzaDir = LOOKDIR::DOWN;
		if (_pizzaDir.x == 1) pizzaDir = LOOKDIR::RIGHT;
		else if (_pizzaDir.x == -1) pizzaDir = LOOKDIR::LEFT;
		else if (_pizzaDir.y == 1) pizzaDir = LOOKDIR::DOWN;
		else if (_pizzaDir.y == -1) pizzaDir = LOOKDIR::UP;

		/// offset
		switch (pizzaDir)
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

	}
	else if(m_owner->GetName() == L"Object_CuttingBoard")
	{
		m_pos = m_owner->GetPos();
		m_pos.x = m_pos.x + 10.f;
		m_pos.y = m_pos.y + 9.f;
		m_tex = m_pizzaTopTex;
		m_slice = 120;
	}
	else if (m_owner->GetName() == L"Object_Dish")
	{
		m_pos = m_owner->GetPos();
		m_pos.x = m_pos.x + 12.f;
		m_pos.y = m_pos.y + 11.f;
		m_tex = m_pizzaTopTex;
		m_slice = 120;
	}

}


void Object_Pizza::PizzaNameCreate()
{
	// 피자 종류 체크 ----------------------------------

	if (m_pizzaTopping.size() == 4)
		m_pizzaKind = PIZZAKIND::SINGLE;

	if (m_pizzaTopping.size() == 5)
	{
		if (m_pizzaTopping[3] == m_pizzaTopping[4])
			m_pizzaKind = PIZZAKIND::DOUBLE;
		else m_pizzaKind = PIZZAKIND::DUAL;
	}

	if (m_pizzaKind == PIZZAKIND::DUAL)
		SortTopping(&m_pizzaTopping);
		
	// 피자 이름 체크 ----------------------------------
	
	std::wstring topping1, topping2;
	topping1 = TranslateTopping(m_pizzaTopping[3]);
	if (m_pizzaTopping.size() == 5) {
		topping2 = TranslateTopping(m_pizzaTopping[4]);
	}
	else topping2 = L"";
	
	m_pizzaName = L"";

	switch (m_pizzaKind)
	{
		case PIZZAKIND::NONE:
		{
			m_pizzaName = L"";
		}
			break;
		case PIZZAKIND::SINGLE:
		{
			m_pizzaName = topping1 + topping2 + L"피자";
		}
			break;
		case PIZZAKIND::DOUBLE:
		{
			m_pizzaName = L"더블 " + topping1 + L"피자";
		}
			break;
		case PIZZAKIND::DUAL:
		{
			m_pizzaName = topping1 + L" " + topping2 + L"피자";
		}
			break;
		default:
			break;
	}

}

void Object_Pizza::SortTopping(vector<TOPPING>* _topping)
{
	if (((*_topping)[3] == TOPPING::POTATO && (*_topping)[4] == TOPPING::PEPPERONI) ||  /// 페퍼로니 포테이토
		((*_topping)[3] == TOPPING::POTATO && (*_topping)[4] == TOPPING::PINEAPPLE) ||  /// 하와이안 포테이토
		((*_topping)[3] == TOPPING::POTATO && (*_topping)[4] == TOPPING::SHIRIMP) ||	/// 슈림프 포테이토
		((*_topping)[3] == TOPPING::POTATO && (*_topping)[4] == TOPPING::OLIVE) ||  /// 올리브 포테이토
		 
		((*_topping)[3] == TOPPING::SHIRIMP && (*_topping)[4] == TOPPING::OLIVE) ||	/// 올리브 슈림프
		 
		((*_topping)[3] == TOPPING::PEPPERONI && (*_topping)[4] == TOPPING::PINEAPPLE) ||	/// 하와이안 페퍼로니
		((*_topping)[3] == TOPPING::PEPPERONI && (*_topping)[4] == TOPPING::SHIRIMP) ||	/// 슈림프 페퍼로니
		((*_topping)[3] == TOPPING::PEPPERONI && (*_topping)[4] == TOPPING::OLIVE) ||  /// 올리브 페퍼로니
		 
		((*_topping)[3] == TOPPING::PINEAPPLE && (*_topping)[4] == TOPPING::SHIRIMP) ||	/// 슈림프 하와이안
		((*_topping)[3] == TOPPING::PINEAPPLE && (*_topping)[4] == TOPPING::OLIVE))			/// 올리브 하와이안
	{
		SwapTopping(_topping);
	}
}

void Object_Pizza::SwapTopping(vector<TOPPING>* _topping)
{
	TOPPING tempTopping;
	tempTopping = (*_topping)[3];

	// 토핑 순서 교체
	(*_topping)[3] = (*_topping)[4];
	(*_topping)[4] = tempTopping;
}

wstring Object_Pizza::TranslateTopping(TOPPING _topping)
{
	switch (_topping)
	{
		case TOPPING::NONE:
			return L"";
			break;
		case TOPPING::OLIVE:
			return L"올리브";
			break;
		case TOPPING::PEPPERONI:
			return L"페퍼로니";
			break;
		case TOPPING::PINEAPPLE:
			return L"하와이안";
			break;
		case TOPPING::POTATO:
			return L"포테이토";
			break;
		case TOPPING::SHIRIMP:
			return L"슈림프";
			break;
	}
	return L"";
}

void Object_Pizza::Render(HDC _dc)
{
}

void Object_Pizza::OwnerRender(HDC _dc)
{

	PizzaRenderCheck();

#ifdef _DEBUG
	/// [디버깅 용도] 피자 이름 출력
	//TextOut(_dc, (int)m_pos.x - 20, (int)m_pos.y - 30, m_pizzaName.c_str(), (int)m_pizzaName.size());
#endif

	for (int i = 0; i < m_pizzaTopping.size(); i++)
	{
		switch (i)
		{
		case 0: /// DOUGH
		{
			if (m_pizzaBaked == PIZZABAKE::UNBAKED)
			{
				m_slicePos.x = 0.f;
				m_slicePos.y = 0.f;
			}
			else if (m_pizzaBaked == PIZZABAKE::BAKED)
			{
				m_slicePos.x = m_slice * 1.f;
				m_slicePos.y = 0.f;
			}
			else if (m_pizzaBaked == PIZZABAKE::OVERBAKED)
			{
				m_slicePos.x = 0.f;
				m_slicePos.y = m_slice * 5.f;
			}
		}
		break;
		case 1:	/// SOURCE
		{
			if (m_pizzaTopping[1] == TOPPING::SOURCE)
			{
				m_slicePos.x = m_slice * 2.f;
				m_slicePos.y = 0.f;
			}
		}
		break;
		case 2: /// CHEEZE
		{
			if (m_pizzaTopping[2] == TOPPING::CHEEZE)
			{
				if (m_pizzaBaked == PIZZABAKE::UNBAKED)
				{
					m_slicePos.x = m_slice * 3.f;
					m_slicePos.y = 0.f;
				}
				else if (m_pizzaBaked == PIZZABAKE::BAKED)
				{
					m_slicePos.x = m_slice * 4.f;
					m_slicePos.y = 0.f;
				}
			}
			else return;
		}
		break;
		case 3: /// TOPPING1
		{
			if (m_pizzaBaked == PIZZABAKE::UNBAKED)
			{
				m_slicePos.x = m_slice * (float)m_pizzaTopping[i];
				m_slicePos.y = m_slice * 1.f;
			}
			else if (m_pizzaBaked == PIZZABAKE::BAKED)
			{
				m_slicePos.x = m_slice * (float)m_pizzaTopping[i];
				m_slicePos.y = m_slice * 3.f;
			}
		}
		break;
		case 4: /// TOPPING2
		{
			m_slicePos.x = m_slice * (float)m_pizzaTopping[i];
			m_slicePos.y += m_slice; // 네번째 토핑 렌더할 땐 y축 한번 더 더해서 적용
		}
		break;
		}

		/// render
		TransparentBlt(_dc
			, (int)(m_pos.x)
			, (int)(m_pos.y + m_animOffset)
			, m_slice
			, m_slice
			, m_tex->GetDC()
			, (int)m_slicePos.x  // x축 시작점
			, (int)m_slicePos.y  // y축 시작점
			, m_slice
			, m_slice
			, RGB(255, 0, 255)
		);

		if (m_pizzaBaked == PIZZABAKE::OVERBAKED) return;
	}
}


