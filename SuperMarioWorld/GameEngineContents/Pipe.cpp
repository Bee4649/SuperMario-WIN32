#include "Pipe.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineResources.h>
#include "ContentsEnum.h"
#include "LevelLoader.h"
#include "Mario.h"

Pipe::Pipe() 
{
}

Pipe::~Pipe() 
{
}

void Pipe::Start()
{
	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetDebugRenderType(Rect);
}

void Pipe::Update(float _DeltaTime)
{
	if (true == IsEnter)
	{
		Timer -= _DeltaTime;

		if (0 >= Timer)
		{
			LevelLoader::ChangeLevel(Level);
		}
		return;
	}

	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = Rect, .ThisColType = Rect };
	if (true == Collision->Collision(Check) && GameEngineInput::IsPress(Key))
	{
		float4 MoveDir;
		switch (Dir)
		{
		case Up:
			MoveDir = float4::UP;
			break;
		case Down:
			MoveDir = float4::DOWN;
			break;
		case Left:
			MoveDir = float4::LEFT;
			break;
		case Right:
			MoveDir = float4::RIGHT;
			break;
		default:
			break;
		}
		Mario::MainPlayer->PipeIn(GetPos(), MoveDir);
		IsEnter = true;
		Timer = EnterTime;
	}
}

void Pipe::Render(float _DeltaTime)
{
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}
