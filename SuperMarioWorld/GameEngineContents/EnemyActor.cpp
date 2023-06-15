#include "EnemyActor.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineResources.h>
#include "Map.h"
#include "Block.h"
#include "ContentsEnum.h"
#include "ContentCore.h"

EnemyActor::EnemyActor() 
{
}

EnemyActor::~EnemyActor() 
{
}

void EnemyActor::Start()
{
	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetScale(CollisionScale);
}

void EnemyActor::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
}

void EnemyActor::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void EnemyActor::CameraInCheck()
{
	// ȭ�� ������ �������� üũ
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.X + 256)
	{
		OffCamera();
	}
	else if (GameEngineWindow::GetScreenSize().X < InCameraPos.X - 256)
	{
		OffCamera();
	}
	else
	{
		OnCamera();
	}
}

void EnemyActor::MoveUpdate(float _DeltaTime)
{
	// �߷�
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
	ForwardPos.Y = GetPos().Y + float4::UP.Y * 4;

	// �� �浹 üũ�� �÷� ����
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);
	// �� üũ
	if (Black == PixelColor)
	{
		Turn();
		NextPos = GetPos() + MoveDir * _DeltaTime;
	}
	// �ٴ� üũ
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		IsSlope = false;
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
		IsSlope = false;
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
		IsSlope = true;
		MoveDir.X = DirValue.X * SlopeSpeed;
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
			else if (GetPos().Y > ColActor->GetPos().Y)
			{
				ColActor->Hit();
				// ��� ������ �̵��Ѵ�
				float4 Pos = GetPos();
				Pos.Y = ColActor->GetPos().Y + BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 100;
				continue;
			}
			// �� �� ���
			else
			{
				Turn();
			}
		}
	}

	SetMove(MoveDir * _DeltaTime);

}

void EnemyActor::DieUpdate(float _DeltaTime)
{
	// �߷�
	MoveDir.Y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.Y)
	{
		MoveDir.Y = GravityMax;
	}
	SetMove(MoveDir * _DeltaTime);

}

void EnemyActor::OffCamera()
{
	IsOnCamera = false;
}

void EnemyActor::OnCamera()
{
	IsOnCamera = true;
}

inline void EnemyActor::DirSetting(const float4& _DirValue)
{
	DirValue = _DirValue;
	MoveDir = DirValue * Speed;
}

inline void EnemyActor::Turn()
{
	DirValue = -DirValue;
	MoveDir = DirValue * Speed;
}

inline void EnemyActor::TurnLeft()
{
	DirValue = float4::LEFT;
	MoveDir = DirValue * Speed;
}

inline void EnemyActor::TurnRight()
{
	DirValue = float4::RIGHT;
	MoveDir = DirValue * Speed;
}
