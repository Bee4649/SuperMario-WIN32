#pragma once
#include "EnemyActor.h"

enum class NokoState
{
	Normal,
	Overturn,
	Naked,
	Die
};

class Nokonoko : public EnemyActor
{
public:
	Nokonoko();
	~Nokonoko();


	bool IsCollisionAttack() override; // 현재 액터가 공격판정이 있으면 true를 리턴하는 함수
	bool IsHold() override; // 현재 액터가 플레이어에게 잡힌 상태면 true를 리턴하는 함수
	bool IsHoldable() override; // 현재 액터를 잡을 수 있는 상태면 true를 리턴하는 함수
	bool IsDie() override;
	void SpinHit() override; // 스핀점프로 밟히면 실행되는 함수
	void JumpHit(bool IsLeft) override; // 점프로 밟히면 실행되는 함수
	void FireHit() override; // 불꽃에 맞으면 실행되는 함수
	void BlockHit() override; // 블록위 서있을때 해당 블록이 히트하면 실행되는 함수
	void MonsterHit(bool IsLeft) override; // 다른 몬스터를 던져서 맞았을때 실행되는 함수, 두 몬스터 중 왼쪽에 있으면 true를 받는다
	void Hold() override; // 플레이어에게 잡히면 실행되는 함수
	void Kick(const float4& _Force) override; // 플레이어에게 발로 치이면 실행되는 함수 (잡힌 후 차는 경우)

	Nokonoko(const Nokonoko& _Other) = delete;
	Nokonoko(Nokonoko&& _Other) noexcept = delete;
	Nokonoko& operator=(const Nokonoko& _Other) = delete;
	Nokonoko& operator=(Nokonoko&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Turn() override;
	void TurnLeft() override;
	void TurnRight() override;

private:
	GameEngineRenderer* AnimationRender = nullptr;
	NokoState StateValue = NokoState::Normal;
	const float4 RenderScale = { 64, 108 };
	const float4 RenderPos = { 0, -50 };
	const float4 CollisionScale = { 48, 84 };
	const float4 CollisionPos = { 0, -42 };
	const float4 MonsterHitForce = { 250, -750 };
	const float HeadingReaction = 100;
	const float ActorSpeed = 80;
	const float ActorSlopeSpeed = ActorSpeed * 0.6f;
	const int Score = 100;
	void OverturnUpdate(float _DeltaTime);
};