#include "StageLevel2.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include "PlayUIManager.h"
#include "Mario.h"
#include "Map.h"
#include "Bamba.h"
#include "SuperMushroom.h"
#include "FireFlower.h"
#include "TurnBlock.h"
#include "QuestionBlock.h"
#include "Goal.h"
#include "Coin.h"
#include "Pipe.h"
#include "CheckPoint.h"
#include "Nokonoko.h"
#include "Killer.h"
StageLevel2::StageLevel2()
{
}

StageLevel2::~StageLevel2()
{
}

void StageLevel2::Loading()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Texture");
	Dir.Move("Play");

}

void StageLevel2::LevelChangeStart(GameEngineLevel* _PrevLevel)
{


	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Overworld.mp3");

	// 맵 이미지 생성
	Map* Map1 = CreateActor<Map>();
	Map1->SetImage(BackGroundName, StageName, StageColName);
	Map1->SetDebugMap("STAGE2DEBUG.bmp");
	CreateActor<Mario>();
	UI = CreateActor<PlayUIManager>();
	// 맵 시작 위치
	Map1->AddStartPos(GridPos(3, 0));
	Map1->AddStartPos(GridPos(163, 0));
	Map1->MoveMap(GetCheckPoint());

	SetCameraMove({ 0, 790 });
	CreateActor<Goal>()->SetGoal(GridPos(302, 0));
	
	CreateActor<CheckPointActor>(RenderOrder::Item)->SetCheckPoint(1, GridPos(163, 0));
	{
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(24, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(28, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(29, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(30, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(31, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(32, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(33, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(55, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(65, 0));
		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(75, 3));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(85, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(95, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(100, 0));
		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(110, 4));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(81, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(82, 3));
		QuestionBlock* ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(80, 3));
		ItemBox->SetItem(ItemType::SuperMushroom);

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(134, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(137, 4));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(150, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(152, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(153, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(154, 0));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(195, 6));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(199, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(220, 5));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(248, 0));

		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(300, 2));
	}
	StageLevel::LevelChangeStart(_PrevLevel);
}
