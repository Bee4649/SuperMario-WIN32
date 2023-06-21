#include "GameEngineTime.h"
#include <Windows.h>

GameEngineTime GameEngineTime::MainTimer;

GameEngineTime::GameEngineTime()
{
	// 1�� ���������� ���� ����
	// ������ �� 1���� ���ϸ� ��.
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

// ������ ���̿��� ����Ǿ���� �Լ����� �� ������Ʈ��� �̸��� ���� ���̴�.
float GameEngineTime::TimeCheck()
{
	// �� �Լ��� ����ɶ����� �ɸ� �ð�...
	QueryPerformanceCounter(&Cur);

	Tick = Cur.QuadPart - Prev.QuadPart;

	// ���� �ð�
	// 8����Ʈ �Ǽ���          200���̰�               100                        /             �����ִ� �ð�
	DoubleDelta = static_cast<double>(Cur.QuadPart) - static_cast<double>(Prev.QuadPart) / static_cast<double>(Count.QuadPart);

	Prev.QuadPart = Cur.QuadPart;

	FloatDelta = static_cast<float>(DoubleDelta);

	return FloatDelta;
}