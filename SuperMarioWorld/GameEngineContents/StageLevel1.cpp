#include "StageLevel1.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include "UIManager.h"
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
#include "CheckPointActor.h"
#include "Nokonoko.h"
#include "Killer.h"
StageLevel1::StageLevel1() {

}

StageLevel1::~StageLevel1() {

}

void StageLevel1::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	
	// BGM 설정
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Overworld.mp3");
	BGMPlayer.LoopCount(0);

	// 맵 이미지 생성
	Map* MainMap = CreateActor<Map>();
	MainMap->SetImage(BackGroundName, StageName, StageColName);
	MainMap->SetDebugMap("STAGE1DEBUG.bmp");
	// 맵 시작 위치 지정
	MainMap->AddStartPos(GridPos(5,0));
	MainMap->AddStartPos(GridPos(146,0));
	MainMap->AddStartPos({ 8945, 1471 });

	CreateActor<Mario>();
	UI = CreateActor<UIManager>();

	// 지하에서 파이프를 통해 이동한 경우
	if ("Underground1" == _PrevLevel->GetName())
	{
		MainMap->MoveMap(2);
		Mario::MainPlayer->PipeOut(float4::Up);
		SetTimer(dynamic_cast<StageLevel*>(_PrevLevel)->GetTimer());
	}
	else
	{
		// 체크포인트 지점으로 이동
		MainMap->MoveMap(GetCheckPoint());
	}
	
	// 스테이지 구성
	{
		CreateActor<CheckPointActor>(RenderOrder::Item)->SetCheckPoint(1, GridPos(149, 0));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(11, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(12, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(13, 3));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(14, 3));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(21, 0));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(23, 3));
		QuestionBlock* ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(25, 3));
		ItemBox->SetItem(ItemType::SuperMushroom);
		ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(26, 3));
		ItemBox->SetItem(ItemType::SuperMushroom);
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(27, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(57, 4));
		ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(70, 7));
		ItemBox->SetItem(ItemType::FireFlower);

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(85, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(90, 0));
		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(95, 0));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(100, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(101, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(101, 6));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(100, 8));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(101, 8));
		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(102, 8));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(102, 3));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(105, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(106, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(107, 0));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos({7700, 1341});
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos({ 7764, 1341 });
		Pipe* NewPipe = CreateActor<Pipe>(RenderOrder::Map);
		NewPipe->SetPos({ 7732, 1405 });
		NewPipe->SetPipe("Underground1", Down);

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(131, 4));
		//CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(139, 4));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(158, 4));
		ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(159, 4));
		ItemBox->SetItem(ItemType::SuperMushroom);
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(160, 4));

		CreateActor<Nokonoko>(RenderOrder::Monster)->SetPos(GridPos(175, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(180, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(185, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(188, 2));

		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(200, 4));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(201, 4));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(202, 4));

		for (int i = 0; i < 15; i++) {
			CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(211 + i, 8));
			CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(211 + i, 9));
			CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(211 + i, 10));
		}

		CreateActor<Killer>(RenderOrder::Monster)->SetPos(GridPos(220, 3));
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(234, 3));
		ItemBox = CreateActor<QuestionBlock>(RenderOrder::Map);
		ItemBox->SetPos(GridPos(236, 6));
		ItemBox->SetItem(ItemType::FireFlower);
		CreateActor<TurnBlock>(RenderOrder::Map)->SetPos(GridPos(238, 3));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(250, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(251, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(252, 0));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(253, 0));

		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(270, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(272, 4));
		CreateActor<Bamba>(RenderOrder::Monster)->SetPos(GridPos(274, 4));

		CreateActor<Goal>()->SetGoal(GridPos(302, 0));
	}
	

	StageLevel::LevelChangeStart(_PrevLevel);

}
