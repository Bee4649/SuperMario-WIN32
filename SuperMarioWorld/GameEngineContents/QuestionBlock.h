#pragma once
#include "Block.h"
#include <GameEngineCore/GameEngineLevel.h>
#include "ItemActor.h"
#include "SuperMushroom.h"
#include "FireFlower.h"

class QuestionBlock : public Block
{
public:
	QuestionBlock();
	~QuestionBlock();

	void Hit() override;
	bool Damage() override;

	void SetItem(ItemType _ItemType)
	{
		switch (_ItemType)
		{
		case ItemType::Coin:
			break;
		case ItemType::UpMushroom:
			break;
		case ItemType::SuperMushroom:
			ItemActor = GetLevel()->CreateActor<SuperMushroom>(RenderOrder::Item);
			break;
		case ItemType::FireFlower:
			ItemActor = GetLevel()->CreateActor<FireFlower>(RenderOrder::Item);
			break;
		case ItemType::Feather:
			break;
		default:
			break;
		}

		if (nullptr == ItemActor)
		{
			return;
		}

		ItemActor->SetPos(GetPos());
		ItemActor->Off();
	}

	QuestionBlock(const QuestionBlock& _Other) = delete;
	QuestionBlock(QuestionBlock&& _Other) noexcept = delete;
	QuestionBlock& operator=(const QuestionBlock& _Other) = delete;
	QuestionBlock& operator=(QuestionBlock&& _Other) noexcept = delete;

protected:
	void Start() override;
	void HitAnimEnd() override;
private:
	bool IsEmpty = false;
	ItemActor* ItemActor = nullptr;
};