#include "Nokonoko.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnums.h"
#include "Map.h"
#include "Block.h"
#include "Particle.h"
#include "Mario.h"
#include "Coin.h"
#include "Shell.h"
Nokonoko::Nokonoko() {

}

Nokonoko::~Nokonoko() {

}

bool Nokonoko::IsCollisionAttack()
{
	return StateValue == NokoState::Normal || StateValue == NokoState::Naked;
}

bool Nokonoko::IsHold()
{
	return false;
}

bool Nokonoko::IsHoldable()
{
	return false;
}

bool Nokonoko::IsDie()
{
	return StateValue == NokoState::Die;
}

void Nokonoko::SpinHit()
{
	GameEngineResources::GetInst().SoundPlay("superstomp.wav");
	Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
	Particle::CreateMovingParticle(GetLevel(), GetPos(), { 450, -450 }, "StarParticle", false, true, 0.5f);
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Nokonoko::JumpHit(bool IsLeft)
{
	StateValue = NokoState::Overturn;
	GetLevel()->CreateActor<Shell>(RenderOrder::Monster)->SetPos(GetPos());
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Nokonoko::FireHit()
{
	GetLevel()->CreateActor<Shell>(RenderOrder::Monster)->SetPos(GetPos());
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Nokonoko::BlockHit()
{
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Nokonoko::MonsterHit(bool IsLeft)
{
	Mario::MainPlayer->AddScore(Score);
	// 애니메이션 변경
	if (0 < DirValue.x)
	{
		AnimationRender->ChangeAnimation("RIGHT_IDLE");
	}
	else
	{
		AnimationRender->ChangeAnimation("LEFT_IDLE");
	}

	// 상태 변경
	StateValue = NokoState::Die;
	MoveDir = MonsterHitForce;

	// 이동 지정
	if (true == IsLeft)
	{
		MoveDir.x *= -1;
	}
}

void Nokonoko::Hold()
{
	
}

void Nokonoko::Kick(const float4& _Force)
{
}

void Nokonoko::Start()
{
	EnemyActor::Start();
	Speed = ActorSpeed;
	SlopeSpeed = ActorSlopeSpeed;
	DirSetting(float4::Left);
	// 렌더 생성
	{
		AnimationRender = CreateRender(RenderOrder::Monster);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_WALK", .ImageName = "LEFT_NOKONOKO.BMP", .Start = 0, .End = 1, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_IDLE", .ImageName = "LEFT_NOKONOKO.BMP", .Start = 0, .End = 0, });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_WALK", .ImageName = "RIGHT_NOKONOKO.BMP", .Start = 0, .End = 1, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_IDLE", .ImageName = "RIGHT_NOKONOKO.BMP", .Start = 0, .End = 0,  });
		AnimationRender->ChangeAnimation("LEFT_WALK");
		AnimationRender->SetPosition(RenderPos);
	}
	// Collision 생성
	{
		Collision = CreateCollision(CollisionOrder::Monster);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::CT_Rect);
	}
}

void Nokonoko::Update(float _DeltaTime)
{
	CameraInCheck();
	if (false == IsOnCamera) {
		return;
	}

	switch (StateValue)
	{
	case NokoState::Normal:
		MoveUpdate(_DeltaTime);
		break;
	case NokoState::Overturn:
		OverturnUpdate(_DeltaTime);
		break;
	case NokoState::Naked:
		MoveUpdate(_DeltaTime);
		break;
	case NokoState::Die:
		DieUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}
void Nokonoko::Turn()
{
	EnemyActor::Turn();
	if (0 < DirValue.x)
	{
		AnimationRender->ChangeAnimation("RIGHT_WALK");
	}
	else
	{
		AnimationRender->ChangeAnimation("LEFT_WALK");
	}
}

void Nokonoko::TurnLeft()
{
	EnemyActor::TurnLeft();
}

void Nokonoko::TurnRight()
{
	EnemyActor::TurnRight();
}

void Nokonoko::OverturnUpdate(float _DeltaTime)
{
}
