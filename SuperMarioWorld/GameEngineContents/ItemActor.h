#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include "ContentsEnum.h"

class ItemActor : public GameEngineActor
{
public:
	ItemActor();
	~ItemActor();

	ItemActor(const ItemActor& _Other) = delete;
	ItemActor(ItemActor&& _Other) noexcept = delete;
	ItemActor& operator=(const ItemActor& _Other) = delete;
	ItemActor& operator=(ItemActor&& _Other) noexcept = delete;

	void BlockOut();
	virtual void BlockHit() {}

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
	GameEngineRenderer* ItemRender = nullptr;
	GameEngineWindowTexture* ColMap = nullptr;

private:
	const float4 CollisionScale = { 48, 48 };
	const float4 CollisionPos = { 0, -24 };
	const float GravityMax = 750;
	const float GravityAcceleration = 1500;
	
	float4 FallDir = float4::ZERO;
	bool IsOnCamera = false;
	bool IsBlockOut = false;
	float Timer = 0;
	const float BlockOutTime = 0.5f;

	GameEngineCollision* Collision = nullptr;

};

