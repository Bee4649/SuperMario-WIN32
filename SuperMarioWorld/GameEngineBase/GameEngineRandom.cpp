#include "GameEngineRandom.h"
#include <iostream>
#include <chrono>

GameEngineRandom GameEngineRandom::MainRandom;

GameEngineRandom::GameEngineRandom()
	: MtRand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
{
	// chrono�� std���� �����ϴ� �ð��� ������� �Լ���.
}

GameEngineRandom::~GameEngineRandom()
{
}
