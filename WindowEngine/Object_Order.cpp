#include "pch.h"
#include "Object.h"
#include "Object_Order.h"
#include "Object_OrderSheetUI.h"
#include "Object_Dish.h"
#include "Object_Pizza.h"
#include "Object_TextureBarUI.h"

#include "Animator.h"
#include "Animation.h"

#include "TimeManager.h"

#include "Scene.h"
#include "Scene_Stage01.h"
#include "SceneManager.h"
#include "SoundManager.h"

struct Order;

const double ORDER_TIME = 15.f;
const int RANDOM_END = 2;

void Object_Order::RandomTopping()
{
	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;
	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());
	// 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	std::uniform_int_distribution<int> dis(0, 5);

	// 랜덤 토핑 생성
	for (int i = 0; i < RANDOM_END; i++) {
		m_randomTopping[i] = (ORDER_TOPPING)dis(gen);
	}
}

PIZZAKIND Object_Order::CreatPizzaKind(ORDER_TOPPING _first, ORDER_TOPPING _second)
{
	// 두 가지 토핑이 둘 다 없으면 재생성
	if (_first == ORDER_TOPPING::NONE && _second == ORDER_TOPPING::NONE)
		return PIZZAKIND::NONE;
	// 토핑 한 가지가 NONE이면 싱글피자
	else if (_first == ORDER_TOPPING::NONE || _second == ORDER_TOPPING::NONE)
		return PIZZAKIND::SINGLE;
	// 두 가지 토핑이 둘 다 있으면 듀얼피자
	else m_pizzaKind = PIZZAKIND::DUAL;

	// 듀얼피자 중에서 두 가지 토핑이 같은 경우, 더블피자
	if (m_pizzaKind == PIZZAKIND::DUAL && _first == _second)
		return PIZZAKIND::DOUBLE;
	else return PIZZAKIND::DUAL;
}

void Object_Order::SortTopping(ORDER_TOPPING* _topping)
{
	if ((_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::PEPPERONI) ||  /// 페퍼로니 포테이토
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::PINEAPPLE) ||  /// 하와이안 포테이토
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// 슈림프 포테이토
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::OLIVE) ||  /// 올리브 포테이토

		(_topping[0] == ORDER_TOPPING::SHIRIMP && _topping[1] == ORDER_TOPPING::OLIVE) ||	/// 올리브 슈림프

		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::PINEAPPLE) ||	/// 하와이안 페퍼로니
		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// 슈림프 페퍼로니
		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::OLIVE) ||  /// 올리브 페퍼로니

		(_topping[0] == ORDER_TOPPING::PINEAPPLE && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// 슈림프 하와이안
		(_topping[0] == ORDER_TOPPING::PINEAPPLE && _topping[1] == ORDER_TOPPING::OLIVE))			/// 올리브 하와이안
	{
		SwapTopping(m_randomTopping);
	}
}

void Object_Order::SwapTopping(ORDER_TOPPING* _topping)
{
	ORDER_TOPPING tempTopping;
	tempTopping = _topping[0];

	// 토핑 순서 교체
	_topping[0] = _topping[1];
	_topping[1] = tempTopping;
}

std::wstring Object_Order::CreateMenu(ORDER_TOPPING first, ORDER_TOPPING second)
{
	std::wstring topping1, topping2;
	topping1 = TranslateTopping(first);
	topping2 = TranslateTopping(second);

	switch (m_pizzaKind)
	{

		case PIZZAKIND::SINGLE:
		{
			return topping1 + topping2 + L"피자";
		}
		break;
		case PIZZAKIND::DUAL:
		{
			return topping1 + L" " + topping2 + L"피자";
		}
		break;
		case PIZZAKIND::DOUBLE:
		{
			return L"더블 " + topping1 + L"피자";
		}
		break;
	}
	return L"";
}

std::wstring Object_Order::TranslateTopping(ORDER_TOPPING _topping)
{
	switch (_topping)
	{
		case ORDER_TOPPING::NONE:
			return L"";
			break;
		case ORDER_TOPPING::OLIVE:
			return L"올리브";
			break;
		case ORDER_TOPPING::PEPPERONI:
			return L"페퍼로니";
			break;
		case ORDER_TOPPING::PINEAPPLE:
			return L"하와이안";
			break;
		case ORDER_TOPPING::POTATO:
			return L"포테이토";
			break;
		case ORDER_TOPPING::SHIRIMP:
			return L"슈림프";
			break;
	}
	return L"";
}

void Object_Order::CreateOrder()
{
	while (m_pizzaKind == PIZZAKIND::NONE)
	{
		// 토핑 생성
		RandomTopping();
		// 피자 종류 확인
		m_pizzaKind = CreatPizzaKind(m_randomTopping[0], m_randomTopping[1]);
		// 토핑 우선순위별 정렬
		if (m_pizzaKind == PIZZAKIND::DUAL)
			SortTopping(m_randomTopping);
	}

	m_menu = CreateMenu(m_randomTopping[0], m_randomTopping[1]);
	Order* order = new Order(m_menu, m_timeLimit);
	m_orderList.push_back(order);
	m_pizzaKind = PIZZAKIND::NONE;
	m_orderCreateTime = 0.f;
}

void Object_Order::OrderCheck(Object_Dish* _dish, wstring _name, PIZZABAKE _baked)
{
	m_dishCheck = _dish;
	m_pizzaNameCheck = _name;
	m_pizzaBakedCheck = _baked;
}

void Object_Order::Update(GameProcess* _gameProcess)
{
	// Scene 받아오기
	Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
	// SoundManager 받아오기
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	// DeltaTime 받아오기
	TimeManager* timeManager = _gameProcess->GetTimeManager();
	double deltaTime = timeManager->GetDeltaTime();

	/// 주문 생성
	UpdateOrderCreate(scene, deltaTime);

	/// 주문 삭제
	UpdateOrderDelete(scene, deltaTime, soundManager);

	/// 주문 확인
	UpdateOrderCheck(scene, soundManager);
}

void Object_Order::Render(HDC _dc)
{

#ifdef _DEBUG
	/// [디버깅 용도] 주문 횟수 출력
	//wstring translateCount = std::to_wstring(m_orderCount);
	//wstring comboCount = std::to_wstring(m_orderCombo);
	//wstring temp = L"성공 횟수 : " + translateCount + L" 콤보 횟수 : " + comboCount;
	//LPCTSTR orderSuccessCount = temp.c_str();
	//TextOut(_dc, 50 , 30 , orderSuccessCount, lstrlen(orderSuccessCount));
#endif

}

/// 주문 생성
void Object_Order::UpdateOrderCreate(Scene_Stage01* scene, double deltaTime)
{
	// 시간 누적
	m_orderCreateTime += deltaTime;

	// 누적된 시간이 ORDER_TIME을 넘으면
	if (m_orderCreateTime >= ORDER_TIME)
	{
		// 1. 주문이 limit를 넘었을 때 신뢰도 -10
		if (m_orderList.size() >= m_orderLimit)
			scene->AddTrust(-30.f);

		// 2. 주문이 limit를 넘지 않았을 때 주문 생성
		else CreateOrder();

		// 누적시간 초기화
		m_orderCreateTime = 0.f;
	}

}

/// 주문 삭제
void Object_Order::UpdateOrderDelete(Scene_Stage01* scene, double deltaTime, SoundManager* soundManager)
{
	// 주문서 리스트를 계속 검사
	for (int index = 0; index < m_orderList.size();)
	{

		// 주문 제한시간을 지나면
		// 1. 애니메이션 & 사운드 "한번" 실행
		// 2. 애니메이션 재생 끝난거 체크 후 처리

		// 지나지 않았다면
		// 1. 주문별 누적 시간 증가
		// 2. 인덱스 증가

		/// 주문 제한시간을 지나면
		if (m_orderList[index]->orderTime >= m_orderList[index]->timelimit)
		{
			// 1. 애니메이션 & 사운드 "한번" 실행
			if (m_animationMode == false)
			{
				m_orderSheetUI->AnimationSetting(L"OrderFail", index);
				m_orderSheetUI->GetAnimator()->Play(L"OrderFail", false);
				soundManager->PlaySound(L"SE_Fail", SOUND_TYPE::ORDER);

				m_animationMode = true;
			}

			// 2. 애니메이션 재생 끝난거 체크 후 처리
			if (m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderFail")->IsFinish())
			{
				// 주문 리스트에서 삭제
				delete m_orderList[index];
				m_orderList.erase(m_orderList.begin() + index);

				// 초기화
				m_animationMode = false;
				m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderFail")->SetFrame(0);

				// 신뢰도 -20
				soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
				scene->AddTrust(-20.f);

				break;
			}

			break;
		}
		/// 지나지 않았다면
		else
		{
			m_orderList[index]->orderTime += deltaTime;

			// 다음 인덱스로 이동
			index++;
		}
	}
}

/// 주문 확인
void Object_Order::UpdateOrderCheck(Scene_Stage01* scene, SoundManager* soundManager)
{
	// 접시가 없을땐 빠져나가기
	if (m_dishCheck == nullptr) return;

	/// 피자가 구워졌을 때만 검사
	if (m_pizzaBakedCheck == PIZZABAKE::BAKED)
	{
		for (m_index = 0; m_index < m_orderList.size();)
		{
			///  주문 목록에 있을 경우
			if (m_orderList[m_index]->name == m_pizzaNameCheck)
			{
				m_orderSheetUI->AnimationSetting(L"OrderSuccess", m_index);

				soundManager->PlaySound(L"SE_Success", SOUND_TYPE::ORDER); // 주문 성공 사운드

				if (m_animationMode == false)
				{
					m_orderSheetUI->GetAnimator()->Play(L"OrderSuccess", false);
					m_animationMode = true;
				}

				if (m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderSuccess")->IsFinish())
				{
					if (m_orderList[m_index]->orderTime <= 30.f)
					{
						/// 30초 안에 처리했을시 신뢰도 +5
						scene->AddTrust(5.f);
					}

					if (++m_orderCombo == 3)
					{
						/// 주문 세번 연속 성공시 신뢰도 +5
						scene->AddTrust(5.f);
						m_orderCombo = 0; // 콤보 초기화
					}

					m_orderCount++;

					delete m_orderList[m_index];
					m_orderList.erase(m_orderList.begin() + m_index);
					m_deleteOk = true;
					break;
				}

				break;
			}
			/// 없을 경우 index++, 콤보 초기화
			else
			{
				++m_index;
				m_orderCombo = 0;
			}
		}
		/// 주문 목록에 없을 경우
		if (m_index >= m_orderList.size())
		{
			/// 신뢰도 -10
			soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
			scene->AddTrust(-10.f);
			m_orderCombo = 0;
			m_deleteOk = true;
		}
		m_index = 0;
	}

	/// 피자가 안구워졌거나 탔을 경우 -> 피자 삭제
	else
	{
		/// 신뢰도 -10
		soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
		scene->AddTrust(-10.f);
		m_deleteOk = true;
	}

	if (m_deleteOk)
	{
		m_dishCheck->CheckOk();

		// 갖고있는 정보 초기화
		m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderSuccess")->SetFrame(0);
		m_animationMode = false;
		m_pizzaBakedCheck = PIZZABAKE::UNBAKED;
		m_pizzaNameCheck = L"";
		m_dishCheck = nullptr;

		m_deleteOk = false;
	}
}

void Object_Order::UpdateOrderCheckAfter()
{
	m_dishCheck->CheckOk();

	// 갖고있는 정보 초기화
	m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderSuccess")->SetFrame(0);
	m_animationMode = false;
	m_pizzaBakedCheck = PIZZABAKE::UNBAKED;
	m_pizzaNameCheck = L"";
	m_dishCheck = nullptr;
}

Object_Order::Object_Order(Object_OrderSheetUI* _orderSheetUI, GameProcess* _gameProcess)
	: m_orderSheetUI(_orderSheetUI)
	, m_orderCreateTime(0.f)
	, m_randomTopping()
	, m_menu()
	, m_timeLimit(50.f) // 주문 하나당 60초 제한
	, m_pizzaKind(PIZZAKIND::NONE)
	, m_orderLimit(5)
	, m_pizzaNameCheck()
	, m_pizzaBakedCheck()
	, m_dishCheck(nullptr)
	, m_animationMode(false)
	, m_orderCombo(0)
	, m_orderCount(0)
	, m_index(0)
	, m_deleteOk(false)
{
	// 시작할 때 주문 하나 생성
	CreateOrder();
}

Object_Order::~Object_Order()
{
	auto iter = m_orderList.begin();
	while (iter != m_orderList.end())
	{
		delete(*iter);
		++iter;
	}
}
