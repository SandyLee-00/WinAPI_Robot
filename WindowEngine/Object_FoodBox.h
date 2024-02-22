#pragma once

enum class TOPPING;

class Object_FoodBox :
    public Object
{
private:
	TOPPING m_thisTopping;

public:
    Object_FoodBox(GameProcess* gameProcess);
	Object_FoodBox(TOPPING _topping, GameProcess* gameProcess);
	~Object_FoodBox();

public:
	void OnCollisionEnter(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollision(BoxCollider* _other, GameProcess* gameProcess) override;
	void OnCollisionExit(BoxCollider* _other, GameProcess* gameProcess) override;

public:
	void Update(GameProcess* _gameProcess) override;
	void Render(HDC _dc) override;
};

