#pragma once
#include "Block.h"
class TurnBlock : public Block
{
public:
	TurnBlock();
	~TurnBlock();

	void Hit() override;
	bool GetIsRoll() override;

	TurnBlock(const TurnBlock& _Other) = delete;
	TurnBlock(TurnBlock&& _Other) noexcept = delete;
	TurnBlock& operator=(const TurnBlock& _Other) = delete;
	TurnBlock& operator=(TurnBlock&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void HitAnimEnd() override;

private:
	bool IsRoll = false;
	float RollTimer = 0;
	const float RollTime = 5.2f;
};

