#include "Mario.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentCore.h"
#include "Map.h"
#include "Fire.h"
#include "Particle.h"
#include "Block.h"
#include "StageLevel.h"
#include "EnemyActor.h"
#include "LevelLoader.h"

Mario* Mario::MainPlayer = nullptr;

void Mario::NewItem(ItemType _Item)
{

	if (MarioState::GameOver == StateValue)
	{
		return;
	}

	switch (_Item)
	{
	case ItemType::Coin:
		CurrentLevel->AddCoin();
		break;
	case ItemType::UpMushroom:
		break;
	case ItemType::SuperMushroom:
		NewPower(PowerState::Super);
		break;
	case ItemType::FireFlower:
		NewPower(PowerState::Fire);
		break;
	default:
		break;
	}
}

void Mario::Die()
{
	ChangeState(MarioState::GameOver);
}


void Mario::StageClear()
{
	StateValue = MarioState::VICTORY;
	DirValue = Dir::Right;
	AnimationRender->On();
	ChangeAnimation("ClearWalk");
	Timer = 0;

	switch (MarioPower)
	{
	case PowerState::Normal:
		break;
	case PowerState::Super:
		BeforeAnim = "Super_";
		break;
	case PowerState::Fire:
		BeforeAnim = "Fire_";
		break;
	default:
		break;
	}
}

void Mario::DropHold()
{
	IsHold = false;
	HoldActor = nullptr;
}


void Mario::PipeIn(const float4& _PipePos, const float4& _Dir)
{
	IsHold = false;
	GameEngineResources::GetInst().SoundPlay("Pipe.wav");
	AnimationRender->On();
	AnimationRender->SetOrder(static_cast<int>(RenderOrder::BackGround));
	SetPos(_PipePos);
	StateValue = MarioState::Pipe;
	Timer = 100;
	MoveDir = _Dir;

	if (0 < _Dir.X || 0 > _Dir.Y)
	{
		ChangeAnimation("Walk");
	}
	else
	{
		ChangeAnimation("Pipe");
	}
}

void Mario::PipeOut(const float4& _Dir)
{
	GameEngineResources::GetInst().SoundPlay("Pipe.wav");
	AnimationRender->SetOrder(static_cast<int>(RenderOrder::BackGround));
	StateValue = MarioState::Pipe;
	Timer = PipeOutTime;
	MoveDir = _Dir;

	if (0 < _Dir.X || 0 > _Dir.X)
	{
		ChangeAnimation("Walk");
	}
	else
	{
		ChangeAnimation("Pipe");
	}
}

void Mario::AddScore(int _Score)
{
	CurrentLevel->AddScore(_Score);
}


Mario::Mario() 
{
	if (MainPlayer != nullptr)
	{
		MsgAssert("두번째 마리오가 생성되었습니다");
	}
	MainPlayer = this;
}


Mario::~Mario() 
{
}


void Mario::Start()
{
	// 마리오의 기록 데이터를 불러온다
	ContentCore& GameCore = ContentCore::GetInst();
	MarioPower = GameCore.GetMarioStateData();

	// 렌더 생성
	{
		AnimationRender = CreateRender(RenderOrder::Player);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->SetPosition(RenderPos);

		AnimationRender->CreateAnimation({ .AnimationName = "Goal", .ImageName = "LEFT_MARIO.BMP", .Start = 23, .End = 23, });
		AnimationRender->CreateAnimation({ .AnimationName = "Super_Goal", .ImageName = "LEFT_MARIO.BMP", .Start = 23 + 53, .End = 23 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Fire_Goal", .ImageName = "LEFT_MARIO.BMP", .Start = 23 + 103, .End = 23 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "GameOver1", .ImageName = "LEFT_MARIO.BMP", .Start = 48, .End = 48, });
		AnimationRender->CreateAnimation({ .AnimationName = "GameOver2", .ImageName = "LEFT_MARIO.BMP", .Start = 48, .End = 49, .InterTime = 0.1f });

		// 오른쪽 기본
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 0, .End = 0 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 1, .End = 1 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 2, .End = 2 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_ClearWalk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 6, .End = 8, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 9, .End = 9, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Pipe", .ImageName = "RIGHT_MARIO.BMP", .Start = 10, .End = 10, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 11, .End = 11, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 12, .End = 12, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 13, .End = 13, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Spin", .ImageName = "RIGHT_MARIO.BMP", .Start = 14, .End = 16, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Slide", .ImageName = "RIGHT_MARIO.BMP", .Start = 18, .End = 18, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Kick", .ImageName = "RIGHT_MARIO.BMP", .Start = 19, .End = 19, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Grow", .ImageName = "RIGHT_MARIO.BMP", .InterTime = 0.07f, .Loop = false, .FrameIndex = {50, 51, 50, 51, 50, 51, 52, 51, 52, 51, 52}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Shrink", .ImageName = "RIGHT_MARIO.BMP", .InterTime = 0.07f, .Loop = false, .FrameIndex = {52, 51, 52, 51, 52, 51, 50, 51, 50, 51, 50}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_GetFire", .ImageName = "RIGHT_MARIO.BMP", .InterTime = 0.1f, .FrameIndex = {103, 53}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 24, .End = 24, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 24, .End = 24, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 25, .End = 25, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 26, .End = 26, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28, .End = 28 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28, .End = 28 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Hold_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 28, .End = 28 });
		// 오른쪽 슈퍼
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 0 + 53, .End = 0 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 1 + 53, .End = 1 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 2 + 53, .End = 2 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_ClearWalk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 6 + 53, .End = 8 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 9 + 53, .End = 9 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Pipe", .ImageName = "RIGHT_MARIO.BMP", .Start = 10 + 53, .End = 10 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 11 + 53, .End = 11 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 12 + 53, .End = 12 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 13 + 53, .End = 13 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Spin", .ImageName = "RIGHT_MARIO.BMP", .Start = 14 + 53, .End = 16 + 53, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Slide", .ImageName = "RIGHT_MARIO.BMP", .Start = 18 + 53, .End = 18 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Kick", .ImageName = "RIGHT_MARIO.BMP", .Start = 19 + 53, .End = 19 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_PowerUp", .ImageName = "RIGHT_MARIO.BMP", .FrameIndex = {53, 104} });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 24 + 53, .End = 24 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 24 + 53, .End = 24 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 25 + 53, .End = 25 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 26 + 53, .End = 26 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Super_Hold_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		// 오른쪽 불꽃
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 0 + 103, .End = 0 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 1 + 103, .End = 1 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 2 + 103, .End = 2 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_ClearWalk", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 6 + 103, .End = 8 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 9 + 103, .End = 9 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Pipe", .ImageName = "RIGHT_MARIO.BMP", .Start = 10 + 103, .End = 10 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 11 + 103, .End = 11 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 12 + 103, .End = 12 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 13 + 103, .End = 13 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Spin", .ImageName = "RIGHT_MARIO.BMP", .Start = 14 + 103, .End = 16 + 103, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Slide", .ImageName = "RIGHT_MARIO.BMP", .Start = 18 + 103, .End = 18 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Kick", .ImageName = "RIGHT_MARIO.BMP", .Start = 19 + 103, .End = 19 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_PowerUp", .ImageName = "RIGHT_MARIO.BMP", .FrameIndex = {104, 104} });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Attack", .ImageName = "RIGHT_MARIO.BMP", .Start = 152, .End = 152 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_JumpAttack", .ImageName = "RIGHT_MARIO.BMP", .Start = 32 + 103, .End = 32 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Idle", .ImageName = "RIGHT_MARIO.BMP", .Start = 24 + 103, .End = 24 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Brake", .ImageName = "RIGHT_MARIO.BMP", .Start = 24 + 103, .End = 24 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_LookUp", .ImageName = "RIGHT_MARIO.BMP", .Start = 25 + 103, .End = 25 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Crouch", .ImageName = "RIGHT_MARIO.BMP", .Start = 26 + 103, .End = 26 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Walk", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Dash", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Run", .ImageName = "RIGHT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Jump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_RunJump", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Fire_Hold_Fall", .ImageName = "RIGHT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
		// 왼쪽 기본
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 0, .End = 0 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 1, .End = 1 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 2, .End = 2 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_ClearWalk", .ImageName = "LEFT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 3, .End = 5, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 6, .End = 8, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 9, .End = 9, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Pipe", .ImageName = "LEFT_MARIO.BMP", .Start = 10, .End = 10, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 11, .End = 11, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 12, .End = 12, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 13, .End = 13, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Spin", .ImageName = "LEFT_MARIO.BMP", .Start = 14, .End = 16, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Slide", .ImageName = "LEFT_MARIO.BMP", .Start = 18, .End = 18, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Kick", .ImageName = "LEFT_MARIO.BMP", .Start = 19, .End = 19, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Grow", .ImageName = "Left_MARIO.BMP", .InterTime = 0.07f, .Loop = false, .FrameIndex = {50, 51, 50, 51, 50, 51, 52, 51, 52, 51, 52}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Shrink", .ImageName = "Left_MARIO.BMP", .InterTime = 0.07f, .Loop = false, .FrameIndex = {52, 51, 52, 51, 52, 51, 50, 51, 50, 51, 50}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_GetFire", .ImageName = "Left_MARIO.BMP", .InterTime = 0.1f, .FrameIndex = {103, 53}, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 24, .End = 24, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 24, .End = 24, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 25, .End = 25, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 26, .End = 26, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 27, .End = 29, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 28, .End = 28 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 28, .End = 28 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Hold_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 28, .End = 28 });
		// 오른쪽 슈퍼
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 0 + 53, .End = 0 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 1 + 53, .End = 1 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 2 + 53, .End = 2 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_ClearWalk", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 53, .End = 5 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 6 + 53, .End = 8 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 9 + 53, .End = 9 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Pipe", .ImageName = "LEFT_MARIO.BMP", .Start = 10 + 53, .End = 10 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 11 + 53, .End = 11 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 12 + 53, .End = 12 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 13 + 53, .End = 13 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Spin", .ImageName = "LEFT_MARIO.BMP", .Start = 14 + 53, .End = 16 + 53, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Slide", .ImageName = "LEFT_MARIO.BMP", .Start = 18 + 53, .End = 18 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Kick", .ImageName = "LEFT_MARIO.BMP", .Start = 19 + 53, .End = 19 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_PowerUp", .ImageName = "LEFT_MARIO.BMP", .FrameIndex = {53, 104} });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 24 + 53, .End = 24 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 24 + 53, .End = 24 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 25 + 53, .End = 25 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 26 + 53, .End = 26 + 53, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 53, .End = 29 + 53, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Super_Hold_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 53, .End = 28 + 53 });
		// 오른쪽 불꽃
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 0 + 103, .End = 0 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 1 + 103, .End = 1 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 2 + 103, .End = 2 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.08f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_ClearWalk", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.2f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 3 + 103, .End = 5 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 6 + 103, .End = 8 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 9 + 103, .End = 9 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Pipe", .ImageName = "LEFT_MARIO.BMP", .Start = 10 + 103, .End = 10 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 11 + 103, .End = 11 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 12 + 103, .End = 12 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 13 + 103, .End = 13 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Spin", .ImageName = "LEFT_MARIO.BMP", .Start = 14 + 103, .End = 16 + 103, .InterTime = 0.035f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Slide", .ImageName = "LEFT_MARIO.BMP", .Start = 18 + 103, .End = 18 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Kick", .ImageName = "LEFT_MARIO.BMP", .Start = 19 + 103, .End = 19 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_PowerUp", .ImageName = "LEFT_MARIO.BMP", .FrameIndex = {104, 104} });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Attack", .ImageName = "LEFT_MARIO.BMP", .Start = 152, .End = 152 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_JumpAttack", .ImageName = "LEFT_MARIO.BMP", .Start = 32 + 103, .End = 32 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Idle", .ImageName = "LEFT_MARIO.BMP", .Start = 24 + 103, .End = 24 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Brake", .ImageName = "LEFT_MARIO.BMP", .Start = 24 + 103, .End = 24 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_LookUp", .ImageName = "LEFT_MARIO.BMP", .Start = 25 + 103, .End = 25 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Crouch", .ImageName = "LEFT_MARIO.BMP", .Start = 26 + 103, .End = 26 + 103, });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Walk", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.06f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Dash", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.03f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Run", .ImageName = "LEFT_MARIO.BMP", .Start = 27 + 103, .End = 29 + 103, .InterTime = 0.02f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Jump", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_RunJump", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Fire_Hold_Fall", .ImageName = "LEFT_MARIO.BMP", .Start = 28 + 103, .End = 28 + 103 });
	}
	ChangeAnimation("Idle");

	// Collision 생성
	{
		Collision = CreateCollision(CollisionOrder::Player);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::Rect);

		FootCollision = CreateCollision(CollisionOrder::Check);
		FootCollision->SetScale(FootCollisionScale);
		FootCollision->SetPosition(FootCollisionPos);
		FootCollision->SetDebugRenderType(CollisionType::Rect);
	}

	ScreenSize = GameEngineWindow::GetScreenSize();
}


void Mario::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
	CurrentLevel = dynamic_cast<StageLevel*>(GetLevel());
	MarioPower = ContentCore::GetInst().GetMarioStateData();
	MapSize = Map::MainMap->GetStageSize();
	CameraMove(1);
}


void Mario::LevelChangeEnd(GameEngineLevel* _NextLevel)
{

	ContentCore::GetInst().SetMarioStateData(MarioPower);
}


void Mario::Update(float _DeltaTime)
{
	if (MarioState::VICTORY == StateValue)
	{
		VictoryUpdate(_DeltaTime);
		return;
	}
	FireAnimTimer += _DeltaTime;
	KickAnimTimer += _DeltaTime;
	UpdateState(_DeltaTime);

	// 시간 멈춘 상태 체크
	if (MarioState::CHANGEPOWER == StateValue || MarioState::GameOver == StateValue || MarioState::Pipe == StateValue)
	{
		return;
	}
	CheckCollision();

	// 시간 멈춘 상태 체크
	if (MarioState::CHANGEPOWER == StateValue || MarioState::GameOver == StateValue)
	{
		return;
	}

	BlockCheck();
	MoveCalculation(_DeltaTime);
	KickAttack();
	FireAttack();
	CameraMove(_DeltaTime);



	// 무적시간 체크
	if (true == IsInvincibility && MarioState::CHANGEPOWER != StateValue)
	{
		Timer -= _DeltaTime;
		if (static_cast<int>((Timer * 20)) % 2 == 0)
		{
			AnimationRender->On();
		}
		else
		{
			AnimationRender->Off();
		}
		if (0 >= Timer)
		{
			AnimationRender->On();
			IsInvincibility = false;
		}
	}

	// 치트
	if (GameEngineInput::IsDown("U"))
	{
		Speed *= 2;
		RunSpeed *= 2;
		IsInvincibility = true;
		Timer = 9999;
		ColMap = GameEngineResources::GetInst().TextureFind("STAGE0COL.bmp");
	}
	if (GameEngineInput::IsDown("I"))
	{
		Speed = 705;
		RunSpeed = 850.0f;
		ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
	}
	if (GameEngineInput::IsDown("P"))
	{
		LevelLoader::ChangeLevel("World");
	}

}

void Mario::ChangeAnimation(const std::string_view& _AnimationName)
{
	std::string AnimStr = DirValue == Dir::Left ? "Left_" : "Right_";

	switch (MarioPower)
	{
	case PowerState::Normal:
		break;
	case PowerState::Super:
		AnimStr += "Super_";
		break;
	case PowerState::Fire:
		AnimStr += "Fire_";
		break;
	default:
		break;
	}

	AnimStr += IsHold ? "Hold_" : "";

	AnimationRender->ChangeAnimation(AnimStr + _AnimationName.data());
}


void Mario::MoveCalculation(float _DeltaTime)
{
	if (MarioState::GameOver == StateValue) { return; }
	// 밑의 위치
	float4 DownPos = GetPos() + float4::DOWN * 10;
	// 앞의 위치
	float4 ForwardPos = GetPos();
	ForwardPos.X += HorizontalForce * Speed * _DeltaTime;
	// 위의 위치
	float4 UpPos = GetPos() + float4::UP * 64;
	// MoveDir 중력 증가
	MoveDir += float4::DOWN * GravityAcceleration * _DeltaTime;

	// 현재 중력이 최대 중력을 초과한 경우
	if (GravityMax < MoveDir.Y)
	{
		// 최대 중력 제한
		MoveDir.Y = GravityMax;
	}
	// 공중에 있으면서 중력에 의해 떨어지고 있는 경우
	else if (false == IsGrounded && 0 < MoveDir.Y)
	{
		if (MarioState::SPIN != StateValue && MarioState::RUNJUMP != StateValue)
		{
			ChangeState(MarioState::FALL);
		}
	}

	// MoveDir 좌우 이동 증가
	if (MarioState::RUN == StateValue || MarioState::RUNJUMP == StateValue)
	{
		MoveDir.X = HorizontalForce * RunSpeed;
	}
	else
	{
		MoveDir.X = HorizontalForce * Speed;
	}

	// 이번 프레임에 이동할 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	// 맵 이탈 방지
	if (0 >= NextPos.iX())
	{
		MoveDir.X = 0;
	}
	else if (Map::MainMap->GetStageSize().iX() <= NextPos.iX())
	{
		MoveDir.X = 0;
	}
	if (0 >= NextPos.iY())
	{
		MoveDir.Y = 0;
	}
	else if (Map::MainMap->GetStageSize().iY() + 100 <= NextPos.iY())
	{
		MarioPower = PowerState::Normal;
		Die();
		MoveDir.Y = 0;
	}

	// 맵 충돌 체크

	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(DownPos, White);
	// 이전까지 비탈길에 있던 경우
	if (true == IsSlope && true == IsGrounded)
	{
		// 내 밑에 비탈길이 있는 경우
		if (Red == PixelColor)
		{
			IsSlope = true;
			IsGrounded = true;
			DownPos.Y = std::round(DownPos.Y);
			// 바닥에서 제일 위로 올라간다
			while (true)
			{
				DownPos.Y -= 1;
				PixelColor = ColMap->GetPixelColor(DownPos, Black);
				if (White == PixelColor)
				{
					SetPos(DownPos);
					MoveDir.Y = 0.0f;
					break;
				}
			}
			DownPos.X += 1;
			PixelColor = ColMap->GetPixelColor(DownPos, Black);
			// 경사로가 왼쪽
			if (Red == PixelColor)
			{
				SlopeDir = Dir::Left;
			}
			// 경사로가 오른쪽
			else
			{
				SlopeDir = Dir::Right;
			}
		}
		// 이젠 내 밑에 비탈길이 없는 경우
		else
		{
			// 바닥 체크
			if (Black == PixelColor || Green == PixelColor)
			{
				IsSlope = false;
				if (MarioState::FALL == StateValue || (MarioState::SPIN == StateValue && 0 < MoveDir.Y) || (MarioState::RUNJUMP == StateValue && 0 < MoveDir.Y))
				{
					IsGrounded = true;
					DownPos.Y = std::round(DownPos.Y);
					// 바닥에서 제일 위로 올라간다
					while (true)
					{
						DownPos.Y -= 1;
						PixelColor = ColMap->GetPixelColor(DownPos, Black);
						if (Black != PixelColor)
						{
							SetPos(DownPos);
							ForwardPos.Y = DownPos.Y;
							MoveDir.Y = 0.0f;
							break;
						}
					}

				}
				else if (MarioState::JUMP != StateValue && MarioState::SPIN != StateValue && MarioState::RUNJUMP != StateValue)
				{
					MoveDir.Y = 0.0f;
				}

			}
			else
			{
				StateValue;
				IsSlope = false;
				IsGrounded = false;
			}
		}
	}
	// 바닥 체크
	else if (Black == PixelColor)
	{
		IsSlope = false;
		if (MarioState::FALL == StateValue || (MarioState::SPIN == StateValue && 0 < MoveDir.Y) || (MarioState::RUNJUMP == StateValue && 0 < MoveDir.Y))
		{
			IsGrounded = true;
			DownPos.Y = std::round(DownPos.Y);
			// 바닥에서 제일 위로 올라간다
			while (true)
			{
				DownPos.Y -= 1;
				PixelColor = ColMap->GetPixelColor(DownPos, Black);
				if (Black != PixelColor)
				{
					SetPos(DownPos);
					MoveDir.Y = 0.0f;
					break;
				}
			}

		}
		else if (MarioState::JUMP != StateValue && MarioState::SPIN != StateValue && MarioState::RUNJUMP != StateValue)
		{
			MoveDir.Y = 0.0f;
		}
	}
	// 점프중이 아닌경우
	else if (MarioState::JUMP != StateValue && (MarioState::SPIN != StateValue || 0 < MoveDir.Y) && (MarioState::RUNJUMP != StateValue || 0 < MoveDir.Y))
	{
		// 아래에서 통과되는 블록들 체크 ex) 구름
		if (Green == PixelColor)
		{
			IsSlope = false;
			if (MarioState::FALL == StateValue || (MarioState::SPIN == StateValue && 0 < MoveDir.Y) || (MarioState::RUNJUMP == StateValue && 0 < MoveDir.Y))
			{
				IsGrounded = true;
				DownPos.Y = std::round(DownPos.Y);
				// 바닥에서 제일 위로 올라간다
				while (true)
				{
					DownPos.Y -= 1;
					PixelColor = ColMap->GetPixelColor(DownPos, Black);
					if (White == PixelColor)
					{
						SetPos(DownPos);
						MoveDir.Y = 0.0f;
						break;
					}
				}

			}
			else if (MarioState::RUNJUMP != StateValue && MarioState::JUMP != StateValue && MarioState::SPIN != StateValue)
			{
				MoveDir.Y = 0.0f;
			}
		}
		// 비탈길 체크
		else if (Red == PixelColor)
		{
			IsSlope = true;
			IsGrounded = true;
			DownPos.Y = std::round(DownPos.Y);
			// 바닥에서 제일 위로 올라간다
			while (true)
			{
				DownPos.Y -= 1;
				PixelColor = ColMap->GetPixelColor(DownPos, Black);
				if (White == PixelColor)
				{
					SetPos(DownPos);
					ForwardPos.Y = DownPos.Y;
					MoveDir.Y = 0.0f;
					break;
				}
			}
			DownPos.X += 1;
			PixelColor = ColMap->GetPixelColor(DownPos, Black);
			// 경사로가 왼쪽
			if (Red == PixelColor)
			{
				SlopeDir = Dir::Left;
			}
			// 경사로가 오른쪽
			else
			{
				SlopeDir = Dir::Right;
			}
		}
		// 이전까지 바닥에 있었으나 현제 공중에 있는경우
		else if (true == IsGrounded && false == IsOnBlock)
		{
			IsSlope = false;
			ChangeState(MarioState::FALL);
			IsGrounded = false;
		}
	}

	// 천장 체크
	if (Black == ColMap->GetPixelColor(UpPos, White))
	{
		if (0 > MoveDir.Y)
		{
			GameEngineResources::GetInst().SoundPlay("bump.wav");
			MoveDir.Y = HeadingReaction;
		}
	}
	// 벽 체크
	if (Black == ColMap->GetPixelColor(ForwardPos, White))
	{
		if (0 < HorizontalForce)
		{
			int i = 0;
			while (true)
			{
				if (20 < ++i) {
					break;
				}
				// 자꾸 무한 반복함
				ForwardPos.X = std::round(ForwardPos.X);
				ForwardPos.X -= 1;
				PixelColor = ColMap->GetPixelColor(ForwardPos, Black);
				if (Black != PixelColor)
				{
					SetPos(ForwardPos);
					MoveDir.X = 0.0f;
					break;
				}
			}
		}
		else
		{
			int i = 0;
			while (true)
			{
				if (20 < ++i) {
					break;
				}
				ForwardPos.X = std::round(ForwardPos.X);
				ForwardPos.X += 1;
				PixelColor = ColMap->GetPixelColor(ForwardPos, Black);
				if (Black != PixelColor)
				{
					SetPos(ForwardPos);
					MoveDir.X = 0.0f;
					break;
				}
			}
		}

		HorizontalForce = 0;
		MoveDir.X = 0;
		if (true == IsGrounded)
		{
			ChangeState(MarioState::IDLE);
		}
	}

	// 마리오 이동
	SetMove(MoveDir * _DeltaTime);
}


void Mario::CameraMove(float _DeltaTime)
{
	float4 CameraPos = GetPos();
	CameraPos.X -= 350;

	if (750 > GetPos().Y)
	{
		CameraPos.Y -= 550;
		CameraPos.Y = std::lerp(GetLevel()->GetCameraPos().Y, CameraPos.Y, _DeltaTime);

	}
	else
	{
		CameraPos.Y = 850;
		CameraPos.Y = std::lerp(GetLevel()->GetCameraPos().Y, CameraPos.Y, _DeltaTime);
	}


	if (CameraPos.X < 0)
	{
		CameraPos.X = 0;
	}
	else if (MapSize.X - ScreenSize.X < CameraPos.X)
	{
		CameraPos.X = MapSize.X - ScreenSize.X;
	}

	if (CameraPos.Y < 0)
	{
		CameraPos.Y = 0;
	}
	else if (MapSize.Y - ScreenSize.Y < CameraPos.Y)
	{
		CameraPos.Y = MapSize.Y - ScreenSize.Y;
	}

	GetLevel()->SetCameraPos(CameraPos);
}

void Mario::GetDamaged()
{
	switch (MarioPower)
	{
	case PowerState::Normal:
		ChangeState(MarioState::GameOver);
		return;
	case PowerState::Super:
		MarioPower = PowerState::Normal;
		ChangeState(MarioState::CHANGEPOWER);
		break;
	case PowerState::Fire:
		MarioPower = PowerState::Normal;
		ChangeState(MarioState::CHANGEPOWER);
		break;
	default:
		break;
	}
	GameEngineResources::GetInst().SoundPlay("damage.wav");

}

void Mario::FireAttack()
{
	// 파이어 마리오 상태일때만 실행
	if (PowerState::Fire != MarioPower) { return; }
	if (MarioState::SPIN == StateValue || MarioState::CROUCH == StateValue || MarioState::SLIDE == StateValue) { return; }
	if (FireAnimTime > FireAnimTimer && false == IsHold)
	{
		if (true == IsGrounded)
		{
			ChangeAnimation("Attack");
		}
		else
		{
			ChangeAnimation("JumpAttack");
		}
	}
	// 대시 키를 입력한 순간에만 실행
	if (false == GameEngineInput::IsDown("Dash")) { return; }
	if (true == Fire::IsMax()) { return; }

	GameEngineResources::GetInst().SoundPlay("fireball.wav");
	Fire* NewFire = GetLevel()->CreateActor<Fire>(RenderOrder::PlayerAttack);
	NewFire->SetPos(GetPos());
	NewFire->SetMove(float4::UP * FireCreatePos.Y);
	if (Dir::Left == DirValue)
	{
		NewFire->SetMove(float4::LEFT * FireCreatePos.X);
		NewFire->SetDir(float4::LEFT);
	}
	else
	{
		NewFire->SetMove(float4::RIGHT * FireCreatePos.X);
		NewFire->SetDir(float4::RIGHT);
	}

	FireAnimTimer = 0;
}

void Mario::NewPower(PowerState _Power)
{
	if (MarioPower == _Power)
	{
		switch (_Power)
		{
		case PowerState::Super:
			CurrentLevel->NewStockItem(ItemType::SuperMushroom);
			break;
		case PowerState::Fire:
			CurrentLevel->NewStockItem(ItemType::FireFlower);
			break;
		default:
			break;
		}
		return;
	}

	if (PowerState::Super == _Power)
	{
		if (PowerState::Normal != MarioPower)
		{
			CurrentLevel->NewStockItem(ItemType::SuperMushroom);
			return;
		}
	}
	else
	{
		switch (MarioPower)
		{
		case PowerState::Super:
			CurrentLevel->NewStockItem(ItemType::SuperMushroom);
			break;
		case PowerState::Fire:
			CurrentLevel->NewStockItem(ItemType::FireFlower);
			break;
		default:
			break;
		}

	}

	MarioPower = _Power;
	GameEngineResources::GetInst().SoundPlay("powerup.wav");
	ChangeState(MarioState::CHANGEPOWER);
}

void Mario::KickAttack()
{
	if (KickAnimTime > KickAnimTimer && false == IsHold)
	{
		ChangeAnimation("Kick");
	}
	if (true == IsHold)
	{
		if (false == GameEngineInput::IsPress("Dash"))
		{
			IsHold = false;
			float4 KickForce;

			if (GameEngineInput::IsPress("Down"))
			{
				KickForce.X = DirValue == Dir::Left ? -250.0f : 250.0f;
				KickForce.Y = 100;
			}
			else
			{
				if (GameEngineInput::IsPress("Up")) {
					KickForce.Y = -1500;
				}
				else
				{
					KickForce.X = DirValue == Dir::Left ? -1000.0f : 1000.0f;
					KickForce.Y = -250;
				}
				float4 Pos = GetPos();
				Pos.X += DirValue == Dir::Right ? HoldPos.X : -HoldPos.X;
				Pos.Y += HoldPos.Y;
				Particle::CreateParticle(GetLevel(), Pos, "KICK");
				GameEngineResources::GetInst().SoundPlay("kick.wav");
				KickAnimTimer = 0;
			}

			if (nullptr != HoldActor)
			{
				HoldActor->Kick(KickForce);
			}
		}
		else
		{
			float4 MovePos = GetPos();
			MovePos.X += DirValue == Dir::Right ? HoldPos.X : -HoldPos.X;
			MovePos.Y += HoldPos.Y;
			if (nullptr != HoldActor)
			{
				HoldActor->SetPos(MovePos);
			}
		}
	}
}

void Mario::CheckCollision()
{
	std::vector<GameEngineCollision*> Collisions;
	// 몬스터 체크
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Monster), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		bool IsKill = false;
		for (; Start != End; Start++)
		{
			EnemyActor* ColActor = (*Start)->GetOwner<EnemyActor>();
			//ColActor->GetOwner<Mario>(); 전환 방식

			// 슬라이딩을 하고있는 경우
			if (MarioState::SLIDE == StateValue)
			{
				// 몬스터 처치
				ColActor->Death();
				Particle::CreateParticle(GetLevel(), GetPos(), "KICK");
				GameEngineResources::GetInst().SoundPlay("kick.wav");
				continue;
			}
			if (true == ColActor->IsHold())
			{
				continue;
			}
			// 잡기
			if (true == ColActor->IsHoldable() && MarioState::SPIN != StateValue && false == IsHold && KickAnimTime < KickAnimTimer)
			{
				if (true == GameEngineInput::IsPress("Dash"))
				{
					// 잡는다
					IsHold = true;
					ColActor->Hold();
					HoldActor = ColActor;
					continue;
				}
				else
				{
					//ColActor->Kick(float4::Right * (DirValue == Dir::Left ? -1000.0f : 1000.0f));
				}
			}
			// 플레이어가 몬스터보다 위에 있으면서 떨어지고 있는 경우
			if (GetPos().Y < ColActor->GetPos().Y - 28 && -500 < MoveDir.Y)
			{
				if (IsKill == true) {
					continue;
				}
				switch (StateValue)
				{
				case MarioState::FALL:
				case MarioState::RUNJUMP:
					ColActor->JumpHit(GetPos().X > ColActor->GetPos().X);
					break;
				case MarioState::SPIN:
					ColActor->SpinHit();
					break;
				case MarioState::SLIDE:

					break;
				default:
					ColActor->JumpHit(GetPos().X > ColActor->GetPos().X);
					break;
				}

				Particle::CreateParticle(GetLevel(), GetPos(), "KICK");
				GameEngineResources::GetInst().SoundPlay("kick.wav");

				IsKill = true;

			}
			// 그 외 무적 시간이 아닌 경우 대미지
			else
			{
				if (true == ColActor->IsCollisionAttack())
				{
					if (false == IsInvincibility && false == IsKill)
					{
						// 대미지
						GetDamaged();
						IsInvincibility = true;
					}
				}
				else if (true == ColActor->IsHoldable())
				{
					Particle::CreateParticle(GetLevel(), ColActor->GetPos(), "KICK");
					GameEngineResources::GetInst().SoundPlay("kick.wav");
					KickAnimTimer = 0;
					ColActor->Kick(float4::RIGHT * (DirValue == Dir::Left ? -1000.0f : 1000.0f));
				}
			}
		}
		if (true == IsKill)
		{
			MoveDir.Y = 0;
			IsGrounded = false;
			JumpTimeCounter = JumpTime;
			MoveDir += float4::UP * JumpForce;
			if (MarioState::RUNJUMP != StateValue && MarioState::SPIN != StateValue)
			{
				StateValue = MarioState::JUMP;
			}
		}
	}
}

void Mario::BlockCheck()
{
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = Rect, .ThisColType = Rect };
	if (true == FootCollision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		bool IsHeading = false;
		bool IsReJump = false;
		float4 Pos = GetPos();
		for (; Start != End; Start++)
		{
			Block* ColBlock = (*Start)->GetOwner<Block>();
			if (true == ColBlock->GetIsRoll())
			{
				continue;
			}
			float4 ColPos = ColBlock->GetPos();
			// 엑터가 블록위에 서있다
			if (Pos.Y < ColPos.Y - BlockYSize)
			{
				if (0 >= MoveDir.Y)
				{
					continue;
				}
				// 파워업 상태에서 스핀으로 착지시
				if (MarioState::SPIN == StateValue && PowerState::Normal != MarioPower)
				{
					// 블록 파괴 및 다시 점프
					if (true == ColBlock->Damage())
					{
						IsReJump = true;
						continue;
					}
				}
				// 그 외에 경우 블록위로 착지
				IsSlope = false;
				IsGrounded = true;
				IsOnBlock = true;
				float4 Pos = GetPos();
				Pos.Y = ColPos.Y - BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 0.0f;
				continue;
			}
			else if (Pos.Y > ColPos.Y + BlockYSize)
			{
				if (0 <= MoveDir.Y)
				{
					continue;
				}
				IsHeading = true;
				ColBlock->Hit();

			}
			else if (Pos.X + FootCollisionScale.hX() < ColPos.X - BlockXSize)
			{
				float4 Pos = GetPos();
				Pos.X = ColPos.X - BlockXSize - FootCollisionScale.hX() - 2;
				Pos.X = std::round(Pos.X);

				SetPos(Pos);
				if (true == IsGrounded)
				{
					ChangeState(MarioState::IDLE);
				}
				HorizontalForce = 0;
				MoveDir.X = 0;
			}
			else if (ColPos.X + BlockXSize < Pos.X - FootCollisionScale.hX())
			{
				float4 Pos = GetPos();
				Pos.X = ColPos.X + BlockXSize + FootCollisionScale.hX() + 2;
				Pos.X = std::round(Pos.X);

				SetPos(Pos);
				if (true == IsGrounded)
				{
					ChangeState(MarioState::IDLE);
				}
				HorizontalForce = 0;
				MoveDir.X = 0;
			}
		}
		if (true == IsReJump)
		{
			IsGrounded = false;
			JumpTimeCounter = JumpTime;
			MoveDir.Y = 0;
			MoveDir.Y -= JumpForce;
		}
		if (true == IsHeading) {
			GameEngineResources::GetInst().SoundPlay("bump.wav");
			MoveDir.Y = HeadingReaction;
		}
	}
	else if (true == IsOnBlock)
	{
		IsOnBlock = false;
		IsGrounded = false;
	}
}

void Mario::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
		FootCollision->DebugRender();
	}
}