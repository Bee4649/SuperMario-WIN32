#include <iostream>
#include <GameEnginePlatform/GameEngineWindow.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
    

	GameEngineWindow::MainWindow.Open("MainWindow", hInstance);

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        
    }

    return (int)msg.wParam;

	return 0;
}