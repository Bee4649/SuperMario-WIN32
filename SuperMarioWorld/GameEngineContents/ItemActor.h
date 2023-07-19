#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentsEnums.h"

class ItemActor : public GameEngineActor
{
public:
	ItemActor();
	~ItemActor();

	void BlockOut();
	virtual void BlockHit() {}

	ItemActor(const ItemActor& _Other) = delete;
	ItemActor(ItemActor&& _Other) noexcept = delete;
	ItemActor& operator=(const ItemActor& _Other) = delete;
	ItemActor& operator=(ItemActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void FallUpdate(float _DeltaTime);

	virtual void OffCamera() = 0;
	virtual void OnCamera() = 0;

	inline bool GetIsOnCamera() {
		return IsOnCamera;
	}

	ItemType ThisItemType = ItemType::Coin;
	GameEngineRender* ItemRender = nullptr;
	GameEngineImage* ColMap = nullptr;
private:
	const float4 CollisionScale = { 48, 48 };
	const float4 CollisionPos = { 0, -24 };
	const float GravityMax = 750;
	const float GravityAcceleration = 1500;
	GameEngineCollision* Collision = nullptr;
	float4 FallDir = float4::Zero;
	bool IsOnCamera = false;

	bool IsBlockOut = false;
	float Timer = 0;
	const float BlockOutTime = 0.5f;

};

