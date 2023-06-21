#include "StageLevel3.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "Mario.h"
#include "Map.h"

#include "Killer.h"

StageLevel3::StageLevel3()
{

}

StageLevel3::~StageLevel3() 
{

}


void StageLevel3::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("MainBgm.mp3");

	Map* Map1 = CreateActor<Map>();
	Map1->SetImage(BackGroundName, StageName, StageColName);

	Map1->AddStartPos({ 350 , 1534 });

	CreateActor<Mario>();

	UI = CreateActor<PlayUIManager>();
	Map1->MoveMap(0);
	

	{
		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(30, 6));
		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(42, 0));
		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(48, 3));
	}

	StageLevel::LevelChangeStart(_PrevLevel);
}
