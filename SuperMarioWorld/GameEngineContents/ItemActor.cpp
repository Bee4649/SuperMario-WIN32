#include "ItemActor.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Mario.h"
#include "Map.h"
#include "Block.h"
#include "ContentsEnums.h"
ItemActor::ItemActor() {

}

ItemActor::~ItemActor() {

}

void ItemActor::BlockOut()
{
	On();
	IsBlockOut = true;
	Timer = BlockOutTime;
	ItemRender->SetOrder(static_cast<int>(RenderOrder::Map));
}

void ItemActor::Start()
{
	Collision = CreateCollision(CollisionOrder::Item);
	Collision->SetScale(CollisionScale);
	Collision->SetPosition(CollisionPos);
	Collision->SetDebugRenderType(CT_Rect);

}


void ItemActor::Update(float _DeltaTime)
{

	if (true == IsBlockOut)
	{
		Timer -= _DeltaTime;
		SetMove(float4::Up * 128 * _DeltaTime);
		if (0 > Timer)
		{
			IsBlockOut = false;
			ItemRender->SetOrder(static_cast<int>(RenderOrder::Item));
		}
		return;
	}

	// 화면 밖으로 나갔는지 체크
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.x + 100)
	{
		IsOnCamera = false;
		OffCamera();
	}
	else if (GameEngineWindow::GetScreenSize().x < InCameraPos.x - 100)
	{
		IsOnCamera = false;
		OffCamera();
	}
	else
	{
		IsOnCamera = true;
		OnCamera();
	}

	if (false == IsOnCamera)
	{
		return;
	}
	
	// 플레이어 체크
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = CT_Rect, .ThisColType = CT_Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		Mario::MainPlayer->NewItem(ThisItemType);
		Death();
	}
}

void ItemActor::Render(float _DeltaTime)
{
	if (true == MarioGameCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void ItemActor::FallUpdate(float _DeltaTime)
{
	// 중력
	FallDir.y += GravityAcceleration * _DeltaTime;
	if (GravityMax < FallDir.y)
	{
		FallDir.y = GravityMax;
	}
	// 충돌 이미지 검사
	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}
	// 이동될 위치
	float4 NextPos = GetPos() + FallDir * _DeltaTime;

	// 바닥 체크
	DWORD PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				FallDir.y = 0;
				break;
			}
		}
	}
	// 아래에서 통과되는 블록들 체크 ex) 구름
	else if (Green == PixelColor)
	{
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				FallDir.y = 0;
				break;
			}
		}
	}
	// 비탈길 체크
	else if (Red == PixelColor)
	{
		NextPos.y = std::round(NextPos.y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			NextPos.y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				FallDir.y = 0;
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
				FallDir.y = 0.0f;
				continue;
			}
		}
	}

	SetMove(FallDir * _DeltaTime);
}
