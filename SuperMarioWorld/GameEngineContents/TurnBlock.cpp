#include "TurnBlock.h"

TurnBlock::TurnBlock() {

}

TurnBlock::~TurnBlock() {

}

void TurnBlock::Hit()
{
	Block::Hit();

	IsRoll = true;
	RollTimer = RollTime;
}

bool TurnBlock::GetIsRoll()
{
	return IsRoll;
}

void TurnBlock::Start()
{
	Block::Start();
	// ·»´õ »ý¼º
	{
		AnimationRender = CreateRender(RenderOrder::Block);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->SetPosition(RenderPos);
		AnimationRender->CreateAnimation({ .AnimationName = "Defalut", .ImageName = "BLOCK.BMP", .Start = 3, .End = 3, });
		AnimationRender->CreateAnimation({ .AnimationName = "ROLL", .ImageName = "BLOCK.BMP", .Start = 0, .End = 3, .InterTime = 0.13f });
		AnimationRender->ChangeAnimation("Defalut");
	}
}

void TurnBlock::Update(float _DeltaTime)
{
	Block::Update(_DeltaTime);
	if (true == IsRoll)
	{
		RollTimer -= _DeltaTime;
		if (0 > RollTimer)
		{
			AnimationRender->ChangeAnimation("Defalut");
			IsRoll = false;
			return;
		}
	}
}

void TurnBlock::HitAnimEnd()
{
	AnimationRender->ChangeAnimation("ROLL");
	IsRoll = true;
	RollTimer = RollTime;
}
