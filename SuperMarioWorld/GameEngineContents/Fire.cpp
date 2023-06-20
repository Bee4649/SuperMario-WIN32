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
	// ���� ����
	{
		AnimationRender = CreateRender(RenderOrder::Player);
		AnimationRender->SetScale({ 64, 64 });
		AnimationRender->CreateAnimation({ .AnimationName = "Defalut", .ImageName = "FIRE.BMP", .Start = 0, .End = 3, .InterTime = 0.07f });
	}
	AnimationRender->ChangeAnimation("Defalut");

	// Collision ����
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


	// ȭ�� ������ �������� üũ
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.X + 10)
	{
		// ȭ�� ������ ������ ����
		Death();
		return;
	}
	if (GameEngineWindow::GetScreenSize().X < InCameraPos.X - 10)
	{
		// ȭ�� ������ ������ ����
		Death();
		return;
	}

	// ���� ������ �޾Ҵ��� üũ
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
	// ���� ��ġ
	//float4 DownPos = GetPos() + float4::Down * 10;
	// MoveDir �߷� ����
	if (true == IsSlope)
	{
		MoveDir += float4::DOWN * GravityAcceleration * 2 * _DeltaTime;
	}
	else
	{
		MoveDir += float4::DOWN * GravityAcceleration * _DeltaTime;
	}
	// MoveDir �ӵ� ����

	// ���� �߷��� �ִ� �߷��� �ʰ��� ���
	if (GravityMax < MoveDir.Y)
	{
		// �ִ� �߷� ����
		MoveDir.Y = GravityMax;
	}

	// �̹� �����ӿ� �̵��� ��ġ
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;

	// ______�� �浹 üũ______

	if (nullptr == ColMap)
	{
		MsgAssert("�浹�� �� �̹����� �����ϴ�.");
	}

	// �� �浹 üũ�� �÷� ����
	DWORD PixelColor = ColMap->GetPixelColor(NextPos, White);
	float4 ForwardPos = NextPos;
	ForwardPos.Y = GetPos().Y - 4;
	// �� üũ
	if (Black == ColMap->GetPixelColor(ForwardPos, White))
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
		Death();
		return;
	}

	// �ٴ� üũ
	if (Black == PixelColor)
	{
		IsSlope = false;
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
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
	// �Ʒ����� ����Ǵ� ���ϵ� üũ ex) ����
	if (Green == PixelColor)
	{
		IsSlope = false;
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
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
	// ��Ż�� üũ
	else if (Red == PixelColor)
	{
		bool Check = false;
		float4 SlopePos = NextPos;
		SlopePos.X += 5;
		SlopePos.Y -= 5;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		// ���ΰ� ������
		if (Red == PixelColor)
		{
			if (0 < MoveDir.X)
			{
				Check = true;
			}
		}
		// ���ΰ� ����
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
			// �ٴڿ��� ���� ���� �ö󰣴�
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
	// ���� üũ
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
			// �� ��ġ�� ���Ϻ��� ���� �ִ� ���
			if (GetPos().Y < ColActor->GetPos().Y - 60)
			{
				MoveDir.Y = -JumpForce;
				break;
			}
			// �� ��ġ�� ���� �ؿ� �ִ� ���
			else if (GetPos().Y > ColActor->GetPos().Y + 60)
			{
				Particle::CreateParticle(GetLevel(), GetPos(), "SMOKE1");
				Death();
				return;
			}
			// �� �� ��� (���� �΋H��)
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