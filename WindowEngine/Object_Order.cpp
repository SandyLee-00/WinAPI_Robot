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
	// �õ尪�� ��� ���� random_device ����.
	std::random_device rd;
	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	std::mt19937 gen(rd());
	// �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	std::uniform_int_distribution<int> dis(0, 5);

	// ���� ���� ����
	for (int i = 0; i < RANDOM_END; i++) {
		m_randomTopping[i] = (ORDER_TOPPING)dis(gen);
	}
}

PIZZAKIND Object_Order::CreatPizzaKind(ORDER_TOPPING _first, ORDER_TOPPING _second)
{
	// �� ���� ������ �� �� ������ �����
	if (_first == ORDER_TOPPING::NONE && _second == ORDER_TOPPING::NONE)
		return PIZZAKIND::NONE;
	// ���� �� ������ NONE�̸� �̱�����
	else if (_first == ORDER_TOPPING::NONE || _second == ORDER_TOPPING::NONE)
		return PIZZAKIND::SINGLE;
	// �� ���� ������ �� �� ������ �������
	else m_pizzaKind = PIZZAKIND::DUAL;

	// ������� �߿��� �� ���� ������ ���� ���, ��������
	if (m_pizzaKind == PIZZAKIND::DUAL && _first == _second)
		return PIZZAKIND::DOUBLE;
	else return PIZZAKIND::DUAL;
}

void Object_Order::SortTopping(ORDER_TOPPING* _topping)
{
	if ((_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::PEPPERONI) ||  /// ���۷δ� ��������
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::PINEAPPLE) ||  /// �Ͽ��̾� ��������
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// ������ ��������
		(_topping[0] == ORDER_TOPPING::POTATO && _topping[1] == ORDER_TOPPING::OLIVE) ||  /// �ø��� ��������

		(_topping[0] == ORDER_TOPPING::SHIRIMP && _topping[1] == ORDER_TOPPING::OLIVE) ||	/// �ø��� ������

		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::PINEAPPLE) ||	/// �Ͽ��̾� ���۷δ�
		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// ������ ���۷δ�
		(_topping[0] == ORDER_TOPPING::PEPPERONI && _topping[1] == ORDER_TOPPING::OLIVE) ||  /// �ø��� ���۷δ�

		(_topping[0] == ORDER_TOPPING::PINEAPPLE && _topping[1] == ORDER_TOPPING::SHIRIMP) ||	/// ������ �Ͽ��̾�
		(_topping[0] == ORDER_TOPPING::PINEAPPLE && _topping[1] == ORDER_TOPPING::OLIVE))			/// �ø��� �Ͽ��̾�
	{
		SwapTopping(m_randomTopping);
	}
}

void Object_Order::SwapTopping(ORDER_TOPPING* _topping)
{
	ORDER_TOPPING tempTopping;
	tempTopping = _topping[0];

	// ���� ���� ��ü
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
			return topping1 + topping2 + L"����";
		}
		break;
		case PIZZAKIND::DUAL:
		{
			return topping1 + L" " + topping2 + L"����";
		}
		break;
		case PIZZAKIND::DOUBLE:
		{
			return L"���� " + topping1 + L"����";
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
			return L"�ø���";
			break;
		case ORDER_TOPPING::PEPPERONI:
			return L"���۷δ�";
			break;
		case ORDER_TOPPING::PINEAPPLE:
			return L"�Ͽ��̾�";
			break;
		case ORDER_TOPPING::POTATO:
			return L"��������";
			break;
		case ORDER_TOPPING::SHIRIMP:
			return L"������";
			break;
	}
	return L"";
}

void Object_Order::CreateOrder()
{
	while (m_pizzaKind == PIZZAKIND::NONE)
	{
		// ���� ����
		RandomTopping();
		// ���� ���� Ȯ��
		m_pizzaKind = CreatPizzaKind(m_randomTopping[0], m_randomTopping[1]);
		// ���� �켱������ ����
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
	// Scene �޾ƿ���
	Scene_Stage01* scene = dynamic_cast<Scene_Stage01*>(_gameProcess->GetSceneManager()->GetCurScene());
	// SoundManager �޾ƿ���
	SoundManager* soundManager = _gameProcess->GetSoundManager();
	// DeltaTime �޾ƿ���
	TimeManager* timeManager = _gameProcess->GetTimeManager();
	double deltaTime = timeManager->GetDeltaTime();

	/// �ֹ� ����
	UpdateOrderCreate(scene, deltaTime);

	/// �ֹ� ����
	UpdateOrderDelete(scene, deltaTime, soundManager);

	/// �ֹ� Ȯ��
	UpdateOrderCheck(scene, soundManager);
}

void Object_Order::Render(HDC _dc)
{

#ifdef _DEBUG
	/// [����� �뵵] �ֹ� Ƚ�� ���
	//wstring translateCount = std::to_wstring(m_orderCount);
	//wstring comboCount = std::to_wstring(m_orderCombo);
	//wstring temp = L"���� Ƚ�� : " + translateCount + L" �޺� Ƚ�� : " + comboCount;
	//LPCTSTR orderSuccessCount = temp.c_str();
	//TextOut(_dc, 50 , 30 , orderSuccessCount, lstrlen(orderSuccessCount));
#endif

}

/// �ֹ� ����
void Object_Order::UpdateOrderCreate(Scene_Stage01* scene, double deltaTime)
{
	// �ð� ����
	m_orderCreateTime += deltaTime;

	// ������ �ð��� ORDER_TIME�� ������
	if (m_orderCreateTime >= ORDER_TIME)
	{
		// 1. �ֹ��� limit�� �Ѿ��� �� �ŷڵ� -10
		if (m_orderList.size() >= m_orderLimit)
			scene->AddTrust(-30.f);

		// 2. �ֹ��� limit�� ���� �ʾ��� �� �ֹ� ����
		else CreateOrder();

		// �����ð� �ʱ�ȭ
		m_orderCreateTime = 0.f;
	}

}

/// �ֹ� ����
void Object_Order::UpdateOrderDelete(Scene_Stage01* scene, double deltaTime, SoundManager* soundManager)
{
	// �ֹ��� ����Ʈ�� ��� �˻�
	for (int index = 0; index < m_orderList.size();)
	{

		// �ֹ� ���ѽð��� ������
		// 1. �ִϸ��̼� & ���� "�ѹ�" ����
		// 2. �ִϸ��̼� ��� ������ üũ �� ó��

		// ������ �ʾҴٸ�
		// 1. �ֹ��� ���� �ð� ����
		// 2. �ε��� ����

		/// �ֹ� ���ѽð��� ������
		if (m_orderList[index]->orderTime >= m_orderList[index]->timelimit)
		{
			// 1. �ִϸ��̼� & ���� "�ѹ�" ����
			if (m_animationMode == false)
			{
				m_orderSheetUI->AnimationSetting(L"OrderFail", index);
				m_orderSheetUI->GetAnimator()->Play(L"OrderFail", false);
				soundManager->PlaySound(L"SE_Fail", SOUND_TYPE::ORDER);

				m_animationMode = true;
			}

			// 2. �ִϸ��̼� ��� ������ üũ �� ó��
			if (m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderFail")->IsFinish())
			{
				// �ֹ� ����Ʈ���� ����
				delete m_orderList[index];
				m_orderList.erase(m_orderList.begin() + index);

				// �ʱ�ȭ
				m_animationMode = false;
				m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderFail")->SetFrame(0);

				// �ŷڵ� -20
				soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
				scene->AddTrust(-20.f);

				break;
			}

			break;
		}
		/// ������ �ʾҴٸ�
		else
		{
			m_orderList[index]->orderTime += deltaTime;

			// ���� �ε����� �̵�
			index++;
		}
	}
}

/// �ֹ� Ȯ��
void Object_Order::UpdateOrderCheck(Scene_Stage01* scene, SoundManager* soundManager)
{
	// ���ð� ������ ����������
	if (m_dishCheck == nullptr) return;

	/// ���ڰ� �������� ���� �˻�
	if (m_pizzaBakedCheck == PIZZABAKE::BAKED)
	{
		for (m_index = 0; m_index < m_orderList.size();)
		{
			///  �ֹ� ��Ͽ� ���� ���
			if (m_orderList[m_index]->name == m_pizzaNameCheck)
			{
				m_orderSheetUI->AnimationSetting(L"OrderSuccess", m_index);

				soundManager->PlaySound(L"SE_Success", SOUND_TYPE::ORDER); // �ֹ� ���� ����

				if (m_animationMode == false)
				{
					m_orderSheetUI->GetAnimator()->Play(L"OrderSuccess", false);
					m_animationMode = true;
				}

				if (m_orderSheetUI->GetAnimator()->FindAnimation(L"OrderSuccess")->IsFinish())
				{
					if (m_orderList[m_index]->orderTime <= 30.f)
					{
						/// 30�� �ȿ� ó�������� �ŷڵ� +5
						scene->AddTrust(5.f);
					}

					if (++m_orderCombo == 3)
					{
						/// �ֹ� ���� ���� ������ �ŷڵ� +5
						scene->AddTrust(5.f);
						m_orderCombo = 0; // �޺� �ʱ�ȭ
					}

					m_orderCount++;

					delete m_orderList[m_index];
					m_orderList.erase(m_orderList.begin() + m_index);
					m_deleteOk = true;
					break;
				}

				break;
			}
			/// ���� ��� index++, �޺� �ʱ�ȭ
			else
			{
				++m_index;
				m_orderCombo = 0;
			}
		}
		/// �ֹ� ��Ͽ� ���� ���
		if (m_index >= m_orderList.size())
		{
			/// �ŷڵ� -10
			soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
			scene->AddTrust(-10.f);
			m_orderCombo = 0;
			m_deleteOk = true;
		}
		m_index = 0;
	}

	/// ���ڰ� �ȱ������ų� ���� ��� -> ���� ����
	else
	{
		/// �ŷڵ� -10
		soundManager->PlaySound(L"SE_Hmm", SOUND_TYPE::BOSS);
		scene->AddTrust(-10.f);
		m_deleteOk = true;
	}

	if (m_deleteOk)
	{
		m_dishCheck->CheckOk();

		// �����ִ� ���� �ʱ�ȭ
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

	// �����ִ� ���� �ʱ�ȭ
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
	, m_timeLimit(50.f) // �ֹ� �ϳ��� 60�� ����
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
	// ������ �� �ֹ� �ϳ� ����
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
