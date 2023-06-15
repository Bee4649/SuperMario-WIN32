#include "Fire.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineResources.h>
#include "ContentsEnum.h"
#include "Particle.h"
#include "Map.h"
#include "Block.h"
#include "EnemyActor.h"
#include "ContentCore.h"

int Fire::Num = 0;

Fire::Fire() 
{
	Fire::Num++;
	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
}


Fire::~Fire() 
{
	Fire::Num--;
}


void Fire::Start()
{
	// 렌더 생성
	{
		AnimationRender = CreateRender(RenderOrder::Player);
		AnimationRender->SetScale({ 64, 64 });
		AnimationRender->CreateAnimation({ .AnimationName = "Defalut", .ImageName = "FIRE.BMP", .Start = 0, .End = 3, .InterTime = 0.07f });
	}
	AnimationRender->ChangeAnimation("Defalut");

	// Collision 생성
	{
		Collision = CreateCollision(CollisionOrder::PlayerAttack);
		Collision->SetScale({ 32, 32 });
		Collision->SetPosition({ 0, 0 });
		Collision->SetDebugRenderType(CollisionType::Rect);
	}
}

void Fire::Update(float _DeltaTime)
{
	//SetMove(Dir * Speed * _DeltaTime);


	// 화면 밖으로 나갔는지 체크
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.X + 10)
	{
		// 화면 밖으로 나가면 제거
		Death();
		return;
	}
	if (GameEngineWindow::GetScreenSize().X < InCameraPos.X - 10)
	{
		// 화면 밖으로 나가면 제거
		Death();
		return;
	}

	// 적이 공격을 받았는지 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Monster), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		EnemyActor* ColActor = Collisions[0]->GetOwner<EnemyActor>();
		if (false == ColActor->IsHold())
		{
			Collisions[0]->GetOwner<EnemyActor>()->FireHit();
			Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
			GameEngineResources::GetInst().SoundPlay("kick.wav");
			Death();
			return;
		}
	}

	MoveCalculation(_DeltaTime);
}

void Fire::MoveCalculation(float _DeltaTime)
{
	// 밑의 위치
	//float4 DownPos = GetPos() + float4::Down * 10;
	// MoveDir 중력 증가
	if (true == IsSlope)
	{
		MoveDir += float4::DOWN * GravityAcceleration * 2 * _DeltaTime;
	}
	else
	{
		MoveDir += float4::DOWN * GravityAcceleration * _DeltaTime;
	}
	// MoveDir 속도 증가

	// 현제 중력이 최대 중력을 초과한 경우
	if (GravityMax < MoveDir.Y)
	{
		// 최대 중력 제한
		MoveDir.Y = GravityMax;
	}

	// 이번 프레임에 이동할 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;

	// ______맵 충돌 체크______

	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(NextPos, White);
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y - 4;
	// 벽 체크
	if (Black == ColMap->GetPixelColor(ForwardPos, White))
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
		Death();
		return;
	}

	// 바닥 체크
	if (Black == PixelColor)
	{
		IsSlope = false;
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = -JumpForce;
				break;
			}
		}
	}
	// 아래에서 통과되는 블록들 체크 ex) 구름
	if (Green == PixelColor)
	{
		IsSlope = false;
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.Y = -JumpForce;
				break;
			}
		}
	}
	// 비탈길 체크
	else if (Red == PixelColor)
	{
		bool Check = false;
		float4 SlopePos = NextPos;
		SlopePos.X += 5;
		SlopePos.Y -= 5;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		// 경사로가 오른쪽
		if (Red == PixelColor)
		{
			if (0 < MoveDir.X)
			{
				Check = true;
			}
		}
		// 경사로가 왼쪽
		else
		{
			if (0 > MoveDir.X)
			{
				Check = true;
			}
		}

		if (true == Check)
		{
			IsSlope = true;
			NextPos.Y = std::round(NextPos.Y);
			// 바닥에서 제일 위로 올라간다
			while (true)
			{
				NextPos.Y -= 1;
				PixelColor = ColMap->GetPixelColor(NextPos, Black);
				if (White == PixelColor)
				{
					SetPos(NextPos);
					MoveDir.Y = -SlopeJumpForce;
					break;
				}
			}
			NextPos.X += 1;
		}
	}
	// 블록 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		bool IsHeading = false;
		std::vector<GameEngineCollision*>::iterator Start = Collisions.begin();
		std::vector<GameEngineCollision*>::iterator End = Collisions.end();
		for (; Start != End; Start++)
		{
			Block* ColActor = (*Start)->GetOwner<Block>();
			if (true == ColActor->GetIsRoll())
			{
				continue;
			}
			// 내 위치가 블록보다 위에 있는 경우
			if (GetPos().Y < ColActor->GetPos().Y - 60)
			{
				MoveDir.Y = -JumpForce;
				break;
			}
			// 내 위치가 블록 밑에 있는 경우
			else if (GetPos().Y > ColActor->GetPos().Y + 60)
			{
				Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
				Death();
				return;
			}
			// 그 외 경우 (벽에 부딫힘)
			else
			{
				Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
				Death();
				return;
			}
		}
	}

	SetMove(MoveDir * _DeltaTime);
}

void Fire::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}