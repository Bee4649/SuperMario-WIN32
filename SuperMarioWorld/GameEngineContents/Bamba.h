#pragma once
#include "EnemyActor.h"

enum class BambaState
{
	Normal,
	Overturn,
	Hold,
	Kick,
	Die
};

class GameEngineImage;
class Bamba : public EnemyActor
{
public:
	Bamba();
	~Bamba();
	
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

	Bamba(const Bamba& _Other) = delete;
	Bamba(Bamba&& _Other) noexcept = delete;
	Bamba& operator=(const Bamba& _Other) = delete;
	Bamba& operator=(Bamba&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Turn() override;
	void TurnLeft() override;
	void TurnRight() override;

private:
	GameEngineRender* AnimationRender = nullptr;
	const float4 RenderScale = {64, 64};
	const float4 RenderPos = {0, -28};
	const float4 CollisionScale = {48, 48};
	const float4 CollisionPos = {0, -24};
	const float4 MonsterHitForce = { 250, -750};
	const float HeadingReaction = 100;
	const float BambaSpeed = 100;
	const float BambaSlopeSpeed = BambaSpeed * 0.5f;

	BambaState StateValue = BambaState::Normal;
	//float4 MoveDir = { 0,0 };
	const float BrakeForce = 1000;
	const int Score = 100;
	void OverturnUpdate(float _DeltaTime);
	void KickUpdate(float _DeltaTime);
	
	void MonsterCheck();
};

