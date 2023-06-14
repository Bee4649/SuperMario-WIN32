#include "SuperMushroom.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Mario.h"
#include "Map.h"
#include "Block.h"

SuperMushroom::SuperMushroom() {
}

SuperMushroom::~SuperMushroom() {

}

void SuperMushroom::BlockHit()
{
}

void SuperMushroom::Start()
{
	ItemActor::Start();

	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetScale(CollisionScale);
	MoveDir = DirValue * Speed;

	ItemRender = CreateRender(RenderOrder::Item);
	ItemRender->SetImage("SUPERMUSHROOM.BMP");
	ItemRender->SetScaleToImage();
	ItemRender->SetPosition({ 0, -30 });

	ThisItemType = ItemType::SuperMushroom;

	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
}

void SuperMushroom::Update(float _DeltaTime)
{
	ItemActor::Update(_DeltaTime);

	if (false == GetIsOnCamera())
	{
		return;
	}

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
	ForwardPos.Y = GetPos().Y;

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
			NextPos.Y -= 1;
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
			NextPos.Y -= 1;
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
		SlopePos.X += 5;
		SlopePos.Y -= 5;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.Y -= 1;
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
				continue;
			}
			// 그 외 경우
			else
			{
				if (GetPos().X < ColActor->GetPos().X)
				{
					TurnLeft();
				}
				else {
					TurnRight();
				}
			}
		}
	}

	SetMove(MoveDir * _DeltaTime);
}

void SuperMushroom::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	ColMap = GameEngineResources::GetInst().TextureFind(Map::MainMap->GetStageColName());
}

void SuperMushroom::OffCamera()
{
}

void SuperMushroom::OnCamera()
{
}

void SuperMushroom::Turn()
{
	DirValue = -DirValue;
	MoveDir = DirValue * Speed;
}

void SuperMushroom::TurnLeft()
{
	DirValue = float4::LEFT;
	MoveDir = DirValue * Speed;
}

void SuperMushroom::TurnRight()
{
	DirValue = float4::RIGHT;
	MoveDir = DirValue * Speed;
}
