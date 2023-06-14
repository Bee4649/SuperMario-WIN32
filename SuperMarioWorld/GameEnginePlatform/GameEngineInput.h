#pragma once
#include <Windows.h>
#include <map>
#include <string>

// ���� :
class GameEngineWindow;
class GameEngineInput
{
	friend GameEngineWindow;

private:
	class GameEngineKey
	{
		friend GameEngineInput;

		bool Down = false; // ���� ����.
		bool Press = false; // ��� ������ �ִ� ����.
		bool Up = false; // �� ����.
		bool Free = true; // �ȴ����� ��.

		// �ǹ̰� ���ٰ� ����. ���ʰ� ������ ��
		float PressTime = 0.0f;

		int Key = -1;

		bool KeyCheck()
		{
			return 0 != GetAsyncKeyState(Key);
		}

		void Update(float _DeltaTime);

	};

public:
	// constrcuter destructer
	GameEngineInput();
	~GameEngineInput();

	// delete Function
	GameEngineInput(const GameEngineInput& _Other) = delete;
	GameEngineInput(GameEngineInput&& _Other) noexcept = delete;
	GameEngineInput& operator=(const GameEngineInput& _Other) = delete;
	GameEngineInput& operator=(GameEngineInput&& _Other) noexcept = delete;

	// static float4 MousePos();

	static void Update(float _DeltaTime);
	static void CreateKey(const std::string_view& _Name, int _Key);
	static bool IsKey(const std::string_view& _Name);

	static bool IsDown(const std::string_view& _Name);
	static bool IsUp(const std::string_view& _Name);
	static bool IsPress(const std::string_view& _Name);
	static bool IsFree(const std::string_view& _Name);
	static float GetPressTime(const std::string_view& _Name);

	static bool IsAnyKey()
	{
		return IsAnyKeyValue;
	}

protected:

private:
	//      PlayerJump       
	static std::map<std::string, GameEngineKey> Keys;
	static bool IsAnyKeyValue;

	static void IsAnyKeyOn()
	{
		IsAnyKeyValue = true;
	}

	static void IsAnyKeyOff()
	{
		IsAnyKeyValue = false;
	}
};

