#include "GameEngineRandom.h"
#include <iostream>
#include <chrono>

GameEngineRandom GameEngineRandom::MainRandom;

GameEngineRandom::GameEngineRandom()
	: MtRand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
{
	// chrono는 std에서 제공하는 시간을 재기위한 함수임.
}

GameEngineRandom::~GameEngineRandom()
{
}
