#include "SuperMushroom.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
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
	ItemRender->SetPosition({ 0, -30});

	ThisItemType = ItemType::SuperMushroom;

	ColMap = GameEngineResources::GetInst().ImageFind(Map::MainMap->GetStageColName());
}

void SuperMushroom::Update(float _DeltaTime)
{
	ItemActor::Update(_DeltaTime);

	if (false == GetIsOnCamera())
	{
		return;
	}

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
	ForwardPos.y = GetPos().y;

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
			NextPos.y -= 1;
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
			NextPos.y -= 1;
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
		SlopePos.x += 5;
		SlopePos.y -= 5;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.y -= 1;
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
			else if (GetPos().y > ColActor->GetPos().y + BlockYSize)
			{
				continue;
			}
			// 그 외 경우
			else
			{
				if (GetPos().x < ColActor->GetPos().x)
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
	ColMap = GameEngineResources::GetInst().ImageFind(Map::MainMap->GetStageColName());
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
	DirValue = float4::Left;
	MoveDir = DirValue * Speed;
}

void SuperMushroom::TurnRight()
{
	DirValue = float4::Right;
	MoveDir = DirValue * Speed;
}
