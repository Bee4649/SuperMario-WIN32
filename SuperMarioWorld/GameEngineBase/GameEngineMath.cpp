#include "GameEngineMath.h"
#include "GameEngineString.h"

// 변수명은 숫자가 앞에 올 수가 없다.

const float GameEngineMath::PI = 3.14159f;
const float GameEngineMath::PI2 = PI * 2.0f;
const float GameEngineMath::DegressToRadians = GameEngineMath::PI / 180;
const float GameEngineMath::RadiansToDegress = 180 / GameEngineMath::PI;

const float4 float4::ZERO = { 0.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::LEFT = { -1.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::UP = { 0.0f, -1.0f, 0.0f, 0.0f };
const float4 float4::DOWN = { 0.0f, 1.0f, 0.0f, 0.0f };

// 123121 [1][2][3][1][2][1]
std::vector<unsigned int> GameEngineMath::GetDigits(int _Value)
{

	std::vector<unsigned int> ResultValue = std::vector<unsigned int>();

	std::string StringValue = GameEngineString::ToString(_Value);

	int StartIndex = 0;

	if (0 != StringValue.size() && StringValue[0] == '-')
	{
		StartIndex = 1;
	}

	ResultValue.reserve(StringValue.size());

	for (size_t i = StartIndex; i < StringValue.size(); i++)
	{
		ResultValue.push_back(StringValue[i] - '0');
	}

	return ResultValue;
}

unsigned int GameEngineMath::GetLenth(int _Value)
{
	std::vector<unsigned int> ResultValue = std::vector<unsigned int>();

	std::string StringValue = GameEngineString::ToString(_Value);

	int StartIndex = 0;

	if (0 != StringValue.size() && StringValue[0] == '-')
	{
		StartIndex = 1;
	}

	return static_cast<unsigned int>(StringValue.size()) - StartIndex;
}