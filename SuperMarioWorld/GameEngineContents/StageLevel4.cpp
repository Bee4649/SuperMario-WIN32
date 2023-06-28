#include "StageLevel4.h"
#include "Map.h"
#include "Mario.h"
#include "ContentsEnum.h"
#include "Koopa.h"

StageLevel4::StageLevel4() 
{

}

StageLevel4::~StageLevel4() 
{

}

void StageLevel4::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("BowserBGM.mp3");

	Map* Map1 = CreateActor<Map>();
	Map1->SetImage(BackGroundName, StageName, StageColName);

	Map1->GetStageRender()->SetOrder(static_cast<int>(RenderOrder::ForeGround));

	Map1->AddStartPos({ 350 , 1534 });

	CreateActor<Mario>();
	UI = CreateActor<PlayUIManager>();
	Map1->MoveMap(0);

	CreateActor<Koopa>(RenderOrder::Monster)->SetPos({ 500, 1200 });

	StageLevel::LevelChangeStart(_PrevLevel);
}
