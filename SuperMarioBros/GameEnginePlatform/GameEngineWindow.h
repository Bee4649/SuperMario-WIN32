#pragma once
#include <Windows.h>
#include <string.h>
#include <cstdlib>

class GameEngineWindow
{

public:
	static GameEngineWindow MainWindow;

	// constrcuter destructer
	GameEngineWindow();
	~GameEngineWindow();

	// delete Function
	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;


	void Open(const std::string& _Title, HINSTANCE _hInstance);

protected:

private:

	

	static HINSTANCE Instance;
	std::string Title = "";
	HWND hWnd = nullptr;



	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void InitInstance();
	void MyRegisterClass();

	
};

