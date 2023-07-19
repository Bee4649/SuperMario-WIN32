#pragma once
#include <GameEngineCore/GameEngineActor.h>
class CheckPointActor : public GameEngineActor
{
public:
	CheckPointActor();
	~CheckPointActor();

	void SetCheckPoint(int _CheckPoint, const float4& _Pos);

	CheckPointActor(const CheckPointActor& _Other) = delete;
	CheckPointActor(CheckPointActor&& _Other) noexcept = delete;
	CheckPointActor& operator=(const CheckPointActor& _Other) = delete;
	CheckPointActor& operator=(CheckPointActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
private:
	const float4 CollisionScale = { 64, 8 };
	const float4 CollisionPos = { 0, -178 };
	const float4 RenderPos = { 0, -128 };
	GameEngineCollision* Collision = nullptr;
	GameEngineRender* CheckPointRender = nullptr;
	int CheckPoint = 0;
	bool IsPass = false;
};

