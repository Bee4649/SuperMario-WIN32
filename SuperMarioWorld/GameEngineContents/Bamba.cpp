#include "Bamba.h"
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

Bamba::Bamba() 
{
}

Bamba::~Bamba() 
{
}

bool Bamba::IsCollisionAttack()
{
	return StateValue == BambaState::Normal;
}

bool Bamba::IsHold()
{
	return StateValue == BambaState::Hold || StateValue == BambaState::Die;
}

bool Bamba::IsHoldable()
{
	return StateValue == BambaState::Overturn;
}

bool Bamba::IsDie()
{
	return StateValue == BambaState::Die;
}

void Bamba::SpinHit()
{
	GameEngineResources::GetInst().SoundPlay("superstomp.wav");
	Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
	Particle::CreateMovingParticle(GetLevel(), GetPos(), { 450, -450 }, "StarParticle", false, true, 0.5f);
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Bamba::JumpHit(bool IsLeft)
{
	if (BambaState::Overturn == StateValue)
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
		AnimationRender->ChangeAnimation("RIGHT_OVERTURN");
	}
	else
	{
		AnimationRender->ChangeAnimation("LEFT_OVERTURN");
	}
	StateValue = BambaState::Kick;

	//Death();

}

void Bamba::FireHit()
{
	GetLevel()->CreateActor<Coin>(RenderOrder::Item)->SetPos(GetPos());
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Bamba::BlockHit()
{
	Mario::MainPlayer->AddScore(Score);
	Death();
}

void Bamba::MonsterHit(bool IsLeft)
{
	Mario::MainPlayer->AddScore(Score);
	// �ִϸ��̼� ����
	if (0 < DirValue.X)
	{
		if (BambaState::Normal == StateValue)
		{
			AnimationRender->ChangeAnimation("RIGHT_IDLE");
		}
		else
		{
			AnimationRender->ChangeAnimation("RIGHT_OVERTURN_IDLE");
		}

	}
	else
	{
		if (BambaState::Normal == StateValue)
		{
			AnimationRender->ChangeAnimation("LEFT_IDLE");
		}
		else
		{
			AnimationRender->ChangeAnimation("LEFT_OVERTURN_IDLE");
		}
	}

	// ���� ����
	StateValue = BambaState::Die;
	MoveDir = MonsterHitForce;

	// �̵� ����
	if (true == IsLeft)
	{
		MoveDir.X *= -1;
	}
}

void Bamba::Hold()
{
	StateValue = BambaState::Hold;
}

void Bamba::Kick(const float4& _Force)
{
	StateValue = BambaState::Kick;
	MoveDir = _Force;
}

void Bamba::Start()
{
	EnemyActor::Start();
	Speed = BambaSpeed;
	SlopeSpeed = BambaSlopeSpeed;
	DirSetting(float4::LEFT);
	// ���� ����
	{
		AnimationRender = CreateRender(RenderOrder::Monster);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_WALK", .ImageName = "BAMBA.BMP", .Start = 0, .End = 1, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_IDLE", .ImageName = "BAMBA.BMP", .Start = 0, .End = 0, });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_WALK", .ImageName = "BAMBA.BMP", .Start = 2, .End = 3, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_IDLE", .ImageName = "BAMBA.BMP", .Start = 2, .End = 2, });
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_OVERTURN", .ImageName = "BAMBA.BMP", .Start = 4, .End = 5, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "LEFT_OVERTURN_IDLE", .ImageName = "BAMBA.BMP", .Start = 4, .End = 4, });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_OVERTURN", .ImageName = "BAMBA.BMP", .Start = 6, .End = 7, .InterTime = 0.25f });
		AnimationRender->CreateAnimation({ .AnimationName = "RIGHT_OVERTURN_IDLE", .ImageName = "BAMBA.BMP", .Start = 6, .End = 6, });
		AnimationRender->ChangeAnimation("LEFT_WALK");
		AnimationRender->SetPosition(RenderPos);
	}
	// Collision ����
	{
		Collision = CreateCollision(CollisionOrder::Monster);
		Collision->SetScale(CollisionScale);
		Collision->SetPosition(CollisionPos);
		Collision->SetDebugRenderType(CollisionType::Rect);
	}
}

void Bamba::Update(float _DeltaTime)
{
	CameraInCheck();
	if (false == IsOnCamera) { return; }

	switch (StateValue)
	{
	case BambaState::Normal:
		MoveUpdate(_DeltaTime);
		break;
	case BambaState::Overturn:
		OverturnUpdate(_DeltaTime);
		break;
	case BambaState::Hold:
		MonsterCheck();
		break;
	case BambaState::Die:
		DieUpdate(_DeltaTime);
		break;
	case BambaState::Kick:
		KickUpdate(_DeltaTime);
		MonsterCheck();
		break;
	default:
		break;
	}
}

void Bamba::Turn()
{
	EnemyActor::Turn();
	if (0 < DirValue.X)
	{
		AnimationRender->ChangeAnimation("RIGHT_WALK");
	}
	else
	{
		AnimationRender->ChangeAnimation("LEFT_WALK");
	}
}

void Bamba::TurnLeft()
{
	EnemyActor::TurnLeft();
	AnimationRender->ChangeAnimation("LEFT_WALK");
}

void Bamba::TurnRight()
{
	EnemyActor::TurnRight();
	AnimationRender->ChangeAnimation("RIGHT_WALK");
}

void Bamba::OverturnUpdate(float _DeltaTime)
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

void Bamba::KickUpdate(float _DeltaTime)
{

	// ������ Overturn ���·� ��ȯ
	if (0 < MoveDir.Y && 0.1f > abs(MoveDir.X))
	{
		MoveDir.X = 0;
		StateValue = BambaState::Overturn;
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
					Pos.X = HitBlock->GetPos().X - BlockXSize - RenderScale.hX();
					Pos.X = std::round(Pos.X);
					SetPos(Pos);
				}
				else
				{
					float4 Pos = GetPos();
					Pos.X = HitBlock->GetPos().X + BlockXSize + RenderScale.hX();
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



void Bamba::MonsterCheck()
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
