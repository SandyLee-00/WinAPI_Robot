#pragma once

class Object_OrderSheetUI;
class Object_Dish;
class Scene_Stage01;
enum class PIZZABAKE;

enum class ORDER_TOPPING
{
	NONE,
	OLIVE,			// 올리브
	PEPPERONI,		// 페퍼로니
	PINEAPPLE,		// 파인애플
	POTATO,			// 감자
	SHIRIMP,		// 새우
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
	wstring name; // 메뉴이름
	double orderTime;    // 주문후 몇분지난지
	double timelimit;    // 주문후 리미트

	Order(wstring _name, double _timeLimit)
		: name(_name)
		, orderTime(0.f)
		, timelimit(_timeLimit)
	{
	}
};

/// 주문 생성 클래스
class Object_Order
	: public Object
{

private:
	Object_OrderSheetUI* m_orderSheetUI;		// 주문서 UI
	vector<Order*> m_orderList;                 // 주문서 리스트 삭제를 하지만 배열의 공간이 작기때문에 vector를 사용
	int    m_orderLimit;						// 최대로 주문가능한 리스트 

	double m_orderCreateTime;					// 주문 생성하는 시간 체크
	double m_timeLimit;							// 주문 제한시간

	ORDER_TOPPING m_randomTopping[2];			// 랜덤 생성되는 토핑을 넣을 배열
	wstring m_menu;								// 주문서 UI에 추가할 메뉴
	PIZZAKIND m_pizzaKind;						// 싱글피자인지 더블피자인지

	wstring m_pizzaNameCheck;
	PIZZABAKE m_pizzaBakedCheck;
	Object_Dish* m_dishCheck;
	bool m_animationMode;
	int m_index;
	bool m_deleteOk;
	int m_orderCombo;

	// 이 결과로 엔딩의 분기점이 달라짐
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

