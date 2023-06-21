#pragma once
#include <chrono>
#include <Windows.h>

// Ό³Έν :
class GameEngineTime
{
public:
	static GameEngineTime MainTimer;

	// constrcuter destructer
	GameEngineTime();
	~GameEngineTime();

	// delete Function
	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(GameEngineTime&& _Other) noexcept = delete;
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(GameEngineTime&& _Other) noexcept = delete;

	float GetDeltaTime()
	{
		return FloatDelta;
	}

	void Reset();

	float TimeCheck();

protected:

private:
	__int64 Tick = 0;
	LARGE_INTEGER Count = LARGE_INTEGER();
	LARGE_INTEGER Cur = LARGE_INTEGER();
	LARGE_INTEGER Prev = LARGE_INTEGER();
	double DoubleDelta = 0.0f;
	float FloatDelta = 0.0f;
};

