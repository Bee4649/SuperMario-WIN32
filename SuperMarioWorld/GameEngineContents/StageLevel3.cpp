#include "StageLevel3.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRender.h>

#include "Mario.h"
#include "Map.h"
#include "Goal.h"
#include "Bamba.h"
#include "SuperMushroom.h"
#include "FireFlower.h"
#include "TurnBlock.h"
#include "QuestionBlock.h"
#include "Coin.h"
#include "Pipe.h"
#include "CheckPointActor.h"
#include "Nokonoko.h"
#include "Killer.h"

StageLevel3::StageLevel3() {

}

StageLevel3::~StageLevel3() {

}


void StageLevel3::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Overworld.mp3");

	Map* Map1 = CreateActor<Map>();
	Map1->SetImage(BackGroundName, StageName, StageColName);

	Map1->AddStartPos({ 350 , 1534 });

	CreateActor<Mario>();
	UI = CreateActor<UIManager>();
	Map1->MoveMap(0);
	CreateActor<Goal>()->SetGoal(GridPos(285, 0));

	{
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(30, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(42, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(48, 3));
		QuestionBlock* ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(35, 4));
		ItemBox->SetItem(ItemType::SuperMushroom);

		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(65, 3));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(78, 0));

		ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(80, 3));
		ItemBox->SetItem(ItemType::FireFlower);

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(90, 7));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(90, 7));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(100, 5));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(100, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(104, 0));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(119, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(131, 6));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(130, 7));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(129, 8));


		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(144, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(143, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(142, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(157, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(158, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(159, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(160, 0));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(176, 0));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(184, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(198, 3));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(196, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(224, 2));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(230, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(231, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(237, 1));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(238, 1));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(244, 5));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(246, 5));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(251, 2));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(258, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(265, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(266, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(267, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(268, 0));

	}

	StageLevel::LevelChangeStart(_PrevLevel);
}
