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

// �˾��޼��� ������ ���� ���� üũ��
enum class BASETOPPINGCHECK
{
	EMPTY,	 // �⺻��ᰡ �� ������ ����
	STAND,	 // �̹� �� �ִ� �⺻���
	FULL,	 // �⺻ ��ᰡ ���� ��
	ALLFULL, // ��� ��ᰡ ���� ��
	OK,		 // ���� �ֱ� ����

	END,	// ��?��?..
};

class Object_Pizza
	: public Object_Food
{
private:
	// ���ڿ� �� ����
	vector<TOPPING> m_pizzaTopping;
	
	PIZZABAKE m_pizzaBaked; // ������ ���� ����
	PIZZAKIND m_pizzaKind;  // ���� ����
	wstring m_pizzaName;	// ���� �̸�
	
	// ���� ����
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

public: /// ���� �̸� ���� ����
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

