#include "WorldImage.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/NumberRenderObject.h>
#include "MarioGameCore.h"
#include "ContentsEnums.h"
WorldImage::WorldImage() {

}

WorldImage::~WorldImage() {

}

void WorldImage::Start()
{
	{
		{
			GameEngineRender* Render = CreateRender("SEA.BMP", RenderOrder::Sky);
			Render->SetScale(GameEngineWindow::GetScreenSize());
			Render->SetPosition(Render->GetScale().half());
		}
		for (int i = 0; i < 42; i++)
		{
			GameEngineRender* Render = CreateRender(RenderOrder::BackGround);
			Render->SetScale({ 1024 ,8 });
			if (i % 2 == 0)
			{
				Render->CreateAnimation({ .AnimationName = "TIDE", .ImageName = "TIDE.BMP", .Start = 0, .End = 6, .InterTime = 0.2f });
			}
			else
			{
				Render->CreateAnimation({ .AnimationName = "TIDE", .ImageName = "TIDE.BMP",  .InterTime = 0.2f, .FrameIndex = {6, 5, 4, 3, 2, 1, 0} });
			}
			Render->ChangeAnimation("TIDE");
			Render->SetPosition({ 512, 168 + (i * 16.0f) });
		}

		{
			GameEngineRender* Render = CreateRender("WORLD1.BMP", RenderOrder::Map);
			Render->SetScaleToImage();
			Render->SetPosition(Render->GetScale().half());
			Render = CreateRender("WORLD1FRONT.BMP", RenderOrder::ForeGround);
			Render->SetScaleToImage();
			Render->SetPosition(Render->GetScale().half());

			BlockRender1 = CreateRender("STAGEBLOCK1.bmp", RenderOrder::Block);
			BlockRender1->SetScaleToImage();
			BlockRender1->SetPosition(Render->GetScale().half());
			BlockRender2 = CreateRender("STAGEBLOCK2.bmp", RenderOrder::Block);
			BlockRender2->SetScaleToImage();
			BlockRender2->SetPosition(Render->GetScale().half());
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {0, 1, 2, 3, 2, 1, 0} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 200, 300 });

		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {0, 1, 2, 3, 2, 1, 0} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 575, 240 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {0, 1, 2, 3, 2, 1, 0} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 775, 280 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {0, 1, 2, 3, 2, 1, 0} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 635, 600 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 150, 400 });
			Render = CreateRender(RenderOrder::ForeGround);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 150, 320 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 615, 215 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 500, 460 });
		}
		{
			GameEngineRender* Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 820, 480 });
			Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 861, 480 });
			Render = CreateRender(RenderOrder::PlayerAttack);
			Render->SetScale({ 82 ,30 });
			Render->CreateAnimation({ .AnimationName = "CLOUD", .ImageName = "CLOUD.BMP",  .InterTime = 0.3f, .FrameIndex = {3, 2, 1, 0, 1, 2, 3} });
			Render->ChangeAnimation("CLOUD");
			Render->SetPosition({ 630, 450 });
		}


		// 스테이지
		{
			GameEngineRender* Render = CreateRender(RenderOrder::Item);
			Render->SetScale({ 68 ,68 });
			Render->CreateAnimation({ .AnimationName = "STAGE", .ImageName = "STAGEBUTTON.BMP", .Start = 0, .End = 6, .InterTime = 0.1f });
			Render->ChangeAnimation("STAGE");
			Render->SetPosition({ 289, 706 });

			Render = CreateRender(RenderOrder::Item);
			Render->SetScale({ 68 ,68 });
			Render->CreateAnimation({ .AnimationName = "STAGE", .ImageName = "STAGEBUTTON.BMP", .Start = 0, .End = 6, .InterTime = 0.1f });
			Render->ChangeAnimation("STAGE");
			Render->SetPosition({ 674, 706 });

			LockStage1 = CreateRender(RenderOrder::Item);
			LockStage1->SetScale({ 68 ,68 });
			LockStage1->CreateAnimation({ .AnimationName = "STAGE", .ImageName = "STAGEBUTTON.BMP", .Start = 0, .End = 6, .InterTime = 0.1f });
			LockStage1->ChangeAnimation("STAGE");
			LockStage1->SetPosition({ 674, 578 });
			LockStage1->Off();

			LockStage2 = CreateRender(RenderOrder::Item);
			LockStage2->SetScale({ 68 ,68 });
			LockStage2->CreateAnimation({ .AnimationName = "STAGE", .ImageName = "STAGEBUTTON.BMP", .Start = 0, .End = 6, .InterTime = 0.1f });
			LockStage2->ChangeAnimation("STAGE");
			LockStage2->SetPosition({ 802, 450 });
			LockStage2->Off();
		}

		// 플레이어
		{
			AnimationRender = CreateRender(RenderOrder::Player);
			AnimationRender->CreateAnimation({.AnimationName = "Normal", .ImageName = "LEFT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.12f });
			AnimationRender->CreateAnimation({ .AnimationName = "Super", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.12f });
			AnimationRender->CreateAnimation({ .AnimationName = "Fire", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.12f });
			AnimationRender->SetScale({ 192,192 });
			AnimationRender->SetPosition({ 148,95 });
			AnimationRender->ChangeAnimation("Normal");
		}
		{
			LifeNum.SetOwner(this);
			LifeNum.SetImage("NUMBER.BMP", { 32 ,28 }, static_cast<int>(RenderOrder::UI), RGB(255, 0, 255), "NONE.bmp");
			LifeNum.SetRenderPos({ 300, 138 });
			LifeNum.SetAlign(Align::Right);
			LifeNum.SetValue(3);
		}
	}
}

void WorldImage::Update(float _DeltaTime)
{
}

void WorldImage::LevelChangeStart(GameEngineLevel* _Prev)
{
	LifeNum.SetValue(MarioGameCore::GetInst().GetLife());
	switch (MarioGameCore::GetInst().GetMarioStateData())
	{
	case PowerState::Normal:
		AnimationRender->ChangeAnimation("Normal");
		break;
	case PowerState::Super:
		AnimationRender->ChangeAnimation("Super");
		break;
	case PowerState::Fire:
		AnimationRender->ChangeAnimation("Fire");
		break;
	default:
		break;
	}
}
