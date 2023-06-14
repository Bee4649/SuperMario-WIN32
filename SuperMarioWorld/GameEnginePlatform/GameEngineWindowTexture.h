#pragma once
#include <Windows.h>
#include <string>
#include <string_view>
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineDebug.h>

struct ImageCutData
{
	float StartX = 0.0f;
	float StartY = 0.0f;
	float SizeX = 0.0f;
	float SizeY = 0.0f;

	float4 GetStartPos()
	{
		return { StartX, StartY };
	}

	float4 GetScale()
	{
		return { SizeX, SizeY };
	}
};
// 설명 :
class GameEnginePath;
class GameEngineWindowTexture
{
public:
	// constrcuter destructer
	GameEngineWindowTexture();
	~GameEngineWindowTexture();

	// delete Function
	GameEngineWindowTexture(const GameEngineWindowTexture& _Other) = delete;
	GameEngineWindowTexture(GameEngineWindowTexture&& _Other) noexcept = delete;
	GameEngineWindowTexture& operator=(const GameEngineWindowTexture& _Other) = delete;
	GameEngineWindowTexture& operator=(GameEngineWindowTexture&& _Other) noexcept = delete;

	bool TextureCreate(HDC _Hdc);

	bool TextureCreate(const float4& _Scale);
	
	bool TextureLoad(const GameEnginePath& _Path);

	bool TextureLoad(const std::string_view& _Path);

	void TextureClear();

	HDC GetImageDC() const
	{
		return ImageDC;
	}

	float4 GetImageScale() const
	{
		return float4{ static_cast<float>(Info.bmWidth), static_cast<float>(Info.bmHeight) };
	}

	bool IsImageCutting()
	{
		return IsCut;
	}

	size_t GetImageCuttingCount()
	{
		return ImageCutDatas.size();
	}

	bool IsCutIndexValid(int _Index) const
	{
		if (0 > _Index)
		{
			return false;
		}

		if (ImageCutDatas.size() <= _Index)
		{
			return false;
		}

		return true;
	}

	ImageCutData GetCutData(int _Index) const
	{
		if (false == IsCutIndexValid(_Index))
		{
			MsgAssert("유효하지 않은 컷 인덱스 입니다.");
		}

		return ImageCutDatas[_Index];
	}

	void Cut(float4 _Start, float4 _End, int _X, int _Y);

	void Cut(int _X, int _Y);

	// copy
	// 디폴트 인자는 선언에서만 가능.

	void BitCopy(const GameEngineWindowTexture* _CopyTexture, float4 _Pos, float4 _Scale);

	// 랜더링을 제외할 컬러.
	void TransCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color = RGB(255, 0, 255));
	
	void TransCopy(const GameEngineWindowTexture* _CopyTexture, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherTexturePos, float4 _OtherTextureSize, int _Color = RGB(255, 0, 255));

	void AlphaCopy(const GameEngineWindowTexture* _CopyTexture, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherTexturePos, float4 _OtherTextureSize, int _Alpha);

	void AlphaCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color);

	void PlgCopy(const GameEngineWindowTexture* _CopyTexture, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherTexturePos, float4 _OtherTextureSize, float _Angle, GameEngineWindowTexture* _FilterTexture);

	void PlgCopy(const GameEngineWindowTexture* _CopyTexture, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineWindowTexture* _FilterTexture);
	
	DWORD GetPixelColor(float4 _Pos, DWORD _OutColor);

	DWORD GetPixelColor(int _X, int _Y, DWORD _OutColor);


protected:

private:
	HBITMAP BitMap;
	HBITMAP OldBitMap;
	HDC ImageDC;
	BITMAP Info;
	bool IsCut = false;

	std::vector<ImageCutData> ImageCutDatas;

	void ScaleCheck();
};

