#pragma once
#include "GameEngineDebug.h"
#include <math.h>
#include <cmath>
#include <string>
#include <Windows.h>
#include <vector>

// final ���̻� ��ӳ����� ���Ѵ�.
// ��ӵ� ���ϰ� �������� ���ϰ� ����� ����

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


	static float4 AngleToDirection2DToDeg(float _Deg)
	{
		return AngleToDirection2DToRad(_Deg * GameEngineMath::DegressToRadians);
	}

	static float4 AngleToDirection2DToRad(float _Rad)
	{
		return float4(cosf(_Rad), sinf(_Rad), 0.0f, 1.0f);
	}

	// �Ǽ��� �⺻������ == �� ���� �Ұ����ϴ�. 
	// �ص� ��Ȯ���� �ʴ´�. �Ǽ��� ó���ϴ� ����� ���ʿ� ��Ȯ���� �ʱ� �����̴�.
	// �ε��Ҽ��� ������� �⺻������ ������ ������ �ְ�
	// + - ���� �Ҷ� �����е��� �������� �ٸ� ���� ������ ���ɼ��� ����. 
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float W = 1.0f;

	inline int iX() const
	{
		return static_cast<int>(X);
	}

	inline int iY() const
	{
		return static_cast<int>(Y);
	}
	inline int iZ() const
	{
		return static_cast<int>(Z);
	}

	inline int iW() const
	{
		return static_cast<int>(W);
	}

	inline float hX() const
	{
		return X * 0.5f;
	}

	inline float hY() const
	{
		return Y * 0.5f;
	}

	float hZ() const
	{
		return Z * 0.5f;
	}

	float hW() const
	{
		return W * 0.5f;
	}


	inline int ihX() const
	{
		return static_cast<int>(hX());
	}

	inline int ihY() const
	{
		return static_cast<int>(hY());
	}

	inline int ihZ() const
	{
		return static_cast<int>(hZ());
	}

	inline int ihW() const
	{
		return static_cast<int>(hW());
	}

	inline float4 Half() const
	{
		return { hX(), hY(), Z, W };
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

	float4 operator-() const
	{
		float4 ReturnValue = *this;

		ReturnValue.X = -ReturnValue.X;
		ReturnValue.Y = -ReturnValue.Y;
		ReturnValue.Z = -ReturnValue.Z;
		return ReturnValue;
		// return { -X, -Y, -Z, 1.0f };
	}

	float4 operator-(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X - _Other.X;
		ReturnValue.Y = Y - _Other.Y;
		ReturnValue.Z = Z - _Other.Z;

		return ReturnValue;
	}



	float4 operator+(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X + _Other.X;
		ReturnValue.Y = Y + _Other.Y;
		ReturnValue.Z = Z + _Other.Z;

		return ReturnValue;
	}

	float4 operator*(const float4& _Other) const
	{
		float4 ReturnValue;

		ReturnValue.X = X * _Other.X;
		ReturnValue.Y = Y * _Other.Y;
		ReturnValue.Z = Z * _Other.Z;

		return ReturnValue;
	}


	float4 operator*(const float _Value) const
	{
		float4 ReturnValue;

		ReturnValue.X = X * _Value;
		ReturnValue.Y = Y * _Value;
		ReturnValue.Z = Z * _Value;

		return ReturnValue;
	}

	float4& operator+=(const float4& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;

		return *this;
	}


	float4& operator-=(const float4& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;

		return *this;
	}

	float4& operator*=(const float4& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;

		return *this;
	}

	float4& operator*=(const float _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;

		return *this;
	}

	bool operator==(const float4 _Value) const
	{
		return X == _Value.X &&
			Y == _Value.Y &&
			Z == _Value.Z;
	}

	inline void Normalize()
	{
		// ���̸� 1�� ����� �Լ��Դϴ�.
		float Len = Size();

		if (0.0f == Len)
		{
			// MsgBoxAssert("0���� �������� �߽��ϴ�.");
			return;
		}

		X /= Len;
		Y /= Len;
		Z /= Len;
	}

	inline float4 NormalizeReturn()
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

	inline float Size()
	{
		float Value = X * X + Y * Y; // == ���� * ����

		// sqrtf : ��Ʈ
		// ������ ����
		// �������̴�.
		// ������ Ǯ� �������̶�� �մϴ�.
		Value; // ���� * ���� => ����

		return sqrtf(Value);
	}

	float Max2D()
	{
		return X > Y ? X : Y;
	}

	POINT ToWindowPOINT()
	{
		return POINT(iX(), iY());
	}

	float4 half() const
	{
		return { X * 0.5f,Y * 0.5f,Z * 0.5f,W };
	}

	bool IsZero() const
	{
		return X == 0.0f && Y == 0.0f && Z == 0.0f;
	}

	std::string ToString()
	{
		char ArrReturn[256];

		sprintf_s(ArrReturn, "X: %f, Y: %f, Z: %f, W: %f", X, Y, Z, W);

		return std::string(ArrReturn);
	}

};

class CollisionData
{
public:
	float4 Position;
	float4 Scale; // x�� ���� ���������� ���ڽ��ϴ�.

	float Left() const
	{
		return Position.X - Scale.hX();
	}
	float Right() const
	{
		return Position.X + Scale.hX();
	}
	float Top() const
	{
		return Position.Y - Scale.hY();
	}
	float Bot() const
	{
		return Position.Y + Scale.hY();
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





