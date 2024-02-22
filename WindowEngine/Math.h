#pragma once


struct Math
{
	/// static ¸â¹ö

	static constexpr float PI = 3.14159265358979323846f;
	static constexpr float TwoPI = 2.f * PI;
	static constexpr float HalfPI = 1.57079632679f;
	static constexpr float InvPI = 0.31830988618f;
	

	inline static constexpr float Deg2Rad(float _degree)
	{
		return _degree * PI / 180.f;
	}

	inline static constexpr float Rad2Deg(float _radian)
	{
		return _radian * 180.f * InvPI;
	}

	template<typename T>
	inline static constexpr T Max(const T A, const T B)
	{
		return (A >= B) ? A : B;
	}

	template<typename T>
	inline static constexpr T Min(const T A, const T B)
	{
		return (A <= B) ? A : B;
	}

	template<typename T>
	inline static constexpr T Abs(const T A)
	{
		return (A >= (T)0) ? A : -A;
	}

	template< typename T >
	inline static T constexpr Max3(const T A, const T B, const T C)
	{
		return Max(Max(A, B), C);
	}

	template< typename T >
	inline static T constexpr Min3(const T A, const T B, const T C)
	{
		return Min(Min(A, B), C);
	}

	template< typename T>
	inline static constexpr T Clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}


};