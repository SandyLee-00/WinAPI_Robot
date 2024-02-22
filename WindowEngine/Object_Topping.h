#pragma once

class Object_Topping :
	public Object_Food
{
private:
	TOPPING m_toppingKind;

public:
	TOPPING GetToppingKind() { return m_toppingKind; }
	void SetToppingKind(TOPPING _topping) { m_toppingKind = _topping; }

public:
	void Update(GameProcess* _gameProcess);
	virtual void Render(HDC _dc);
	void OwnerRender(HDC _dc);
	
public:
	Object_Topping(TOPPING _topping, GameProcess* gameProcess);
	Object_Topping();
	~Object_Topping() override;
};

