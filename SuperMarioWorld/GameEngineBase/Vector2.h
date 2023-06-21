#pragma once

#include <math.h>

const float	PI = 3.14159f;

// 180도 = PI

float RadianToDegree(float Radian)
{

	return Radian / PI * 180.f;
}

float DegreeToRadian(float Degree)
{
	return Degree / 180.f * PI;
}


struct Vector2
{
	float	x, y;

	Vector2() :
		x(0.f),
		y(0.f)
	{
	}

	Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	Vector2(const Vector2& v) :
		x(v.x),
		y(v.y)
	{
	}

	void operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	void operator = (float f)
	{
		x = f;
		y = f;
	}

	bool operator == (const Vector2& v)	const
	{
		return x == v.x && y == v.y;
	}

	bool operator != (const Vector2& v)	const
	{
		return x != v.x || y != v.y;
	}

	// +
	Vector2 operator + (const Vector2& v)	const
	{
		Vector2	result;

		result.x = x + v.x;
		result.y = y + v.y;

		return result;
	}

	Vector2 operator + (float f)	const
	{
		Vector2	result;

		result.x = x + f;
		result.y = y + f;

		return result;
	}

	Vector2 operator + (int i)	const
	{
		Vector2	result;

		result.x = x + (float)i;
		result.y = y + (float)i;

		return result;
	}

	// +=
	void operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator += (float f)
	{
		x += f;
		y += f;
	}

	void operator += (int i)
	{
		x += (float)i;
		y += (float)i;
	}

	// -
	Vector2 operator - (const Vector2& v)	const
	{
		Vector2	result;

		result.x = x - v.x;
		result.y = y - v.y;

		return result;
	}

	Vector2 operator - (float f)	const
	{
		Vector2	result;

		result.x = x - f;
		result.y = y - f;

		return result;
	}

	Vector2 operator - (int i)	const
	{
		Vector2	result;

		result.x = x - (float)i;
		result.y = y - (float)i;

		return result;
	}

	// -=
	void operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	void operator -= (float f)
	{
		x -= f;
		y -= f;
	}

	void operator -= (int i)
	{
		x -= (float)i;
		y -= (float)i;
	}

	// *
	Vector2 operator * (const Vector2& v)	const
	{
		Vector2	result;

		result.x = x * v.x;
		result.y = y * v.y;

		return result;
	}

	Vector2 operator * (float f)	const
	{
		Vector2	result;

		result.x = x * f;
		result.y = y * f;

		return result;
	}

	Vector2 operator * (int i)	const
	{
		Vector2	result;

		result.x = x * (float)i;
		result.y = y * (float)i;

		return result;
	}

	// *=
	void operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
	}

	void operator *= (float f)
	{
		x *= f;
		y *= f;
	}

	void operator *= (int i)
	{
		x *= (float)i;
		y *= (float)i;
	}

	// /
	Vector2 operator / (const Vector2& v)	const
	{
		Vector2	result;

		result.x = x / v.x;
		result.y = y / v.y;

		return result;
	}

	Vector2 operator / (float f)	const
	{
		Vector2	result;

		result.x = x / f;
		result.y = y / f;

		return result;
	}

	Vector2 operator / (int i)	const
	{
		Vector2	result;

		result.x = x / (float)i;
		result.y = y / (float)i;

		return result;
	}

	// /=
	void operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
	}

	void operator /= (float f)
	{
		x /= f;
		y /= f;
	}

	void operator /= (int i)
	{
		x /= (float)i;
		y /= (float)i;
	}

	float Angle(const Vector2& v)	const
	{
		// 너비 길이
		float Width = v.x - x;
		// 높이 길이
		float Height = v.y - y;

		// sqrtf : 루트 
		// 빗변의 길이
		float c = sqrtf(Width * Width + Height * Height);

		// 코사인 세타  값 :밑변값과 빗변값으로 이용해서 코사인값을 구하고
		float Angle = Width / c;
		// 세타가 나온다(아크코사인)
		Angle = acosf(Angle) / PI * 180.f;

		if (v.y < y)
			Angle = 360.f - Angle;

		return Angle;
	}
};
