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
        MsgBoxAssert("HInstance���� �����츦 ������� �����ϴ�.");
        return;
    }

    MyRegisterClass();
    InitInstance();
}

void GameEngineWindow::InitInstance()
{
    
    // ������ ����� �Լ���
    // const char* == std::string
    hWnd = CreateWindowA("DefalutWindow", Title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, Instance, nullptr);


    if (!hWnd)
    {
        MsgBoxAssert("������ ������ �����߽��ϴ�.")
        return;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
 

}


LRESULT CALLBACK GameEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // HWND hWnd <= � �����쿡 �޼����� �Դ��� �˷��ִ� �뵵�Դϴ�.

    // message �����쿡�� ������ ���� ����
    // ex) �����찡 ���õƾ�
    // ex) �����찡 ũ�Ⱑ �ٲ����. // wParam lParam <= ũ�� 

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
        //GetMessage�Լ����� 0�� �����ϰ� �����.
        PostQuitMessage(0);
        break;
    default:
        // �ϰ� ó���Ҽ� ������ �⺻������ �츮�� ó���ϴ� ������� ó�����ٲ�.
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

    // ũ��
    wcex.cbSize = sizeof(WNDCLASSEX);

    // ũ��ٲٸ� �ٽ� �׷��� 
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // LRESULT(CALLBACK* WNDPROC)(HWND, unsigned int, unsigned int, unsigned int);
    
    // �����쿡 �������� ����� ��� �ؾ���?
    // UI���� ����Ѵٰ� ����? �Լ�������.
    // ���� �ʴ�� �� �ൿ(�Լ�)�� ���ٲ�.
    wcex.lpfnWndProc = GameEngineWindow::WndProc;

    // �� 0�̸� �⺻����
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    // �� ������ Ŭ������ ����Ϸ��� ���α׷��� ����.
    wcex.hInstance = Instance;

    // ������  ������ �⺻������
    wcex.hIcon = nullptr;

    // Ŀ���� ���մϴ�.
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);

    // ������ ����
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = nullptr;
    // wcex.lpszMenuName = nullptr;

    // �� ������ �̸��� "AAAAAA" �Դϴ�
    // ������ ���� ������ ���鶧 �����Ҽ� �ִ� �����߿� 1������ "AAAAAA"��
    // ������ּ���.
    wcex.lpszClassName ="DefalutWindow";

    wcex.hIconSm = nullptr;

    // "AAAAA" ������ ������ �޴��� ������� �ʾ�.
    // ������ ������ ����ϴ� �Լ�
    
    if(false == RegisterClassExA(&wcex))
    {
        MsgBoxAssert("������ Ŭ���� ��Ͽ� �����߽��ϴ�.");
        return;
    }

    Check = true;
}

