#include "GameEngineWindow.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <iostream>
#include <string>


HINSTANCE GameEngineWindow::Instance = nullptr;
GameEngineWindow GameEngineWindow::MainWindow;

GameEngineWindow::GameEngineWindow()
{
}

GameEngineWindow::~GameEngineWindow()
{
    
    
}


void GameEngineWindow::Open(const std::string& _Title, HINSTANCE _hInstance)
{
    Instance = _hInstance;
    Title = _Title;

    if (nullptr == Instance)
    {
        MsgBoxAssert("HInstance없이 윈도우를 만들수는 없습니다.");
        return;
    }

    MyRegisterClass();
    InitInstance();
}

void GameEngineWindow::InitInstance()
{
    
    // 윈도우 만드는 함수인
    // const char* == std::string
    hWnd = CreateWindowA("DefalutWindow", Title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, Instance, nullptr);


    if (!hWnd)
    {
        MsgBoxAssert("윈도우 생성에 실패했습니다.")
        return;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
 

}


LRESULT CALLBACK GameEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // HWND hWnd <= 어떤 윈도우에 메세지가 왔는지 알려주는 용도입니다.

    // message 윈도우에서 벌어진 일의 종류
    // ex) 윈도우가 선택됐어
    // ex) 윈도우가 크기가 바뀌었어. // wParam lParam <= 크기 

    switch (message)
    {
   
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        //GetMessage함수에서 0을 리턴하게 만든다.
        PostQuitMessage(0);
        break;
    default:
        // 니가 처리할수 없으면 기본적으로 우리가 처리하는 방식으로 처리해줄께.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void GameEngineWindow::MyRegisterClass()
{

    static bool Check = false;

    if (true == Check)
    {
        return;
    }

    WNDCLASSEXA wcex;

    // 크기
    wcex.cbSize = sizeof(WNDCLASSEX);

    // 크기바꾸면 다시 그려라 
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // LRESULT(CALLBACK* WNDPROC)(HWND, unsigned int, unsigned int, unsigned int);
    
    // 윈도우에 무슨일이 생기면 어떻게 해야해?
    // UI에서 사용한다고 했죠? 함수포인터.
    // 내가 너대신 그 행동(함수)을 해줄께.
    wcex.lpfnWndProc = GameEngineWindow::WndProc;

    // 모름 0이면 기본설정
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // 이 윈도우 클래스를 등록하려는 프로그램이 나야.
    wcex.hInstance = Instance;

    // 아이콘  윈도우 기본아이콘
    wcex.hIcon = nullptr;

    // 커서를 정합니다.
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);

    // 윈도우 색깔
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = nullptr;
    // wcex.lpszMenuName = nullptr;

    // 이 형식의 이름은 "AAAAAA" 입니다
    // 앞으로 제가 윈도우 만들때 선택할수 있는 형식중에 1가지로 "AAAAAA"를
    // 등록해주세요.
    wcex.lpszClassName ="DefalutWindow";

    wcex.hIconSm = nullptr;

    // "AAAAA" 윈도우 형식은 메뉴를 사용하지 않아.
    // 윈도우 형식을 등록하는 함수
    
    if(false == RegisterClassExA(&wcex))
    {
        MsgBoxAssert("윈도우 클래스 등록에 실패했습니다.");
        return;
    }

    Check = true;
}

