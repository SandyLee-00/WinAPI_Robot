#pragma once

#include "PathPoint.h"

struct Vector2
{
public:
	/// ������
	inline Vector2() = default;
	inline explicit constexpr Vector2(int _x, int _y);
	inline explicit constexpr Vector2(float _x, float _y);

	/// ������
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

	/// ��� �Լ�

	// ��Ʈ ������ (����)
	inline float Size() const;

	// ������ ��ȯ
	inline constexpr float SizeSquared() const;

	// ���� ����ȭ
	void Normalize();
	[[nodiscard]] Vector2 GetNormalize() const;

	// ��ȯ���� radian
	inline float Angle() const;

	// ������ǥ�� -> ����ǥ�� ��ȯ
	inline Vector2 ToPolarCoordinate() const;

	// ����ǥ�� -> ���� ��ǥ�� ��ȯ
	inline Vector2 ToCartesianCoordinate() const;

	string ToString() const;

	// return normal
	[[nodiscard]] Vector2 GetNormalize2();

	// return dir
	[[nodiscard]] Vector2 GetPlayerDir();

	// return bossDir
	[[nodiscard]] Vector2 GetBossDir();

	// ȸ����ȯ
	static Vector2 RotateRadian(const Vector2& _point, const Vector2& _center, float _radian);

	/// ������� ����
	static const Vector2 UnitX; // ǥ�ر�������
	static const Vector2 UnitY; // ǥ�ر�������
	static const Vector2 One;
	static const Vector2 Zero;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Up;
	static const Vector2 Down;
	static constexpr int Dimension = 2;


	/// ��� ����
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
	// 0���� ������ ���
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
	// TODO: ���⿡ return ���� �����մϴ�.
	return (x == _other.x && y == _other.y);
}

inline constexpr bool Vector2::operator!=(const Vector2& _other)
{
	// TODO: ���⿡ return ���� �����մϴ�.
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
	// ����ǥ�迡���� x�� ������ y�� ��(rad)���� Ȱ��
	float sinValue = sin(y);
	float cosValue = cos(y);

	return Vector2(x * sinValue, x * cosValue);
}

