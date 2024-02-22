#include "pch.h"
#include "Vector2.h"

// 정적멤버 변수 초기화
const Vector2 Vector2::UnitX{ 1.f,0.f };
const Vector2 Vector2::UnitY{ 0.f, 1.f };
const Vector2 Vector2::One{ 1.f, 1.f };
const Vector2 Vector2::Zero{ 0.f,0.f };
const Vector2 Vector2::Left(1.f, 1.f);
const Vector2 Vector2::Right(1.f, 0.f);
const Vector2 Vector2::Up(0.f, 1.f);
const Vector2 Vector2::Down(0.f, 1.f);

string Vector2::ToString() const
{
	char result[64];
	std::snprintf(result, sizeof(result), "(%.3f, %.3f)", x, y);
	return result;
}

Vector2 Vector2::GetPlayerDir()
{
	// 원본과 같은 벡터를 만든다
	Vector2 temp(x, y);

	if (abs(x) > 0 && abs(y) > 0)
	{
		temp.y = 0.f;
		return temp;
	}
	else
	{
		return temp;
	}
}

Vector2 Vector2::RotateRadian(const Vector2& _point, const Vector2& _center, float _radian)
{
	float cosTheta = std::cos(_radian);
	float sinTheta = std::sin(_radian);

	Vector2 relative = _point - _center;
	Vector2 rotated{};
	rotated.x = _center.x + relative.x * cosTheta - relative.y * sinTheta;
	rotated.y = _center.y + relative.x * sinTheta + relative.y * cosTheta;

	return rotated;
}

void Vector2::Normalize()
{
	*this = GetNormalize();
}

Vector2 Vector2::GetNormalize() const
{
	float squarSum = SizeSquared();
	if (squarSum == 1.f)
	{
		return *this;
	}
	else if (squarSum == 0.f)
	{
		return Vector2::Zero;
	}

	// 고속 역 제곱근 공식을 사용하면 빠른 연산이 가능하지만 아직 어려움

	float invLength = 1 / sqrtf(squarSum);

	return Vector2(x, y) * invLength;
}

Vector2 Vector2::GetNormalize2()
{
	// 원본과 같은 벡터를 만든다
	Vector2 temp(x, y);

	float len = Size();

	if (len == 0.f)
		return Vector2(0.f, 0.f); // Zero Vector
	else if (len == 1.f)
		return temp; // 이미 Normal Vector

	temp.x /= len;
	temp.y /= len;

	return temp;
}

Vector2 Vector2::GetBossDir()
{
	// 원본과 같은 벡터를 만든다
	Vector2 temp(x, y);

	// x가 크면 x의 방향을 따라간다
	if (abs(temp.x) > abs(temp.y))
	{
		temp.y = 0.f;
		return temp.GetNormalize2();
	}
	// y가 크면 y의 방향을 따라간다
	else
	{
		temp.x = 0.f;
		return temp.GetNormalize2();
	}
}