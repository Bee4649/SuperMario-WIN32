#include "WorldLevel.h"
#include "WorldImage.h"
#include "WorldMario.h"
#include "LevelLoader.h"

WorldLevel* WorldLevel::Instance = nullptr;


void WorldLevel::StageClear(const std::string_view& _StageName)
{
	if ("Stage1" == _StageName)
	{
		ImageActor->BlockRender1->Off();
		ImageActor->LockStage1->On();
		//WorldData->Stage3->Unlock();
		return;
	}
	if ("Stage2" == _StageName)
	{
		ImageActor->BlockRender2->Off();
		ImageActor->LockStage2->On();
		//WorldData->Stage4->Unlock();
		return;
	}
}

WorldLevel::WorldLevel() {
	if (nullptr == Instance)
	{
		Instance = this;
	}
}

WorldLevel::~WorldLevel() {

}

void WorldLevel::Loading()
{
	ImageActor = CreateActor<WorldImage>();
	//WorldData = CreateActor<WorldMario>();
	//WorldData->SetPos({ 475, 650 });
	CreateActor<LevelLoader>();
}

void WorldLevel::Update(float _DeltaTime)
{

}

void WorldLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{

	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("YoshiIsland.mp3");
}

void WorldLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	BGMPlayer.Stop();
}