#pragma once
#include "Object_Food.h"

class Object_Order;
enum class PIZZAKIND;

enum class PIZZABAKE
{
	UNBAKED,
	BAKED,
	OVERBAKED,
};

// 팝업메세지 구분을 위한 상태 체크값
enum class BASETOPPINGCHECK
{
	EMPTY,	 // 기본재료가 다 들어가있지 않음
	STAND,	 // 이미 들어가 있는 기본재료
	FULL,	 // 기본 재료가 가득 참
	ALLFULL, // 모든 재료가 가득 참
	OK,		 // 토핑 넣기 성공

	END,	// 몰?루?..
};

class Object_Pizza
	: public Object_Food
{
private:
	// 피자에 들어갈 토핑
	vector<TOPPING> m_pizzaTopping;
	
	PIZZABAKE m_pizzaBaked; // 피자의 구운 상태
	PIZZAKIND m_pizzaKind;  // 피자 종류
	wstring m_pizzaName;	// 피자 이름
	
	// 렌더 전용
	Texture* m_pizzaTopTex;
	Texture* m_pizzaSideTex;
	Vector2 m_slicePos;

public:
	BASETOPPINGCHECK AddTopping(TOPPING _topping);
	void SetPizzaBaked(PIZZABAKE _bakedState) { m_pizzaBaked = _bakedState; }
	UINT GetPizzaToppingSize() { return (UINT)m_pizzaTopping.size(); }

	PIZZAKIND GetPizzaKind() { return m_pizzaKind; }
	wstring GetPizzaName() { return m_pizzaName; }
	PIZZABAKE GetPizzaBaked() { return m_pizzaBaked; }

public:
	void PizzaRenderCheck();

public: /// 피자 이름 생성 관련
	void PizzaNameCreate();
	void SortTopping(vector<TOPPING>* _topping);
	void SwapTopping(vector<TOPPING>* _topping);
	wstring TranslateTopping(TOPPING _topping);

public:
	void Update(GameProcess* _gameProcess);
	void Render(HDC _dc);
	void OwnerRender(HDC _dc);

public:
	Object_Pizza(GameProcess* gameProcess);
	~Object_Pizza() override;
};

