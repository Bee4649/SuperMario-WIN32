#pragma once
#include "GameEngineDebug.h"
#include <math.h>
#include <cmath>
#include <string>
#include <Windows.h>
#include <vector>

// final 더이상 상속내릴지 못한다.
// 상속도 못하고 만들지도 못하게 만드는 상태

class GameEngineMath final
{
public:
	static std::vector<unsigned int> GetDigits(int _Value);
	static unsigned int GetLenth(int _Value);
	static const float PI;
	static const float PI2;
	static const float DegressToRadians;
	static const float RadiansToDegress;

private:
	virtual ~GameEngineMath() = 0;
};

class float4
{
public:
	static const float4 ZERO;
	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 UP;
	static const float4 DOWN;
	static const float4 FORWARD;
	static const float4 BACK;
	static const float4 Null;


	static float4 AngleToDirection2DToDeg(float _Deg)
	{
		return AngleToDirection2DToRad(_Deg * GameEngineMath::DegressToRadians);
	}

	static float4 AngleToDirection2DToRad(float _Rad)
	{
		return float4(cosf(_Rad), sinf(_Rad), 0.0f, 1.0f);
	}

	// 실수는 기본적으로 == 이 거의 불가능하다. 
	// 해도 정확하지 않는다. 실수를 처리하는 방식이 애초에 정확하지 않기 때문이다.
	// 부동소수점 계산방식은 기본적으로 오차를 가지고 있고
	// + - 등을 할때 여러분들의 생각과는 다른 값이 존재할 가능성이 높다. 
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 1.0f;

	int ix() const
	{
		return static_cast<int>(X);
	}

	int iy() const
	{
		return static_cast<int>(Y);
	}

	int iz() const
	{
		return static_cast<int>(Z);
	}

	int iw() const
	{
		return static_cast<int>(W);
	}

	int hix() const
	{
		return static_cast<int>(X * 0.5f);
	}

	int hiy() const
	{
		return static_cast<int>(Y * 0.5f);
	}

	int hiz() const
	{
		return static_cast<int>(Z * 0.5f);
	}

	int hiw() const
	{
		return static_cast<int>(W * 0.5f);
	}


	float hx() const
	{
		return X * 0.5f;
	}

	float hy() const
	{
		return Y * 0.5f;
	}

	float hz() const
	{
		return X * 0.5f;
	}

	float hw() const
	{
		return W * 0.5f;
	}


	float GetAnagleDegress()
	{
		return GetAnagleRadians() * GameEngineMath::RadiansToDegress;
	}

	void RotaitonZDegress(float _Deg)
	{
		RotaitonZRadians(_Deg * GameEngineMath::DegressToRadians);
	}

	void RotaitonZRadians(float _Rad)
	{
		float4 Copy = *this;
		X = Copy.X * cosf(_Rad) - Copy.Y * sinf(_Rad);
		Y = Copy.X * sinf(_Rad) + Copy.Y * cosf(_Rad);
	}

	float4 RotaitonZDegReturn(float _Deg)
	{
		float4 Copy = *this;
		Copy.RotaitonZDegress(_Deg);
		return Copy;
	}


	float GetAnagleRadians()
	{
		float4 AngleCheck = (*this);
		AngleCheck.Normalize();
		float Result = acosf(AngleCheck.X);

		if (AngleCheck.Y > 0)
		{
			Result = GameEngineMath::PI2 - Result;
		}
		return Result;

	}

	POINT ToWindowPOINT()
	{
		return POINT(ix(), iy());
	}

	float4 half() const
	{
		return { X * 0.5f,Y * 0.5f,Z * 0.5f,W };
	}

	bool IsZero() const
	{
		return X == 0.0f && Y == 0.0f && Z == 0.0f;
	}

	float Size() const
	{
		return sqrtf(X * X + Y * Y);
	}

	void Normalize()
	{
		// 길이를 1로 만드는 함수입니다.
		float Len = Size();

		X /= Len;
		Y /= Len;
		Z /= Len;
	}

	float4 NormalizeReturn()
	{
		float4 Result = *this;
		Result.Normalize();
		return Result;
	}

	static float4 Lerp(const float4& Start, const float4& End, float Ratio)
	{
		// 1.5 + 0.5 * 2.5;
		return Start * (1.0f - Ratio) + (End * Ratio);
	}

	static float4 LerpClamp(const float4& Start, const float4& End, float Ratio)
	{
		if (0 >= Ratio)
		{
			Ratio = 0.0f;
		}

		if (1.0f <= Ratio)
		{
			Ratio = 1.0f;
		}

		return Lerp(Start, End, Ratio);
	}


	float4 operator *(const float _Value) const
	{
		float4 Return;
		Return.X = X * _Value;
		Return.Y = Y * _Value;
		Return.Z = Z * _Value;
		return Return;
	}



	float4 operator +(const float4 _Value) const
	{
		float4 Return;
		Return.X = X + _Value.X;
		Return.Y = Y + _Value.Y;
		Return.Z = Z + _Value.Z;
		return Return;
	}

	float4 operator -(const float4 _Value) const
	{
		float4 Return;
		Return.X = X - _Value.X;
		Return.Y = Y - _Value.Y;
		Return.Z = Z - _Value.Z;
		return Return;
	}

	float4 operator *(const float4 _Value) const
	{
		float4 Return;
		Return.X = X * _Value.X;
		Return.Y = Y * _Value.Y;
		Return.Z = Z * _Value.Z;
		return Return;
	}

	float4 operator /(const float4 _Value) const
	{
		float4 Return;
		Return.X = X / _Value.X;
		Return.Y = Y / _Value.Y;
		Return.Z = Z / _Value.Z;
		return Return;
	}

	float4 operator -() const
	{
		return { -X, -Y, -Z, 1.0f };
	}

	float4& operator +=(const float4& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;
		return *this;
	}

	float4& operator *=(const float& _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;
		return *this;
	}


	float4& operator *=(const float4& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;
		return *this;
	}

	float4& operator -=(const float4& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;
		return *this;
	}

	float4& operator /=(const float4& _Other)
	{
		X /= _Other.X;
		Y /= _Other.Y;
		Z /= _Other.Z;
		return *this;
	}

	std::string ToString()
	{
		char ArrReturn[256];

		sprintf_s(ArrReturn, "x: %f, y: %f, z: %f, w: %f", X, Y, Z, W);

		return std::string(ArrReturn);
	}

};

class CollisionData
{
public:
	float4 Position;
	float4 Scale; // x만 원의 반지름으로 보겠습니다.

	float Left() const
	{
		return Position.X - Scale.hx();
	}
	float Right() const
	{
		return Position.X + Scale.hx();
	}
	float Top() const
	{
		return Position.Y - Scale.hy();
	}
	float Bot() const
	{
		return Position.Y + Scale.hy();
	}

	float4 LeftTop() const
	{
		return float4{ Left(), Top() };
	}
	float4 RightTop() const
	{
		return float4{ Right(), Top() };
	}
	float4 LeftBot() const
	{
		return float4{ Left(), Bot() };
	}
	float4 RightBot() const
	{
		return float4{ Right(), Bot() };
	}
};




