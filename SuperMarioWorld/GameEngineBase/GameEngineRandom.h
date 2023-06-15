#pragma once
#include <random>

// 설명 :
class GameEngineRandom
{
public:
	static GameEngineRandom MainRandom;

	// constrcuter destructer
	GameEngineRandom();
	~GameEngineRandom();

	// delete Function
	GameEngineRandom(const GameEngineRandom& _Other) = delete;
	GameEngineRandom(GameEngineRandom&& _Other) noexcept = delete;
	GameEngineRandom& operator=(const GameEngineRandom& _Other) = delete;
	GameEngineRandom& operator=(GameEngineRandom&& _Other) noexcept = delete;

	int RandomInt(int _Min, int _Max)
	{

		std::uniform_int_distribution<int> Uniform(_Min, _Max);
		return Uniform(MtRand);
	}

	float RandomFloat(float _Min, float _Max)
	{
		std::uniform_real_distribution <float> Uniform(_Min, _Max);
		return Uniform(MtRand);
	}

	void SetSeed(__int64 _Seed)
	{
		MtRand = std::mt19937_64(_Seed);
	}

protected:

private:

	// 메르센 트위스터 알고리즘을 사용한 제네레이터 
	std::mt19937_64 MtRand = std::mt19937_64();
};

