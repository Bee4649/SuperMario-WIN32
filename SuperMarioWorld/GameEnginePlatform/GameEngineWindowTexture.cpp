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
	// �ܼ��� ��Ʈ�ʸ� �ε��ϴ� �Լ��� �ƴ϶� �츮���� �׳� HANDLE�� �ش�
	// ������ ������δ� Ŀ�� �����ܵ �̳༮���� �ε��Ҽ� �־���? �ϴµ� �׷��� 

	// LoadBitmapA()

	// LPCSTR == const char*

	// HANDLE ImageHandle = LoadImageA(nullptr, _Path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (nullptr == _Hdc)
	{
		MsgAssert("�̹��� �ε忡 �����߽��ϴ�.");
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
		MsgAssert("ũ�Ⱑ 0�� �̹����� ���� �� �����ϴ�.");
		return false;
	}

	// �̹����� �ڵ��� ���̰�.
	BitMap = CreateCompatibleBitmap(GameEngineWindow::GetWindowBackBufferHdc(), _Scale.ix(), _Scale.iy());

	if (nullptr == BitMap)
	{
		MsgAssert("�̹��� ������ �����߽��ϴ�.");
		return false;
	}

	// ���� ������ ���Ӱ� ������ �Ѵ�.
	// Window���� ���� DC
	ImageDC = CreateCompatibleDC(nullptr);
	// �̹����� ���� ������ ����� ����

	if (nullptr == ImageDC)
	{
		MsgAssert("�̹��� HDC ������ �����߽��ϴ�.");
		return false;
	}

	// ImageDC ���ѿ�
	// BitMap 2������ ���� ������ �����ض�.
	// ��� �̹� ��������ڸ��� ���ο� 1,1 �̹����� ����Ǿ��ְ�
	// ���� �ε��� �̹����� �� 1,1¥���� �о�� ��ü�ϴ� �۾��� �ϴµ�.
	// �� �Լ��� ���ϰ��� ������ ����Ǿ��ִ� �ָ� �������ִ°�.
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
	// �̹����߿� �Ϻθ� �ε��Ҽ� �ִµ� 0�� �־��ָ� �� �ε�
	// LR_LOADFROMFILE ���Ͽ������� �ε��ϰڴٴ� �ǹ̰� �ȴ�

	// �̹����� �ε��� 2���� �迭�� ������
	// �����쿡�� new�� �����ѰͰ� �ٸ��� ����.

	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " �̹��� �ε忡 �����߽��ϴ�.");
		return false;
	}
	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " �̹��� HDC ������ �����߽��ϴ�.");
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
		ImageDC, // ���� ���� �̹���
		_Pos.ix() - _Scale.hix(), // ��ġ 
		_Pos.iy() - _Scale.hiy(),
		_Scale.ix(),
		_Scale.iy(),
		_CopyTexture->GetImageDC(), // ������ �̹���
		0,
		0,
		SRCCOPY
	);

}
// �����ʿ����� ����Ʈ ���ڸ� ǥ���� �ʿ䰡 �����ϴ�.
void GameEngineWindowTexture::TransCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color/* = RGB(255, 0, 255)*/)
{
	if (false == _CopyTexture->IsCut)
	{
		MsgAssert(" �߸��� ���� �̹����� cut��� �Լ��� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	ImageCutData Data = _CopyTexture->GetCutData(_CutIndex);

	TransCopy(_CopyTexture, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Color);
}

void GameEngineWindowTexture::TransCopy(const GameEngineWindowTexture* _CopyTexture, float4 _Pos, float4 _Scale, float4 _OtherPos, float4 _OtherScale, int _TransColor)
{
	// Ư�� DC�� ����� ������
	// Ư�� DC�� ��Ӻ����ϴ� �Լ��Դϴ�.
	TransparentBlt(ImageDC, // ���⿡ �׷���.
		_Pos.ix() - _Scale.hix(), // ���⸦ ��������
		_Pos.iy() - _Scale.hiy(),
		_Scale.ix(), // �� ũ���
		_Scale.iy(),
		_CopyTexture->GetImageDC(),
		_OtherPos.ix(), // ī���Ϸ��� �̹����� ������ x
		_OtherPos.iy(), // ī���Ϸ��� �̹����� ������ y
		_OtherScale.ix(), // �׺κк��� ������  x
		_OtherScale.iy(), // �׺κк��� ������  y
		_TransColor);
}

void GameEngineWindowTexture::AlphaCopy(const GameEngineWindowTexture* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color)
{
	if (false == _OtherImage->IsCut)
	{
		MsgAssert(" �߸��� ���� �̹����� cut��� �Լ��� ����Ϸ��� �߽��ϴ�.");
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

	AlphaBlend(ImageDC, // ���⿡ �׷���.
		_CopyCenterPos.ix() - _CopySize.hix(), // ���⸦ ��������
		_CopyCenterPos.iy() - _CopySize.hiy(),
		_CopySize.ix(), // �� ũ���
		_CopySize.iy(),
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),// �̹����� x y��������
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // �̹����� x y������ ��ġ��
		_OtherImageSize.iy(),
		BF);
}

void GameEngineWindowTexture::PlgCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineWindowTexture* _FilterImage)
{
	if (false == _CopyTexture->IsCut)
	{
		MsgAssert(" �߸��� ���� �̹����� cut��� �Լ��� ����Ϸ��� �߽��ϴ�.");
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


	PlgBlt(ImageDC, // ���⿡ �׷���.
		ArrRotPoint,
		_CopyTexture->GetImageDC(),
		_OtherImagePos.ix(),// �̹����� x y��������
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // �̹����� x y������ ��ġ��
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