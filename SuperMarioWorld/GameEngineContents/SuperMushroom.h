#pragma once
#include "ItemActor.h"
class GameEngineImage;
class SuperMushroom : public ItemActor
{
public:
	SuperMushroom();
	~SuperMushroom();

	SuperMushroom(const SuperMushroom& _Other) = delete;
	SuperMushroom(SuperMushroom&& _Other) noexcept = delete;
	SuperMushroom& operator=(const SuperMushroom& _Other) = delete;
	SuperMushroom& operator=(SuperMushroom&& _Other) noexcept = delete;

	void BlockHit() override;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;

	void OffCamera() override;
	void OnCamera() override;

private:

	float Speed = 250;
	float SlopeSpeed = Speed * 0.6f;

	float4 DirValue = float4::RIGHT;
	const float GravityMax = 1750;
	const float GravityAcceleration = 4600;
	const float4 CollisionScale = { 16, 16 };

	bool IsOnCamera = true;
	bool IsSlope = false;
	GameEngineCollision* Collision = nullptr;

	float4 MoveDir = float4::ZERO;

	void Turn();
	void TurnLeft();
	void TurnRight();
};

