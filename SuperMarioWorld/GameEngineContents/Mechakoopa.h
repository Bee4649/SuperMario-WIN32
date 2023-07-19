#pragma once
#include "EnemyActor.h"

enum  class MechaState
{
	Spawn,
	Move,
	Stun,
	Hold,
	Kick,
	Die,
};

class Mechakoopa : public EnemyActor
{
public:
	Mechakoopa();
	~Mechakoopa();

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
	void SetPosDir(const float4& _Pos);

	Mechakoopa(const Mechakoopa& _Other) = delete;
	Mechakoopa(Mechakoopa&& _Other) noexcept = delete;
	Mechakoopa& operator=(const Mechakoopa& _Other) = delete;
	Mechakoopa& operator=(Mechakoopa&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Turn() override;
	void TurnLeft() override;
	void TurnRight() override;
private:
	GameEngineRender* AnimationRender = nullptr;
	const float4 RenderScale = { 120, 96 };
	const float4 RenderPos = { 0, -48 };
	const float4 CollisionScale = { 84, 84 };
	const float4 CollisionPos = { 0, -42 };
	const float4 MonsterHitForce = { 250, -750 };
	const float HeadingReaction = 100;
	const float BambaSpeed = 100;
	const float BambaSlopeSpeed = BambaSpeed * 0.5f;

	MechaState StateValue = MechaState::Move;

	void StunUpdate(float _DeltaTime);
	void KickUpdate(float _DeltaTime);

	void MonsterCheck();
};

