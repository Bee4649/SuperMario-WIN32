//#include "StageUnderground2.h"
//#include "Map.h"
//#include "Mario.h"
//#include "Pipe.h"
//#include "Coin.h"
//#include "QuestionBlock.h"
//
//StageUnderground2::StageUnderground2() 
//{
//}
//
//StageUnderground2::~StageUnderground2() 
//{
//}
//
//void StageUnderground2::Loading()
//{
//	StageLevel::Loading();
//	
//}
//
//void StageUnderground2::LevelChangeStart(GameEngineLevel* _Prev)
//{
//	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Underground.mp3");
//	BGMPlayer.LoopCount(0);
//
//	Map* Map1 = CreateActor<Map>();
//	Map1->SetImage(BackGroundName, StageName, StageColName);
//
//	Map1->AddStartPos({ 128, 1216 });
//
//	Map1->GetBackGroundRender()->CreateAnimation({ .AnimationName = "BackGroundAnim", .ImageName = BackGroundName, .Start = 0, .End = 3 });
//	Map1->GetBackGroundRender()->SetScale({ 2048, 1728 });
//	Map1->GetBackGroundRender()->SetPosition({ 1024, 864 });
//	Map1->GetBackGroundRender()->ChangeAnimation("BackGroundAnim");
//
//	CreateActor<Mario>();
//	UI = CreateActor<PlayUIManager>();
//
//	if ("Stage2" == _Prev->GetName())
//	{
//		SetTimer(dynamic_cast<StageLevel*>(_Prev)->GetTimer());
//		Mario::MainPlayer->PipeOut(float4::DOWN);
//	}
//
//
//	Pipe* NewPipe = CreateActor<Pipe>(RenderOrder::Map);
//	NewPipe->SetPos({ 1772, 1532 });
//
//	NewPipe->SetPipe("Stage2", Right);
//
//	Map1->MoveMap(0);
//
//	{
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(7, 2));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(7, 3));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(8, 2));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(8, 3));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(9, 2));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(9, 3));
//
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(15, 3));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(15, 4));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(16, 3));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(16, 4));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(17, 3));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(17, 4));
//
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(20, 4));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(20, 5));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(21, 4));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(21, 5));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(22, 4));
//		CreateActor<Coin>(RenderOrder::Item)->SetPos(GridPos(22, 5));
//
//		QuestionBlock* NewBlock = CreateActor<QuestionBlock>(RenderOrder::Map);
//		NewBlock->SetPos(GridPos(30, 5));
//		NewBlock->SetItem(ItemType::FireFlower);
//	}
//
//
//	StageLevel::LevelChangeStart(_Prev);
//}
