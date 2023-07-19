#include "EnemyActor.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineResources.h>
#include "Map.h"
#include "Block.h"
#include "ContentsEnums.h"
#include "MarioGameCore.h"
EnemyActor::EnemyActor() {

}

EnemyActor::~EnemyActor() {

}

void EnemyActor::Start()
{
	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetScale(CollisionScale);
}

void EnemyActor::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	ColMap = GameEngineResources::GetInst().ImageFind(Map::MainMap->GetStageColName());
}

void EnemyActor::Render(float _DeltaTime)
{
	if (true == MarioGameCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void EnemyActor::CameraInCheck()
{
	// 화면 밖으로 나갔는지 체크
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.x + 256)
	{
		OffCamera();
	}
	else if (GameEngineWindow::GetScreenSize().x < InCameraPos.x - 256)
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
	MoveDir.y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.y)
	{
		MoveDir.y = GravityMax;
	}
	// 충돌 이미지 검사
	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}
	// 이동될 위치
	float4 NextPos = GetPos() + MoveDir * _DeltaTime;
	float4 ForwardPos = NextPos;
	ForwardPos.y = GetPos().y + float4::Up.y * 4;

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
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::Up;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				MoveDir.y = 0;
				break;
			}
		}
	}
	// 아래에서 통과되는 블록들 체크 ex) 구름
	else if (Green == PixelColor)
	{
		IsSlope = false;
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::Up;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.y = 0;
				break;
			}
		}
	}
	// 비탈길 체크
	else if (Red == PixelColor)
	{
		IsSlope = true;
		MoveDir.x = DirValue.x * SlopeSpeed;
		float4 SlopePos = NextPos;
		SlopePos += SlopeRightUp;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos += float4::Up;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				MoveDir.y = 0;
				break;
			}
		}

	}

	// 블록 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Block), .TargetColType = CT_Rect, .ThisColType = CT_Rect };
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
			if (GetPos().y < ColActor->GetPos().y - BlockYSize)
			{
				float4 Pos = GetPos();
				Pos.y = ColActor->GetPos().y - BlockOnPos;
				Pos.y = std::round(Pos.y);
				SetPos(Pos);
				MoveDir.y = 0.0f;
				continue;
			}
			else if (GetPos().y > ColActor->GetPos().y)
			{
				ColActor->Hit();
				// 블록 밑으로 이동한다
				float4 Pos = GetPos();
				Pos.y = ColActor->GetPos().y + BlockOnPos;
				Pos.y = std::round(Pos.y);
				SetPos(Pos);
				MoveDir.y = 100;
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
	MoveDir.y += GravityAcceleration * _DeltaTime;
	if (GravityMax < MoveDir.y)
	{
		MoveDir.y = GravityMax;
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
	DirValue = float4::Left;
	MoveDir = DirValue * Speed;
}

inline void EnemyActor::TurnRight()
{
	DirValue = float4::Right;
	MoveDir = DirValue * Speed;
}
