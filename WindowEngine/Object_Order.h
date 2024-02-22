#pragma once

class Object_OrderSheetUI;
class Object_Dish;
class Scene_Stage01;
enum class PIZZABAKE;

enum class ORDER_TOPPING
{
	NONE,
	OLIVE,			// �ø���
	PEPPERONI,		// ���۷δ�
	PINEAPPLE,		// ���ξ���
	POTATO,			// ����
	SHIRIMP,		// ����
};

enum class PIZZAKIND
{
	NONE,
	SINGLE,
	DOUBLE,
	DUAL,
};

struct Order
{
public:
	wstring name; // �޴��̸�
	double orderTime;    // �ֹ��� ���������
	double timelimit;    // �ֹ��� ����Ʈ

	Order(wstring _name, double _timeLimit)
		: name(_name)
		, orderTime(0.f)
		, timelimit(_timeLimit)
	{
	}
};

/// �ֹ� ���� Ŭ����
class Object_Order
	: public Object
{

private:
	Object_OrderSheetUI* m_orderSheetUI;		// �ֹ��� UI
	vector<Order*> m_orderList;                 // �ֹ��� ����Ʈ ������ ������ �迭�� ������ �۱⶧���� vector�� ���
	int    m_orderLimit;						// �ִ�� �ֹ������� ����Ʈ 

	double m_orderCreateTime;					// �ֹ� �����ϴ� �ð� üũ
	double m_timeLimit;							// �ֹ� ���ѽð�

	ORDER_TOPPING m_randomTopping[2];			// ���� �����Ǵ� ������ ���� �迭
	wstring m_menu;								// �ֹ��� UI�� �߰��� �޴�
	PIZZAKIND m_pizzaKind;						// �̱��������� ������������

	wstring m_pizzaNameCheck;
	PIZZABAKE m_pizzaBakedCheck;
	Object_Dish* m_dishCheck;
	bool m_animationMode;
	int m_index;
	bool m_deleteOk;
	int m_orderCombo;

	// �� ����� ������ �б����� �޶���
	int m_orderCount;

public:
	vector<Order*>* GetOrderList() { return &m_orderList; }
	int GetOrderCount() { return m_orderCount; }

public:
	void OrderCheck(Object_Dish* _dish, wstring _name, PIZZABAKE _baked);

public:
	void Update(GameProcess* _gameProcess);
	void Render(HDC _dc);

public:
	void UpdateOrderCreate(Scene_Stage01* scene, double deltaTime);
	void UpdateOrderDelete(Scene_Stage01* scene, double deltaTime, SoundManager* soundManager);
	void UpdateOrderCheck(Scene_Stage01* scene, SoundManager* soundManager);
	void UpdateOrderCheckAfter();

public:
	void RandomTopping();
	PIZZAKIND CreatPizzaKind(ORDER_TOPPING _first, ORDER_TOPPING _second);
	void SortTopping(ORDER_TOPPING* _topping);
	void SwapTopping(ORDER_TOPPING* _topping);
	wstring CreateMenu(ORDER_TOPPING _first, ORDER_TOPPING _second);
	wstring TranslateTopping(ORDER_TOPPING _topping);
	void CreateOrder();


public:
	Object_Order(Object_OrderSheetUI* _orderSheetUI, GameProcess* _gameProcess);
	~Object_Order();


};

