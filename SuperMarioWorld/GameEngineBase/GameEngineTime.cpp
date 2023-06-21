#include "GameEngineTime.h"
#include <Windows.h>

GameEngineTime GameEngineTime::MainTimer;

GameEngineTime::GameEngineTime()
{
	// 1초 기준으로한 정수 단위
	// 시작할 때 1번만 정하면 됨.
	QueryPerformanceFrequency(&Count);
	QueryPerformanceFrequency(&Prev);

}

GameEngineTime::~GameEngineTime()
{
}

void GameEngineTime::Reset()
{
	QueryPerformanceCounter(&Prev);
}

// 프레임 사이에서 실행되어야할 함수들은 다 업데이트라고 이름을 지을 것이다.
float GameEngineTime::TimeCheck()
{
	// 이 함수가 실행될때까지 걸린 시간...
	QueryPerformanceCounter(&Cur);

	Tick = Cur.QuadPart - Prev.QuadPart;

	// 현재 시간
	// 8바이트 실수형          200사이값               100                        /             셀수있는 시간
	DoubleDelta = static_cast<double>(Cur.QuadPart) - static_cast<double>(Prev.QuadPart) / static_cast<double>(Count.QuadPart);

	Prev.QuadPart = Cur.QuadPart;

	FloatDelta = static_cast<float>(DoubleDelta);

	return FloatDelta;
}