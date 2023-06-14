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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 흰색 
	wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = "GameEngineWindowDefault";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (0 == RegisterClassEx(&wcex))
	{
		MsgAssert("윈도우 클래스 등록에 실패했습니다.");
		return;
	}



	HWnd = CreateWindow("GameEngineWindowDefault", _TitleName.data(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInstance, nullptr);

	if (nullptr == HWnd)
	{
		MsgAssert("윈도우 클래스 생성에 실패했습니다.");
		return;
	}

	// 윈도우가 만들어지면서부터 만들어진 색깔의 2차원배열의 수정권한을 얻어오는 것이다.
	WindowBackBufferHdc = GetDC(HWnd);


	ShowWindow(HWnd, SW_SHOW);
	UpdateWindow(HWnd);

	SettingWindowSize(_Size);
	SettingWindowPos(_Pos);

	// 크기 바꾸고 얻어온다.
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
		// Message함수가 0을 리턴하게 만들어라.
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
	// 윈도우가 뜨기전에 로딩해야할 이미지나 사운드 등등을 처리하는 단계
	if (nullptr != _Start)
	{
		_Start();
	}

	MSG msg;

	while (IsWindowUpdate)
	{
		// 윈도우에 무슨일이 있는게 아니라 메세지가 있든 없든
		// 동기함수 _getch()
		// <= 키가 눌릴때까지 멈춘다
		// (함수가 제대로 끝날때까지 기다리는 함수들을 동기 함수라고 합니다).
		// GetMessage는 동기 함수에요. 윈도우의 메세지가 발생할때까지 기다린다.
		// 비동기 메세지 함수가 있다. 
		// PeekMessage는 윈도우 메세지가 없으면 0이 리턴되고 그냥 리턴합니다.

		// 프레임과 데드타임이 완성됐다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// 메세지가 있을때도 게임을 실행
			if (nullptr != _Loop)
			{
				_Loop();
			}

			GameEngineInput::IsAnyKeyOff();
			continue;
		}

		// 윈도우 메세지가 없는 시간을 데드타임이라고 합니다.
		// 게임은 데드타임에 돌아가는게 보통이다.
		// 게임중에 2가지 종류가 있다. 윈도우를 움직이거나 크기를 줄이면
		// 화면이 정지하는 게임. 
		// 내가 그런 윈도c우 메세지를 발생시키는 와중에도 게임은 계속 돌아가는 게임있다.

		// 이게 한바뀌가 도는 것을 프레임
		// FPS
		// 초당 화면이 그려지는 횟수
		// 하드웨어와도 연결이 있다.

		// 데드타임
		// 데드타임에 게임을 실행하는 것.
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
	// Window에서 LP 포인터라는 뜻 Long Pointer
	
	// 타이틀바의 프레임까지 고려해서 크기를 설정해야 함. 
	
	//          위치    크기 200        200
	RECT Rc = { 0, 0, _Size.iX(), _Size.iY() };

	ScreenSize = _Size;

	// 내가 원하는 크기를 넣으면 타이틀바까지 고려한 크기를 리턴주는 함수.
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW,FALSE);

	WindowSize = { static_cast<float>(Rc.right - Rc.left), static_cast<float>(Rc.bottom - Rc.top) };
	
	// 0을 넣어주면 기존의 크기를 유지한다.
	SetWindowPos(HWnd, nullptr, WindowPos.iX(), WindowPos.iY(), WindowSize.iX(), WindowSize.iY(), SWP_NOZORDER);

	// 완전히 똑같은 크기의 이미지.

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