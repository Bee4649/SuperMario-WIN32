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

	float Update();

protected:

private:
	LARGE_INTEGER Count = LARGE_INTEGER(0);
	LARGE_INTEGER Cur = LARGE_INTEGER(0);
	LARGE_INTEGER Prev = LARGE_INTEGER(0);
	__int64 Tick;
	double DoubleDelta;
	float FloatDelta;
};

