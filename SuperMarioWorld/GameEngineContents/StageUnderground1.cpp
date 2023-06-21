#include "StageUnderground1.h"
#include "Map.h"
#include "Mario.h"
#include "Pipe.h"
#include "Coin.h"
#include "QuestionBlock.h"

StageUnderground1::StageUnderground1() 
{
}

StageUnderground1::~StageUnderground1() 
{
}


void StageUnderground1::Loading()
{
	StageLevel::Loading();
	
}

void StageUnderground1::LevelChangeStart(GameEngineLevel* _Prev)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Underground.mp3");
	BGMPlayer.LoopCount(0);

	Map* MainMap = CreateActor<Map>();
	MainMap->SetImage(BackGroundName, StageName, StageColName);

	MainMap->AddStartPos({ 128, 1216 });

	MainMap->GetBackGroundRender()->CreateAnimation({ .AnimationName = "BackGroundAnim", .ImageName = BackGroundName, .Start = 0, .End = 3 });
	MainMap->GetBackGroundRender()->SetScale({ 2048, 1728 });
	MainMap->GetBackGroundRender()->SetPosition({ 1024, 864 });
	MainMap->GetBackGroundRender()->ChangeAnimation("BackGroundAnim");

	CreateActor<Mario>();
	UI = CreateActor<PlayUIManager>();

	if ("Stage1" == _Prev->GetName())
	{
		SetTimer(dynamic_cast<StageLevel*>(_Prev)->GetTimer());
		Mario::MainPlayer->PipeOut(float4::DOWN);
	}


	Pipe* NewPipe = CreateActor<Pipe>(RenderOrder::Map);
	NewPipe->SetPos({ 1772, 1532 });

	NewPipe->SetPipe("Stage1", Right);

	MainMap->MoveMap(0);

	{
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(7, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(7, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(8, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(8, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(9, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(9, 3));

		
		QuestionBlock* NewBlock = CreateActor<QuestionBlock>(RenderOrder::Map);
		NewBlock->SetPos(GridPos(30, 5));
		NewBlock->SetItem(ItemType::FireFlower);
	}


	StageLevel::LevelChangeStart(_Prev);
}