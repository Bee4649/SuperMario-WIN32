#pragma once
#include <Windows.h>
#include <string>
#include "GameEngineWindowTexture.h"
#include <GameEngineBase/GameEngineMath.h>

// 설명 :
class GameEngineWindowTexture;
class GameEngineWindow
{
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

public:
	static GameEngineWindow MainWindow;
	
	// 윈도우를 만들어 주는 기능
	static void WindowCreate(HINSTANCE _hInstance, const std::string_view& _TitleName, float4 _Size, float4 _Pos);

	static void SettingWindowSize(float4 _Size);
	static void SettingWindowPos(float4 _Pos);

	static float4 GetScreenSize()
	{
		return ScreenSize;
	}

	static HWND GetHWnd()
	{
		return HWnd;
	}

	static HDC GetWindowBackBufferHdc()
	{
		return WindowBackBufferHdc;
	}

	static GameEngineWindowTexture* GetDoubleBufferImage()
	{
		return DoubleBufferImage;
	}

	static void AppOff()
	{
		IsWindowUpdate = false;
	}

	static void DoubleBufferClear();
	static void DoubleBufferRender();

	static int MessageLoop(void(*Start)(), void(*Loop)(), void(*End)());

	static float4 GetMousePos();

	// constrcuter destructer
	GameEngineWindow();
	~GameEngineWindow();

	// delete Function
	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;

protected:

private:
	static bool IsWindowUpdate;
	static float4 WindowSize;
	static float4 ScreenSize;
	static float4 WindowPos;
	static HWND HWnd;
	static HDC WindowBackBufferHdc;
	static GameEngineWindowTexture* BackBufferImage;
	static GameEngineWindowTexture* DoubleBufferImage;
};

