#include "Mechakoopa.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"
#include "Map.h"
#include "Block.h"
#include "Particle.h"
#include "Mario.h"
#include "Coin.h"
Mechakoopa::Mechakoopa() {

}

Mechakoopa::~Mechakoopa() {

}

bool Mechakoopa::IsCollisionAttack()
{
	return StateValue == MechaState::Move;
}

bool Mechakoopa::IsHold()
{
	return StateValue == MechaState::Hold || StateValue == MechaState::Die;
}

bool Mechakoopa::IsHoldable()
{
	return StateValue == MechaState::Stun;
}

bool Mechakoopa::IsDie()
{
	return StateValue == MechaState::Die;
}

void Mechakoopa::SpinHit()
{
	GameEngineResources::GetInst().SoundPlay("superstomp.wav");
	Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
	Particle::CreateMovingParticle(GetLevel(), GetPos(), { 450, -450 }, "StarParticle", false, true, 0.5f);
	Death();
}

void Mechakoopa::JumpHit(bool IsLeft)
{
	if (MechaState::Stun == StateValue)
	{
		if (true == IsLeft)
		{
			MoveDir.X -= 250;
		}
		else
		{
			MoveDir.X += 250;
		}
		return;
	}
	if (0 < MoveDir.X)
	{
		AnimationRender->ChangeAnimation("RIGHT_Damage");
	}
	else
	{
		AnimationRender->ChangeAnimation("LEFT_Damage");
	}
	StateValue = MechaState::Kick;
}

void Mechakoopa::FireHit()
{
	if (0 < MoveDir.X)
	{
		AnimationRender->ChangeAnimation("RIGHT_Damage");
	}
	else if (0 > MoveDir.X)
	{
		AnimationRender->ChangeAnimation("LEFT_Damage");
	}
	StateValue = MechaState::Kick;
}

void Mechakoopa::BlockHit()
{
}

void Mechakoopa::MonsterHit(bool IsLeft)
{
	// �ִϸ��̼� ����
	if (0 < DirValue.X)
	{
		AnimationRender->ChangeAnimation("Right_Damage");
	}
	else
	{
		AnimationRender->ChangeAnimation("Left_Damage");
	}

	// ���� ����
	StateValue = MechaState::Die;
	MoveDir = MonsterHitForce;

	// �̵� ����
	if (true == IsLeft)
	{
		MoveDir.X *= -1;
	}
}

void Mechakoopa::Hold()
{
	StateValue = MechaState::Hold;
}

void Mechakoopa::Kick(const float4& _Force)
{
	StateValue = MechaState::Kick;
	MoveDir = _Force;
}

void Mechakoopa::SetPosDir(const float4& _Pos)
{
	SetPos(_Pos);

	if (Mario::MainPlayer->GetPos().X < GetPos().X)
	{
		TurnLeft();
	}
	else
	{
		TurnRight();
	}
}

void Mechakoopa::Start()
{
	EnemyActor::Start();
	Speed = BambaSpeed;
	SlopeSpeed = BambaSlopeSpeed;
	// Render ����
	{
		AnimationRender = CreateRender(RenderOrder::Monster);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->SetPosition(RenderPos);
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Walk", .ImageName = "LEFT_Mechakoopa.bmp", .Start = 0, .End = 3, .InterTime = 0.15f });
		AnimationRender->CreateAnimation({ .AnimationName = "Left_Damage", .ImageName = "LEFT_Mechakoopa.bmp", .Start = 4, .End = 6, .InterTime = 0.15f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Walk", .ImageName = "RIGHT_Mechakoopa.bmp", .Start = 0, .End = 3, .InterTime = 0.15f });
		AnimationRender->CreateAnimation({ .AnimationName = "Right_Damage", .ImageName = "RIGHT_Mechakoopa.bmp", .Start = 4, .End = 6, .InterTime = 0.15f });
		AnimationRender->ChangeAnimation("LEFT_WALK");
	}
	// Collision ����
	{
		Collision = CreateCollision(CollisionOrder::Monster);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::Rect);
	}
	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());

}

void Mechakoopa::Update(float _DeltaTime)
{
	switch (StateValue)
	{
	case MechaState::Spawn:
		break;
	case MechaState::Move:
		MoveUpdate(_DeltaTime);
		break;
	case MechaState::Stun:
		StunUpdate(_DeltaTime);
		break;
	case MechaState::Hold:
		MonsterCheck();
		break;
	case MechaState::Kick:
		KickUpdate(_DeltaTime);
		MonsterCheck();
		break;
	case MechaState::Die:
		DieUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Mechakoopa::Turn()
{
	EnemyActor::Turn();
	if (0 < DirValue.X)
	{
		AnimationRender->ChangeAnimation("Right_Walk");
	}
	else
	{
		AnimationRender->ChangeAnimation("Left_Walk");
	}
}

void Mechakoopa::TurnLeft()
{
	EnemyActor::TurnLeft();
	AnimationRender->ChangeAnimation("Left_Walk");
}

void Mechakoopa::TurnRight()
{
	EnemyActor::TurnRight();
	AnimationRender->ChangeAnimation("Right_Walk");
}

void Mechakoopa::StunUpdate(float _DeltaTime)
{
	// �߷�
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}
	// ����
	if (0.1f > std::abs(MoveDir.X))
	{
		MoveDir.X = 0;
	}
	else if (0 < MoveDir.X)
	{
		MoveDir.X -= _DeltaTime * 500;
	}
	else
	{
		MoveDir.X += _DeltaTime * 500;
	}
	// �浹 �̹��� �˻�
	if (nullptr == ColMap)
	{
		MsgAssert("�浹�� �� �̹����� �����ϴ�.");
	}
	// �̵��� ��ġ
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y + float4::UP.Y;

	// �� �浹 üũ�� �÷� ����
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);
	// �� üũ
	if (Black == PixelColor)
	{
		MoveDir.X = 0;
	}
	// �ٴ� üũ
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				break;
			}
		}
	}
	// �Ʒ����� ����Ǵ� ��ϵ� üũ ex) ����
	else if (Green == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				break;
			}
		}
	}
	// ��Ż�� üũ
	else if (Red == PixelColor)
	{
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				break;
			}
		}

	}

	// ��� üũ
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			Block* ColActor = (*Start)->GetOwner<Block>();
			if (true == ColActor->GetIsRoll())
			{
				continue;
			}
			// ���Ͱ� ��Ϻ��� ���� �ִ� ���
			if (GetPos().Y < ColActor->GetPos().Y - BlockYSize)
			{
				float4 Pos = GetPos();
				Pos.Y = ColActor->GetPos().Y - BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 0.0f;
				continue;
			}
			else if (GetPos().Y > ColActor->GetPos().Y + BlockYSize)
			{
				MoveDir.X = 0;
				continue;
			}
			// �� �� ���
			else
			{
				MoveDir.X = 0;
			}
		}
	}

	SetMove(MoveDir * _DeltaTime);

}

void Mechakoopa::KickUpdate(float _DeltaTime)
{

	// ������ Stun ���·� ��ȯ
	if (0.1f > abs(MoveDir.Y) && 0.1f > abs(MoveDir.X))
	{
		MoveDir.X = 0;
		StateValue = MechaState::Stun;
	}

	// �̵� ����
	if (0 < MoveDir.X)
	{
		MoveDir.X -= _DeltaTime * 1000;
		MoveDir.X = std::max<float>(MoveDir.X - _DeltaTime, 0);
	}
	else if (0 > MoveDir.X)
	{
		MoveDir.X += _DeltaTime * 1000;
		MoveDir.X = std::min<float>(MoveDir.X + _DeltaTime, 0);
	}

	// �߷� ����
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}

	// �浹 �̹��� �˻�
	if (nullptr == ColMap)
	{
		MsgAssert("�浹�� �� �̹����� �����ϴ�.");
	}
	// �̵��� ��ġ
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y + float4::UP.Y;

	// �� �浹 üũ�� �÷� ����
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);

	// �� üũ
	if (Black == PixelColor)
	{
		MoveDir.X = 0;
		return;
	}
	// �ٴ� üũ
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				//StateValue = State::Overturn;
				return;
			}
		}
	}
	// �Ʒ����� ����Ǵ� ��ϵ� üũ ex) ����
	else if (Green == PixelColor && 0 < MoveDir.Y)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				//StateValue = State::Overturn;
				return;
			}
		}
	}
	// ��Ż�� üũ
	else if (Red == PixelColor && 0 < MoveDir.Y)
	{
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos += float4::UP;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = 0;
				return;
			}
		}

	}

	// ��� üũ
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		bool Hit = false;
		for (; Start != End; Start++)
		{
			Block* HitBlock = (*Start)->GetOwner<Block>();
			if (true == HitBlock->GetIsRoll())
			{
				continue;
			}
			// ���Ͱ� ��Ϻ��� ���� �ִ� ���
			if (GetPos().Y < HitBlock->GetPos().Y - BlockYSize)
			{
				// ������� ����
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y - BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 0.0f;
				continue;
			}
			// �ؿ��� ���� ģ ���
			else if (GetPos().Y > HitBlock->GetPos().Y)
			{
				// ����� ��Ʈ�Ѵ�
				Hit = true;
				HitBlock->Hit();
				// ��� ������ �̵��Ѵ�
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y + BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = HeadingReaction;
			}
			// �� �� ��� (������ ģ ���)
			else
			{
				// ���⿡ ���� ��ġ �����Ѵ�

				if (GetPos().X < HitBlock->GetPos().X)
				{
					float4 Pos = GetPos();
					Pos.X = HitBlock->GetPos().X - BlockXSize - RenderScale.hx();
					Pos.X = std::round(Pos.X);
					SetPos(Pos);
				}
				else
				{
					float4 Pos = GetPos();
					Pos.X = HitBlock->GetPos().X + BlockXSize + RenderScale.hx();
					Pos.X = std::round(Pos.X);
					SetPos(Pos);
				}
				// ����� ��Ʈ�Ѵ�
				Hit = true;
				HitBlock->Hit();
			}

		}
		// ����� ��Ʈ�� ��� ȿ���� �� �¿� �̵� ����
		if (true == Hit)
		{
			MoveDir.X = 0;
			GameEngineResources::GetInst().SoundPlay("bump.wav");
			return;
		}
	}




	SetMove(MoveDir * _DeltaTime);

}

void Mechakoopa::MonsterCheck()
{
	// ���� üũ
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Monster), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			EnemyActor* ColActor = (*Start)->GetOwner<EnemyActor>();
			if (this == ColActor)
			{
				continue;
			}
			if (true == ColActor->IsHold())
			{
				continue;
			}

			Mario::MainPlayer->DropHold();
			GameEngineResources::GetInst().SoundPlay("kick.wav");
			Particle::CreateParticle(GetLevel(), GetPos(), "KICK");

			if (ColActor->GetPos().X < GetPos().X)
			{
				ColActor->MonsterHit(true);
				MonsterHit(false);
			}
			else
			{
				ColActor->MonsterHit(false);
				MonsterHit(true);
			}
			return;
		}
	}
}
