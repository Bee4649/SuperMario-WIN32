#pragma once
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <map>
#include "GameEngineComponent.h"
// 랜더링에 관련된 기능을 모두 집약한다.

enum class TextAlign
{
	Left = DT_LEFT,
	Right = DT_RIGHT,
	Center = DT_CENTER
};

class FrameAnimationParameter
{
public:
	std::string_view AnimationName = "";
	std::string_view ImageName = "";
	std::string_view FilterName = "";
	int Start = 0;
	int End = 0;
	int CurrentIndex = 0;
	float InterTime = 0.1f;
	bool Loop = true;
	std::vector<int> FrameIndex = std::vector<int>();
	std::vector<float> FrameTime = std::vector<float>();
};


// 설명 :
class GameEngineActor;
class GameEngineLevel;
class GameEngineRenderer : public GameEngineComponent
{
	friend GameEngineActor;
	friend GameEngineLevel;

public:
	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	inline GameEngineWindowTexture* GetImage()
	{
		return Image;
	}

	inline int GetFrame()
	{
		return Frame;
	}

	inline void SetTransColor(int _Color)
	{
		TransColor = _Color;
	}

	inline void SetEffectCamera(bool _Effect)
	{
		IsEffectCamera = _Effect;
	}

	inline void EffectCameraOff()
	{
		SetEffectCamera(false);
	}

	inline void EffectCameraOn()
	{
		SetEffectCamera(true);
	}

	inline int GetTextHeight()
	{
		return TextHeight;
	}

	inline void SetAlpha(int _Alpha)
	{
		Alpha = _Alpha;
	}

	inline float GetAngle(float _Angle)
	{
		return Angle;
	}

	inline void SetAngleAdd(float _Angle)
	{
		Angle += _Angle;
	}

	inline void SetAngle(float _Angle)
	{
		Angle = _Angle;
	}

	inline void SetTextBoxScale(float4 _TextBoxScale)
	{
		TextBoxScale = _TextBoxScale;
	}

	void SetRotFilter(const std::string_view& _ImageName);

	void SetImage(const std::string_view& _ImageName);

	void SetImageToScaleToImage(const std::string_view& _ImageName);

	void SetScaleToImage();

	void SetFrame(int _Frame);

	bool IsAnimationEnd();
	void CreateAnimation(const FrameAnimationParameter& _Paramter);
	void ChangeAnimation(const std::string_view& _AnimationName, bool _ForceChange = false);

	void SetOrder(int _Order) override;


	void SetText(const std::string_view& _Text, const int _TextHeight = 20, const std::string_view& _TextType = "굴림", const TextAlign _TextAlign = TextAlign::Center, const COLORREF _TextColor = RGB(0, 0, 0), float4 TextBoxScale = float4::ZERO);

protected:

private:
	GameEngineWindowTexture* Image = nullptr;
	GameEngineWindowTexture* RotationFilter = nullptr;

	bool IsEffectCamera = true;

	int TransColor = RGB(255, 0, 255);

	int Frame = 0;

	int Alpha = 255;

	void Render(float _DeltaTime);

	void TextRender(float _DeltaTime);
	void ImageRender(float _DeltaTime);

	class FrameAnimation
	{
	public:
		GameEngineRenderer* Parent = nullptr;
		// 짤려있는 이미지여야 한다.
		GameEngineWindowTexture* Image = nullptr;
		GameEngineWindowTexture* FilterImage = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		int CurrentIndex = 0;
		float CurrentTime = 0.0f;
		bool Loop = true;


		bool IsEnd();

		void Render(float _DeltaTime);

		void Reset()
		{
			CurrentIndex = 0;
			CurrentTime = 0.0f;
		}
	};

	std::map<std::string, FrameAnimation> Animation;
	FrameAnimation* CurrentAnimation = nullptr;


	/// TextRender
	
	std::string RenderText = std::string();
	int TextHeight = 0;
	std::string TextType = std::string();
	TextAlign Align = TextAlign::Left;
	COLORREF TextColor = RGB(0, 0, 0);
	float4 TextBoxScale;


	float Angle = 0.0f;

	
};

