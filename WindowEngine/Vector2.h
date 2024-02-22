#pragma once

#include "PathPoint.h"

struct Vector2
{
public:
	/// 생성자
	inline Vector2() = default;
	inline explicit constexpr Vector2(int _x, int _y);
	inline explicit constexpr Vector2(float _x, float _y);

	/// 연산자
	inline constexpr Vector2 operator-() const;
	inline constexpr Vector2 operator*(float _scalar) const;
	inline constexpr Vector2 operator/(float _scalar) const;
	inline constexpr Vector2 operator*(const Vector2& _other) const;
	inline constexpr Vector2 operator+(const Vector2& _other) const;
	inline constexpr Vector2 operator-(const Vector2& _other) const;
	inline constexpr Vector2& operator=(const PathPoint& _other);
	inline constexpr Vector2& operator*=(float _scale);
	inline constexpr Vector2& operator/=(float _scale);
	inline constexpr Vector2& operator+=(const Vector2& _other);
	inline constexpr Vector2& operator-=(const Vector2& _other);
	inline constexpr bool operator==(const Vector2& _other);
	inline constexpr bool operator!=(const Vector2& _other);

	/// 멤버 함수

	// 루트 제곱근 (길이)
	inline float Size() const;

	// 제곱근 반환
	inline constexpr float SizeSquared() const;

	// 벡터 정규화
	void Normalize();
	[[nodiscard]] Vector2 GetNormalize() const;

	// 반환값은 radian
	inline float Angle() const;

	// 직교좌표계 -> 극좌표계 변환
	inline Vector2 ToPolarCoordinate() const;

	// 극좌표계 -> 직교 좌표계 변환
	inline Vector2 ToCartesianCoordinate() const;

	string ToString() const;

	// return normal
	[[nodiscard]] Vector2 GetNormalize2();

	// return dir
	[[nodiscard]] Vector2 GetPlayerDir();

	// return bossDir
	[[nodiscard]] Vector2 GetBossDir();

	// 회번변환
	static Vector2 RotateRadian(const Vector2& _point, const Vector2& _center, float _radian);

	/// 정적멤버 변수
	static const Vector2 UnitX; // 표준기저벡터
	static const Vector2 UnitY; // 표준기저벡터
	static const Vector2 One;
	static const Vector2 Zero;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Up;
	static const Vector2 Down;
	static constexpr int Dimension = 2;


	/// 멤버 변수
	float x, y;

};

inline constexpr Vector2::Vector2(int _x, int _y)
	:x((float)_x)
	, y((float)_y)
{}

inline constexpr Vector2::Vector2(float _x, float _y)
	: x(_x)
	, y(_y)
{}

inline constexpr Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

inline constexpr Vector2 Vector2::operator*(float _scalar) const
{
	return Vector2(x * _scalar, y * _scalar);
}

inline constexpr Vector2 Vector2::operator/(float _scalar) const
{
	// 0으로 나누는 경우
	assert(_scalar);

	return Vector2(x / _scalar, y / _scalar);
}

inline constexpr Vector2 Vector2::operator*(const Vector2& _other) const
{
	return Vector2(x * _other.x, y * _other.y);
}

inline constexpr Vector2 Vector2::operator+(const Vector2& _other) const
{
	return Vector2(x + _other.x, y + _other.y);
}

inline constexpr Vector2 Vector2::operator-(const Vector2& _other) const
{

	return Vector2(x - _other.x, y - _other.y);
}

inline constexpr Vector2& Vector2::operator=(const PathPoint& _other)
{
	x = (float)_other.x;
	y = (float)_other.y;

	return *this;
}

inline constexpr Vector2& Vector2::operator*=(float _scale)
{
	x *= _scale;
	y *= _scale;

	return *this;
}

inline constexpr Vector2& Vector2::operator/=(float _scale)
{
	x /= _scale;
	y /= _scale;

	return *this;
}

inline constexpr Vector2& Vector2::operator+=(const Vector2& _other)
{
	x += _other.x;
	y += _other.y;

	return *this;
}

inline constexpr Vector2& Vector2::operator-=(const Vector2& _other)
{
	x -= _other.x;
	y -= _other.y;

	return *this;
}

inline constexpr bool Vector2::operator==(const Vector2& _other)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return (x == _other.x && y == _other.y);
}

inline constexpr bool Vector2::operator!=(const Vector2& _other)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return (x != _other.x || y != _other.y);
}

inline float Vector2::Size() const
{
	return sqrtf(SizeSquared());
}

inline constexpr float Vector2::SizeSquared() const
{
	return x * x + y * y;
}

inline float Vector2::Angle() const
{
	return atan2f(y, x);
}


inline Vector2 Vector2::ToPolarCoordinate() const
{
	return Vector2(Size(), Angle());
}

inline Vector2 Vector2::ToCartesianCoordinate() const
{
	// 극좌표계에서는 x는 반지름 y는 각(rad)으로 활용
	float sinValue = sin(y);
	float cosValue = cos(y);

	return Vector2(x * sinValue, x * cosValue);
}

