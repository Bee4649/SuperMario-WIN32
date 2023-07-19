#include "Pipe.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineResources.h>
#include "ContentsEnums.h"
#include "LevelLoader.h"
#include "Mario.h"
Pipe::Pipe() {

}

Pipe::~Pipe() {

}

void Pipe::Start()
{
	Collision = CreateCollision(CollisionOrder::Check);
	Collision->SetDebugRenderType(CT_Rect);
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

	CollisionCheckParameter Check = { .TargetGroup = static_cast<int>(CollisionOrder::Player), .TargetColType = CT_Rect, .ThisColType = CT_Rect };
	if (true == Collision->Collision(Check) && GameEngineInput::IsPress(Key))
	{
		float4 MoveDir;
		switch (Dir)
		{
		case Up:
			MoveDir = float4::Up;
			break;
		case Down:
			MoveDir = float4::Down;
			break;
		case Left:
			MoveDir = float4::Left;
			break;
		case Right:
			MoveDir = float4::Right;
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
	if (true == MarioGameCore::GetInst().GetCollisionDebug())
	{
		Collision->DebugRender();
	}
}
