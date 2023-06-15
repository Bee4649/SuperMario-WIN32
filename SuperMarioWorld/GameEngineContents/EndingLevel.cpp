#include "EndingLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include "LevelLoader.h"
#include "ContentsEnum.h"
#include "ContentCore.h"

EndingLevel::EndingLevel() 
{
}

EndingLevel::~EndingLevel() 
{
}

void EndingLevel::Loading()
{
	GameEngineRenderer* ImageRender = CreateActor<GameEngineActor>()->CreateRender("ENDING.bmp", RenderOrder::BackGround);
	ImageRender->SetScaleToImage();
	ImageRender->SetPosition(ImageRender->GetScale().half());
	CreateActor<LevelLoader>();
}

void EndingLevel::Update(float _DeltaTime)
{
	Timer += _DeltaTime;
	if (WaitTime > Timer) { return; }
	if (GameEngineInput::IsAnyKey())
	{
		BGMPlayer.Stop();
		LevelLoader::ChangeLevel("Title");
	}
}

void EndingLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("StaffRoll.mp3");
	BGMPlayer.LoopCount(1);

}

