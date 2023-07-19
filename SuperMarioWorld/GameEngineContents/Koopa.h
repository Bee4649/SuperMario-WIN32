#pragma once
#include "EnemyActor.h"

enum class KoopaState
{
	Appear,
	Move,
	Spawn,
	Attack,
	Hit,
	Die,
};

class Koopa : public EnemyActor
{
public:
	Koopa();
	~Koopa();

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

	Koopa(const Koopa& _Other) = delete;
	Koopa(Koopa&& _Other) noexcept = delete;
	Koopa& operator=(const Koopa& _Other) = delete;
	Koopa& operator=(Koopa&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	GameEngineRender* KoopaRender = nullptr;
	GameEngineRender* PierrotRender = nullptr;
	GameEngineRender* PropellerRender = nullptr;
	const float4 KoopaRenderScale = { 256, 160 };
	const float4 KoopaRenderPos = { 0, 0 };
	const float4 PierrotRenderScale = { 256, 224 };
	const float4 PierrotRenderPos = { 0, 160 };
	const float4 PropellerRenderScale = { 160, 32 };
	const float4 PropellerRenderPos = { 0, 284 };
	const float4 KoopaCollisionScale = { 200, 100 };
	const float4 KoopaCollisionPos = { 0, 0 };
	const float4 PierrotCollisionScale = { 225, 225 };
	const float4 PierrotCollisionPos = { 0, 150 };
	const float4 SpawnPos = { 0, 128 };
	const float HitAnimTime = 1.0f;
	const float SpawnAnimTime = 1.0f;
	const float SpawnTime = 3.0f;

	KoopaState StateValue = KoopaState::Appear;
	std::string AnimStr = "";
	int HP = 3;
	float MoveTimer = 0;
	float AnimTimer = 0;
	float SpawnTimer = 0;
	bool IsSpawn = false;
	void AppearUpdate(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);
	void HitUpdate(float _DeltaTime);
	void DieUpdate(float _DeltaTime);
	void SpawnUpdate(float _DeltaTime);
	void ChangeAnimation(const std::string_view& _Name);
};

