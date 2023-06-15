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
	// 화면 밖으로 나갔는지 체크
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
	// 중력
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
	ForwardPos.Y = GetPos().Y + float4::UP.Y * 4;

	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(ForwardPos, White);
	// 벽 체크
	if (Black == PixelColor)
	{
		Turn();
		NextPos = GetPos() + MoveDir * _DeltaTime;
	}
	// 바닥 체크
	PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		IsSlope = false;
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
		IsSlope = false;
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
		IsSlope = true;
		MoveDir.X = DirValue.X * SlopeSpeed;
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
			else if (GetPos().Y > ColActor->GetPos().Y)
			{
				ColActor->Hit();
				// 블록 밑으로 이동한다
				float4 Pos = GetPos();
				Pos.Y = ColActor->GetPos().Y + BlockOnPos;
				Pos.Y = std::round(Pos.Y);
				SetPos(Pos);
				MoveDir.Y = 100;
				continue;
			}
			// 그 외 경우
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
	// 중력
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
