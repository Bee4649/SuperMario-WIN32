#include "GameEngineWindowTexture.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEnginePath.h>
#include "GameEngineWindow.h"

#pragma comment(lib, "msimg32.lib")


GameEngineWindowTexture::GameEngineWindowTexture()
{
}

GameEngineWindowTexture::~GameEngineWindowTexture() 
{
	if (nullptr != BitMap)
	{
		DeleteObject(BitMap);
		BitMap = nullptr;
	}

	if (nullptr != OldBitMap)
	{
		DeleteObject(OldBitMap);
		OldBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}
}

bool GameEngineWindowTexture::TextureCreate(HDC _Hdc)
{
	// 단순히 비트맵만 로드하는 함수가 아니라서 우리에게 그냥 HANDLE을 준다
	// 선생님 기억으로는 커서 아이콘등도 이녀석으로 로드할수 있었나? 하는데 그래서 

	// LoadBitmapA()

	// LPCSTR == const char*

	// HANDLE ImageHandle = LoadImageA(nullptr, _Path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (nullptr == _Hdc)
	{
		MsgAssert("이미지 로드에 실패했습니다.");
		return false;
	}

	ImageDC = _Hdc;
	ImageScaleCheck();
	return true;

}

void GameEngineWindowTexture::TextureClear()
{
	Rectangle(ImageDC, 0, 0, Info.bmWidth, Info.bmHeight);
}

bool GameEngineWindowTexture::TextureCreate(const float4& _Scale)
{

	if (true == _Scale.IsZero())
	{
		MsgAssert("크기가 0인 이미지를 만들 수 없습니다.");
		return false;
	}

	// 이미지의 핸들일 뿐이고.
	BitMap = CreateCompatibleBitmap(GameEngineWindow::GetWindowBackBufferHdc(), _Scale.ix(), _Scale.iy());

	if (nullptr == BitMap)
	{
		MsgAssert("이미지 생성에 실패했습니다.");
		return false;
	}

	// 없던 권한을 새롭게 만들어야 한다.
	// Window에서 얻어온 DC
	ImageDC = CreateCompatibleDC(nullptr);
	// 이미지의 수정 권한을 만들어 내고

	if (nullptr == ImageDC)
	{
		MsgAssert("이미지 HDC 생성에 실패했습니다.");
		return false;
	}

	// ImageDC 권한에
	// BitMap 2차원의 색깔 집합을 연결해라.
	// 사실 이미 만들어지자마자 내부에 1,1 이미지와 연결되어있고
	// 내가 로드한 이미지를 그 1,1짜리를 밀어내고 교체하는 작업을 하는데.
	// 이 함수의 리턴값이 기존에 연결되어있던 애를 리턴해주는것.
	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();
	
	TextureClear();

	return true;

}

bool GameEngineWindowTexture::TextureLoad(const GameEnginePath& _Path)
{
	return TextureLoad(_Path.GetPathToString().c_str());
}

bool GameEngineWindowTexture::TextureLoad(const std::string_view& _Path)
{
	// 이미지중에 일부만 로드할수 있는데 0을 넣어주면 다 로드
	// LR_LOADFROMFILE 파일에서부터 로드하겠다는 의미가 된다

	// 이미지를 로드한 2차원 배열의 정보고
	// 윈도우에게 new를 지시한것과 다름이 없다.

	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 로드에 실패했습니다.");
		return false;
	}
	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 HDC 생성에 실패했습니다.");
		return false;
	}

	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();

	return true;
}

void GameEngineWindowTexture::ImageScaleCheck()
{
	HBITMAP CurrentBitMap = static_cast<HBITMAP>(GetCurrentObject(ImageDC, OBJ_BITMAP));
	
	GetObject(CurrentBitMap, sizeof(BITMAP), &Info);

}

// Copy

void GameEngineWindowTexture ::BitCopy(const GameEngineWindowTexture* _CopyTexture, float4 _Pos, float4 _Scale)
{
	BitBlt(
		ImageDC, // 복사 당할 이미지
		_Pos.ix() - _Scale.hix(), // 위치 
		_Pos.iy() - _Scale.hiy(),
		_Scale.ix(),
		_Scale.iy(),
		_CopyTexture->GetImageDC(), // 복사할 이미지
		0,
		0,
		SRCCOPY
	);

}
// 구현쪽에서는 디폴트 인자를 표시할 필요가 없습니다.
void GameEngineWindowTexture::TransCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color/* = RGB(255, 0, 255)*/)
{
	if (false == _CopyTexture->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _CopyTexture->GetCutData(_CutIndex);

	TransCopy(_CopyTexture, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Color);
}

void GameEngineWindowTexture::TransCopy(const GameEngineWindowTexture* _CopyTexture, float4 _Pos, float4 _Scale, float4 _OtherPos, float4 _OtherScale, int _TransColor)
{
	// 특정 DC에 연결된 색상을
	// 특정 DC에 고속복사하는 함수입니다.
	TransparentBlt(ImageDC, // 여기에 그려라.
		_Pos.ix() - _Scale.hix(), // 여기를 시작으로
		_Pos.iy() - _Scale.hiy(),
		_Scale.ix(), // 이 크기로
		_Scale.iy(),
		_CopyTexture->GetImageDC(),
		_OtherPos.ix(), // 카피하려는 이미지의 왼쪽위 x
		_OtherPos.iy(), // 카피하려는 이미지의 왼쪽위 y
		_OtherScale.ix(), // 그부분부터 사이즈  x
		_OtherScale.iy(), // 그부분부터 사이즈  y
		_TransColor);
}

void GameEngineWindowTexture::AlphaCopy(const GameEngineWindowTexture* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color)
{
	if (false == _OtherImage->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _OtherImage->GetCutData(_CutIndex);

	AlphaCopy(_OtherImage, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Color);
}

void GameEngineWindowTexture::AlphaCopy(const GameEngineWindowTexture* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Alpha)
{
	BLENDFUNCTION BF;

	BF.BlendOp = AC_SRC_OVER;
	BF.BlendFlags = 0;
	BF.SourceConstantAlpha = _Alpha;
	BF.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(ImageDC, // 여기에 그려라.
		_CopyCenterPos.ix() - _CopySize.hix(), // 여기를 시작으로
		_CopyCenterPos.iy() - _CopySize.hiy(),
		_CopySize.ix(), // 이 크기로
		_CopySize.iy(),
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),// 이미지의 x y에서부터
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // 이미지의 x y까지의 위치를
		_OtherImageSize.iy(),
		BF);
}

void GameEngineWindowTexture::PlgCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineWindowTexture* _FilterImage)
{
	if (false == _CopyTexture->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _CopyTexture->GetCutData(_CutIndex);

	PlgCopy(_CopyTexture, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Angle, _FilterImage);
}

void GameEngineWindowTexture::PlgCopy(const GameEngineWindowTexture* _CopyTexture, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, float _Angle, GameEngineWindowTexture* _FilterImage)
{
	POINT ArrRotPoint[3];

	CollisionData Data = { float4::ZERO, _CopySize };

	float4 LeftTop = Data.LeftTop();
	float4 RightTop = Data.RightTop();
	float4 LeftBot = Data.LeftBot();

	ArrRotPoint[0] = (LeftTop.RotaitonZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();
	ArrRotPoint[1] = (RightTop.RotaitonZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();
	ArrRotPoint[2] = (LeftBot.RotaitonZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();


	PlgBlt(ImageDC, // 여기에 그려라.
		ArrRotPoint,
		_CopyTexture->GetImageDC(),
		_OtherImagePos.ix(),// 이미지의 x y에서부터
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // 이미지의 x y까지의 위치를
		_OtherImageSize.iy(),
		_FilterImage->BitMap,
		_OtherImagePos.ix(),
		_OtherImagePos.iy()
	);

}

void GameEngineWindowTexture::Cut(int _X, int _Y)
{
	ImageCutData Data;

	Data.SizeX = static_cast<float>(GetImageScale().ix() / _X);
	Data.SizeY = static_cast<float>(GetImageScale().iy() / _Y);

	for (size_t i = 0; i < _Y; i++)
	{
		for (size_t i = 0; i < _X; i++)
		{
			ImageCutDatas.push_back(Data);
			Data.StartX += Data.SizeX;
		}

		Data.StartX = 0.0f;
		Data.StartY += Data.SizeY;
	}

	IsCut = true;
}

void GameEngineWindowTexture::Cut(float4 _Start, float4 _End, int _X, int _Y)
{
	ImageCutData Data;

	Data.SizeX = static_cast<float>((_End.X - _Start.X) / _X);
	Data.SizeY = static_cast<float>((_End.Y - _Start.Y) / _Y);

	Data.StartX = _Start.X;
	Data.StartY = _Start.Y;

	for (size_t i = 0; i < _Y; i++)
	{
		for (size_t i = 0; i < _X; i++)
		{
			ImageCutDatas.push_back(Data);
			Data.StartX += Data.SizeX;
		}

		Data.StartX = _Start.X;
		Data.StartY += Data.SizeY;
	}

	IsCut = true;
}

DWORD GameEngineWindowTexture::GetPixelColor(float4 _Pos, DWORD _OutColor)
{
	return GetPixelColor(_Pos.ix(), _Pos.iy(), _OutColor);
}

DWORD GameEngineWindowTexture::GetPixelColor(int _X, int _Y, DWORD _OutColor)
{
	if (_X < 0)
	{
		return _OutColor;
	}
	if (_X >= GetImageScale().ix())
	{
		return _OutColor;
	}
	if (_Y < 0)
	{
		return _OutColor;
	}
	if (_Y >= GetImageScale().iy())
	{
		return _OutColor;
	}

	return GetPixel(ImageDC, _X, _Y);
}