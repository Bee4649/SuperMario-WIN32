#include "PlayLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include "Mario.h"
#include "Map.h"

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::Loading()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Image");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("STAGE0COL.BMP"));
}

void PlayLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsDown("1"))
	{
		GameEngineCore::GetInst()->ChangeLevel("Title");
	}
}

void PlayLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	if (nullptr != Mario::MainPlayer)
	{
		Mario::MainPlayer->Death();
		Mario::MainPlayer = nullptr;
	}
	SetCameraPos(float4::Zero);
}

void PlayLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	CreateActor<Mario>();
	Map* Map1 = CreateActor<Map>();
	Map1->SetImage("SKY.BMP", "BACKGROUND01.BMP", "STAGE1.BMP");
	Map1->MoveMap(1);

}
