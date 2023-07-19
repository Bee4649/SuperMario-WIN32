#include "LevelLoader.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>
#include "MarioGameCore.h"
#include "ContentsEnums.h"
#include "StageLevel.h"
#include "Mario.h"
LevelLoader* LevelLoader::MainLoader = nullptr;

LevelLoader::LevelLoader()
	: IsFadeIn(true), IsFadeOut(true)
{

}

LevelLoader::~LevelLoader() {
}

void LevelLoader::Goal()
{

}

void LevelLoader::FadeIn()
{
	MainLoader->State = FadeState::FadeIn;
}

void LevelLoader::FadeOut()
{
	MainLoader->State = FadeState::FadeOut;
}

void LevelLoader::ChangeLevel(const std::string_view& _Name)
{
	MainLoader->LevelName = _Name;
	MainLoader->State = FadeState::FadeOutLoad;
}

void LevelLoader::Start()
{
	FadeRender = CreateRender("ALLBLACK.BMP", RenderOrder::Fade);
	FadeRender->EffectCameraOff();
	FadeRender->SetScaleToImage();
	FadeRender->SetPosition(FadeRender->GetScale().half());

}

void LevelLoader::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	MainLoader = this;
	if (true == IsFadeIn)
	{
		State = FadeState::FadeIn;
		FadeRender->SetAlpha(255);
		Timer = 1;
	}
}

void LevelLoader::Update(float _DeltaTime)
{
	switch (State)
	{
	case FadeState::FadeIn:
		FadeInUpdate(_DeltaTime);
		break;
	case FadeState::FadeOutLoad:
		FadeOutLoadUpdate(_DeltaTime);
		break;
	case FadeState::FadeOut:
		FadeOutUpdate(_DeltaTime);
		break;
	case FadeState::STOP:
		break;
	default:
		break;
	}
}

void LevelLoader::FadeInUpdate(float _DeltaTime)
{
	Timer -= Speed * _DeltaTime;
	if (0 > Timer)
	{
		Timer = 0;
		FadeRender->SetAlpha(0);
		State = FadeState::STOP;
		return;
	}
	FadeRender->SetAlpha(static_cast<int>(Timer * 255));
}

void LevelLoader::FadeOutUpdate(float _DeltaTime)
{
	Timer += Speed * _DeltaTime;
	if (1 < Timer)
	{
		Timer = 1;
		FadeRender->SetAlpha(255);
		State = FadeState::STOP;
		return;
	}
	FadeRender->SetAlpha(static_cast<int>(Timer * 255));
}

void LevelLoader::FadeOutLoadUpdate(float _DeltaTime)
{
	Timer += Speed * _DeltaTime;
	if (1 < Timer)
	{
		MarioGameCore::GetInst().ChangeLevel(LevelName);
		return;
	}
	FadeRender->SetAlpha(static_cast<int>(Timer * 255));
}

void LevelLoader::ClearUpdate(float _DeltaTime)
{

}
