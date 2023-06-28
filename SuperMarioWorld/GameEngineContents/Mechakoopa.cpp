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
	// 애니메이션 변경
	if (0 < DirValue.X)
	{
		AnimationRender->ChangeAnimation("Right_Damage");
	}
	else
	{
		AnimationRender->ChangeAnimation("Left_Damage");
	}

	// 상태 변경
	StateValue = MechaState::Die;
	MoveDir = MonsterHitForce;

	// 이동 지정
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
	// Render 생성
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
	// Collision 생성
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
	// 중력
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}
	// 마찰
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
	// 충돌 이미지 검사
	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}
	// 이동될 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y + float4::UP.Y;

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);
	// 벽 체크
	if (Black == PixelColor)
	{
		MoveDir.X = 0;
	}
	// 바닥 체크
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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
	// 아래에서 통과되는 블록들 체크 ex) 구름
	else if (Green == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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
	// 비탈길 체크
	else if (Red == PixelColor)
	{
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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

	// 블록 체크
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
			// 엑터가 블록보다 위에 있는 경우
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
			// 그 외 경우
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

	// 정지시 Stun 상태로 전환
	if (0.1f > abs(MoveDir.Y) && 0.1f > abs(MoveDir.X))
	{
		MoveDir.X = 0;
		StateValue = MechaState::Stun;
	}

	// 이동 감속
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

	// 중력 적용
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}

	// 충돌 이미지 검사
	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}
	// 이동될 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y + float4::UP.Y;

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);

	// 벽 체크
	if (Black == PixelColor)
	{
		MoveDir.X = 0;
		return;
	}
	// 바닥 체크
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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
	// 아래에서 통과되는 블록들 체크 ex) 구름
	else if (Green == PixelColor && 0 < MoveDir.Y)
	{
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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
	// 비탈길 체크
	else if (Red == PixelColor && 0 < MoveDir.Y)
	{
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
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

	// 블록 체크
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
			// 엑터가 블록보다 위에 있는 경우
			if (GetPos().Y < HitBlock->GetPos().Y - BlockYSize)
			{
				// 블록위에 선다
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y - BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 0.0f;
				continue;
			}
			// 밑에서 위로 친 경우
			else if (GetPos().Y > HitBlock->GetPos().Y)
			{
				// 블록이 히트한다
				Hit = true;
				HitBlock->Hit();
				// 블록 밑으로 이동한다
				float4 Pos = GetPos();
				Pos.Y = HitBlock->GetPos().Y + BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = HeadingReaction;
			}
			// 그 외 경우 (옆에서 친 경우)
			else
			{
				// 방향에 따라 위치 조정한다

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
				// 블록이 히트한다
				Hit = true;
				HitBlock->Hit();
			}

		}
		// 블록이 히트한 경우 효과음 및 좌우 이동 정지
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
	// 몬스터 체크
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
