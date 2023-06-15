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
	ForwardPos.Y = GetPos().Y;

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
	// �Ʒ����� ����Ǵ� ��ϵ� üũ ex) ����
	else if (Green == PixelColor)
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
		SlopePos.X += 5;
		SlopePos.Y -= 5;
		PixelColor = ColMap->GetPixelColor(SlopePos, Black);
		NextPos.Y = std::round(NextPos.Y);
		// �ٴڿ��� ���� ���� �ö󰣴�
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
			else if (GetPos().Y > ColActor->GetPos().Y + BlockYSize)
			{
				continue;
			}
			// �� �� ���
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
