#include "QuestionBlock.h"
#include <GameEngineCore/GameEngineResources.h>
QuestionBlock::QuestionBlock() {

}

QuestionBlock::~QuestionBlock() {

}

void QuestionBlock::Hit()
{
	if (false == IsEmpty)
	{
		Block::Hit();
		IsEmpty = true;
	}
}

bool QuestionBlock::Damage()
{
	return false;
}

void QuestionBlock::Start()
{
	Block::Start();
	// ·»´õ »ý¼º
	{
		AnimationRender = CreateRender(RenderOrder::Block);
		AnimationRender->SetScale(RenderScale);
		AnimationRender->SetPosition(RenderPos);
		AnimationRender->CreateAnimation({ .AnimationName = "Defalut", .ImageName = "QuestionBlock.BMP", .Start = 0, .End = 3, .InterTime = 0.13f });
		AnimationRender->CreateAnimation({ .AnimationName = "EMPTY", .ImageName = "QuestionBlock.BMP", .Start = 4, .End = 4, });
		AnimationRender->ChangeAnimation("Defalut");
	}
}

void QuestionBlock::HitAnimEnd()
{
	AnimationRender->ChangeAnimation("EMPTY");
	if (nullptr != ItemActor)
	{
		ItemActor->BlockOut();
		GameEngineResources::GetInst().SoundPlay("OutItem.wav");
	}
}
