#include "ItemActor.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "Mario.h"
#include "Map.h"
#include "Block.h"
#include "ContentsEnum.h"
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
	Collision->SetDebugRenderType(Rect);

}


void ItemActor::Update(float _DeltaTime)
{

	if (true == IsBlockOut)
	{
		Timer -= _DeltaTime;
		SetMove(float4::UP * 128 * _DeltaTime);
		if (0 > Timer)
		{
			IsBlockOut = false;
			ItemRender->SetOrder(static_cast<int>(RenderOrder::Item));
		}
		return;
	}

	// ȭ�� ������ �������� üũ
	float4 InCameraPos = GetPos() - GetLevel()->GetCameraPos();
	if (0 > InCameraPos.X + 100)
	{
		IsOnCamera = false;
		OffCamera();
	}
	else if (GameEngineWindow::GetScreenSize().X < InCameraPos.X - 100)
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

	// �÷��̾� üũ
	std::vector<GameEngineCollision*> Collisions;
	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check, Collisions))
	{
		Mario::MainPlayer->NewItem(ThisItemType);
		Death();
	}
}

void ItemActor::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}

void ItemActor::FallUpdate(float _DeltaTime)
{
	// �߷�
	FallDir.Y+= GravityAcceleration * _DeltaTime;
	if (GravityMax < FallDir.Y)
	{
		FallDir.Y = GravityMax;
	}
	// �浹 �̹��� �˻�
	if (nullptr == ColMap)
	{
		MsgAssert("�浹�� �� �̹����� �����ϴ�.");
	}
	// �̵��� ��ġ
	float4 NextPos = GetPos() + FallDir * _DeltaTime;

	// �ٴ� üũ
	DWORD PixelColor = ColMap->GetPixelColor(NextPos, White);
	if (Black == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (Black != PixelColor)
			{
				SetPos(NextPos);
				FallDir.Y = 0;
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
			NextPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				FallDir.Y = 0;
				break;
			}
		}
	}
	// ��Ż�� üũ
	else if (Red == PixelColor)
	{
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
		while (true)
		{
			NextPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(NextPos, Black);
			if (White == PixelColor)
			{
				SetPos(NextPos);
				FallDir.Y = 0;
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
				FallDir.Y = 0.0f;
				continue;
			}
		}
	}

	SetMove(FallDir * _DeltaTime);
}
