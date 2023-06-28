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
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(10, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(10, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(11, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(11, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(12, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(12, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(13, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(13, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(14, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(14, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(15, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(15, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(16, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(16, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(17, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(17, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(18, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(18, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(19, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(19, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(20, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(20, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(21, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(21, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(22, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(22, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(23, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(23, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(24, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(24, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(25, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(25, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(26, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(26, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(27, 2));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(27, 3));

		
		QuestionBlock* ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		
		ItemBox->SetItem(ItemType::SuperMushroom);
		ItemBox->SetPos(GridPos(22, 4));
		ItemBox->SetItem(ItemType::UpMushroom);

		QuestionBlock* NewBlock = CreateActor<QuestionBlock>(RenderOrder::Map);
		NewBlock->SetPos(GridPos(30, 5));
		NewBlock->SetItem(ItemType::FireFlower);


	}


	StageLevel::LevelChangeStart(_Prev);
}