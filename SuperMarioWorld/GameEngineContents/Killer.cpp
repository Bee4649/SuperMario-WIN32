#include "Killer.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include "ContentsEnum.h"

Killer::Killer() 
{
}

Killer::~Killer() 
{
}

bool Killer::IsCollisionAttack()
{
	return false == StateDie;
}

bool Killer::IsHold()
{
	return false;
}

bool Killer::IsHoldable()
{
	return false;
}

bool Killer::IsDie()
{
	return true;
}

void Killer::SpinHit()
{
	StateDie = true;
}

void Killer::JumpHit(bool IsLeft)
{
	StateDie = true;
}

void Killer::FireHit()
{
}

void Killer::BlockHit()
{
}

void Killer::MonsterHit(bool IsLeft)
{
}

void Killer::Hold()
{
}

void Killer::Kick(const float4& _Force)
{
}

void Killer::Start()
{
	EnemyActor::Start();
	SpriteRender = CreateRender("Killer.bmp", RenderOrder::Monster);
	SpriteRender->SetScaleToImage();
	SpriteRender->SetPosition(RenderPos);

	Collision = CreateCollision(CollisionOrder::Monster);
	Collision->SetScale(CollisionScale);
	Collision->SetPosition(CollisionPos);
	Collision->SetDebugRenderType(CollisionType::Rect);
}

void Killer::Update(float _DeltaTime)
{
	if (true == StateDie)
	{
		DieUpdate(_DeltaTime);
		return;
	}

	CameraInCheck();
	if (false == IsOnCamera) { return; }
	SetMove(float4::LEFT * ActorSpeed * _DeltaTime);
}

