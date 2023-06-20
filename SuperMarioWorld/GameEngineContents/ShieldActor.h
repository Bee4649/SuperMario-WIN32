#pragma once
#include "EnemyActor.h"
class ShieldActor : public EnemyActor
{
public:
	ShieldActor();
	~ShieldActor();

	void SetOwner(GameEngineActor* _Owner);
	void SetCollision(const float4& _Scale, const float4 _FixedPos);
	bool IsCollisionAttack() override;
	bool IsHold() override;
	bool IsHoldable() override;
	bool IsDie() override;
	void SpinHit() override {};
	void JumpHit(bool IsLeft) override {};
	void FireHit() override {};
	void BlockHit() override {};
	void MonsterHit(bool IsLeft) override {};
	void Hold() override {};
	void Kick(const float4& _Force) override {};

	ShieldActor(const ShieldActor& _Other) = delete;
	ShieldActor(ShieldActor&& _Other) noexcept = delete;
	ShieldActor& operator=(const ShieldActor& _Other) = delete;
	ShieldActor& operator=(ShieldActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineActor* Owner = nullptr;
};

