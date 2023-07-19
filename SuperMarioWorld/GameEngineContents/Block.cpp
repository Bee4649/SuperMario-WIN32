#include "Block.h"
#include <GameEngineCore/GameEngineResources.h>
#include "Particle.h"
#include "EnemyActor.h"
#include "ItemActor.h"
#include "MarioGameCore.h"
Block::Block() {

}

Block::~Block() {

}

bool Block::Damage()
{
	GameEngineResources::GetInst().SoundPlay("breakblock.wav");
	Particle::CreateMovingParticle(GetLevel(), GetPos(), BlockdebritsForce, "Blockdebrits", true, false, BlockdebritsLiveTime);
	Death();
	return true;
}

void Block::Hit()
{
	IsHit = true;
	HitTimer = HitTime2;

	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Monster), .TargetColType = CT_Rect,  .ThisColType = CT_Rect };
	if (true == HitCollision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			EnemyActor* ColActor = (*Start)->GetOwner<EnemyActor>();
			ColActor->BlockHit();
		}
	}
	Check = { .TargetGroup = static_cast<int>(CollisionOrder::Item), .TargetColType = CT_Rect,  .ThisColType = CT_Rect };
	if (true == HitCollision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			ItemActor* ColActor = (*Start)->GetOwner<ItemActor>();
			ColActor->BlockHit();
		}
	}
}

void Block::Start()
{
	// Collision »ý¼º
	{
		Collision = CreateCollision(CollisionOrder::Block);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::CT_Rect);

		HitCollision = CreateCollision(CollisionOrder::Check);
		HitCollision->SetScale(HitCollisionScale);
		HitCollision->SetPosition(HitCollisionPos);
		HitCollision->SetDebugRenderType(CollisionType::CT_Rect);
	}
}

void Block::Update(float _DeltaTime)
{
	if (true == IsHit)
	{
		HitTimer -= _DeltaTime;
		if (0 > HitTimer)
		{
			IsHit = false;
			AnimationRender->SetPosition(RenderPos);
			HitAnimEnd();
			return;
		}
		else if (HitTime > HitTimer)
		{
			AnimationRender->SetMove(float4::Down * HitAnimSpeed * _DeltaTime);
		}
		else
		{
			AnimationRender->SetMove(float4::Up * HitAnimSpeed * _DeltaTime);
		}
	}
	
}

void Block::Render(float _DeltaTime)
{
	if (true == MarioGameCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
		HitCollision->DebugRender();
	}
}
