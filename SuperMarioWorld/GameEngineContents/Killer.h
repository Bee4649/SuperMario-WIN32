#pragma once
#include "EnemyActor.h"
class Killer : public EnemyActor
{
public:
	Killer();
	~Killer();

	bool IsCollisionAttack() override;
	bool IsHold() override;
	bool IsHoldable() override;
	bool IsDie() override;
	void SpinHit() override;
	void JumpHit(bool IsLeft) override;
	void FireHit() override;
	void BlockHit() override;
	void MonsterHit(bool IsLeft) override;
	void Hold() override;
	void Kick(const float4& _Force) override;

	Killer(const Killer& _Other) = delete;
	Killer(Killer&& _Other) noexcept = delete;
	Killer& operator=(const Killer& _Other) = delete;
	Killer& operator=(Killer&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineRenderer* SpriteRender = nullptr;
	const float4 RenderPos = { 0, 0 };
	const float4 CollisionScale = { 208, 208 };
	const float4 CollisionPos = { 8, 0 };
	const float ActorSpeed = 250;
	bool StateDie = false;
};

