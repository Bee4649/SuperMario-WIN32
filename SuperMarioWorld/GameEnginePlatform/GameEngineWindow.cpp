#include "GameEngineWindow.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <iostream>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include "GameEngineInput.h"
#include <GameEngineContents/IconRes.h>

HWND GameEngineWindow::HWnd = nullptr;
HDC  GameEngineWindow::WindowBackBufferHdc = nullptr;

float4 GameEngineWindow::WindowSize = { 800, 600 };
float4 GameEngineWindow::WindowPos = { 100, 100 };
float4 GameEngineWindow::ScreenSize = { 800, 600 };

GameEngineWindowTexture* GameEngineWindow::BackBufferImage = nullptr;
GameEngineWindowTexture* GameEngineWindow::DoubleBufferImage = nullptr;

bool GameEngineWindow::IsWindowUpdate = true;


GameEngineWindow::GameEngineWindow()
{
}

GameEngineWindow::~GameEngineWindow()
{
}

void GameEngineWindow::DoubleBufferClear()
{
	DoubleBufferImage->TextureClear();
}

void GameEngineWindow::DoubleBufferRender()
{
	BackBufferImage->BitCopy(DoubleBufferImage, WindowSize.half(), WindowSize);
}

void GameEngineWindow::WindowCreate(HINSTANCE _hInstance, const std::string_view& _TitleName, float4 _Size, float4 _Pos)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &GameEngineWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ��� 
	wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = "GameEngineWindowDefault";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (0 == RegisterClassEx(&wcex))
	{
		MsgAssert("������ Ŭ���� ��Ͽ� �����߽��ϴ�.");
		return;
	}



	HWnd = CreateWindow("GameEngineWindowDefault", _TitleName.data(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInstance, nullptr);

	if (nullptr == HWnd)
	{
		MsgAssert("������ Ŭ���� ������ �����߽��ϴ�.");
		return;
	}

	// �����찡 ��������鼭���� ������� ������ 2�����迭�� ���������� ������ ���̴�.
	WindowBackBufferHdc = GetDC(HWnd);


	ShowWindow(HWnd, SW_SHOW);
	UpdateWindow(HWnd);

	SettingWindowSize(_Size);
	SettingWindowPos(_Pos);

	// ũ�� �ٲٰ� ���´�.
	BackBufferImage = new GameEngineWindowTexture();
	BackBufferImage->TextureCreate(WindowBackBufferHdc);


	return;
}

LRESULT CALLBACK GameEngineWindow::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_MOUSEMOVE:
	{
		int a = 0;
		break;
	}
	case WM_SETFOCUS:
	{
		int a = 0;
		break;
	}
	case WM_ACTIVATE:
	{
		int a = 0;
		break;
	}
	case WM_KILLFOCUS:
	{
		int a = 0;
		break;
	}
	case WM_KEYDOWN:
	{
		GameEngineInput::IsAnyKeyOn();
		break;
	}
	case WM_DESTROY:
	{
		// Message�Լ��� 0�� �����ϰ� ������.
		PostQuitMessage(0);
		IsWindowUpdate = false;
		break;
	}
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}


int GameEngineWindow::MessageLoop(void(*_Start)(), void(*_Loop)(), void(*_End)())
{
	// �����찡 �߱����� �ε��ؾ��� �̹����� ���� ����� ó���ϴ� �ܰ�
	if (nullptr != _Start)
	{
		_Start();
	}

	MSG msg;

	while (IsWindowUpdate)
	{
		// �����쿡 �������� �ִ°� �ƴ϶� �޼����� �ֵ� ����
		// �����Լ� _getch()
		// <= Ű�� ���������� �����
		// (�Լ��� ����� ���������� ��ٸ��� �Լ����� ���� �Լ���� �մϴ�).
		// GetMessage�� ���� �Լ�����. �������� �޼����� �߻��Ҷ����� ��ٸ���.
		// �񵿱� �޼��� �Լ��� �ִ�. 
		// PeekMessage�� ������ �޼����� ������ 0�� ���ϵǰ� �׳� �����մϴ�.

		// �����Ӱ� ����Ÿ���� �ϼ��ƴ�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// �޼����� �������� ������ ����
			if (nullptr != _Loop)
			{
				_Loop();
			}

			GameEngineInput::IsAnyKeyOff();
			continue;
		}

		// ������ �޼����� ���� �ð��� ����Ÿ���̶�� �մϴ�.
		// ������ ����Ÿ�ӿ� ���ư��°� �����̴�.
		// �����߿� 2���� ������ �ִ�. �����츦 �����̰ų� ũ�⸦ ���̸�
		// ȭ���� �����ϴ� ����. 
		// ���� �׷� ����c�� �޼����� �߻���Ű�� ���߿��� ������ ��� ���ư��� �����ִ�.

		// �̰� �ѹٲ ���� ���� ������
		// FPS
		// �ʴ� ȭ���� �׷����� Ƚ��
		// �ϵ����͵� ������ �ִ�.

		// ����Ÿ��
		// ����Ÿ�ӿ� ������ �����ϴ� ��.
		if (nullptr != _Loop)
		{
			_Loop();
		}

		GameEngineInput::IsAnyKeyOff();

	}


	if (nullptr != _End)
	{
		_End();
	}

	if (nullptr != BackBufferImage)
	{
		delete DoubleBufferImage;
		DoubleBufferImage = nullptr;

		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	return (int)msg.wParam;
}

void GameEngineWindow::SettingWindowSize(float4 _Size)
{
	// Window���� LP �����Ͷ�� �� Long Pointer
	
	// Ÿ��Ʋ���� �����ӱ��� ����ؼ� ũ�⸦ �����ؾ� ��. 
	
	//          ��ġ    ũ�� 200        200
	RECT Rc = { 0, 0, _Size.iX(), _Size.iY() };

	ScreenSize = _Size;

	// ���� ���ϴ� ũ�⸦ ������ Ÿ��Ʋ�ٱ��� ����� ũ�⸦ �����ִ� �Լ�.
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW,FALSE);

	WindowSize = { static_cast<float>(Rc.right - Rc.left), static_cast<float>(Rc.bottom - Rc.top) };
	
	// 0�� �־��ָ� ������ ũ�⸦ �����Ѵ�.
	SetWindowPos(HWnd, nullptr, WindowPos.iX(), WindowPos.iY(), WindowSize.iX(), WindowSize.iY(), SWP_NOZORDER);

	// ������ �Ȱ��� ũ���� �̹���.

	if (nullptr != DoubleBufferImage)
	{
		delete DoubleBufferImage;
		DoubleBufferImage = nullptr;
	}

	DoubleBufferImage = new GameEngineWindowTexture();
	DoubleBufferImage->TextureCreate(ScreenSize);
}

void GameEngineWindow::SettingWindowPos(float4 _Pos)
{
	WindowPos = _Pos;
	SetWindowPos(HWnd, nullptr, WindowPos.iX(), WindowPos.iY(), WindowSize.iX(), WindowSize.iY(), SWP_NOZORDER);
}

float4 GameEngineWindow::GetMousePos()
{
	POINT MoniterPoint;
	LPPOINT PointPtr = &MoniterPoint;
	GetCursorPos(&MoniterPoint);
	ScreenToClient(HWnd, &MoniterPoint);

	return float4{ static_cast<float>(MoniterPoint.x), static_cast<float>(MoniterPoint.y) };
}