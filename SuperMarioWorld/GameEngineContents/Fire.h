#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>

class GameEngineImage;
class Fire : public GameEngineActor
{
public:
	Fire();
	~Fire();

	void SetDir(const float4& _Dir)
	{
		MoveDir = _Dir * Speed;
	}
	static bool IsMax()
	{
		return Max <= Num;
	}
	Fire(const Fire& _Other) = delete;
	Fire(Fire&& _Other) noexcept = delete;
	Fire& operator=(const Fire& _Other) = delete;
	Fire& operator=(Fire&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void MoveCalculation(float _DeltaTime);
private:

	GameEngineRender* AnimationRender = nullptr;
	GameEngineCollision* Collision = nullptr;
	GameEngineImage* ColMap = nullptr;

	static int Num;
	static const int Max = 2;
	bool IsSlope = false;
	const float Speed = 850.0f;
	const float GravityMax = 2000;
	const float GravityAcceleration = 4000;
	const float JumpForce = 750;
	const float SlopeJumpForce = 1500;
	float4 MoveDir = float4::Zero;
};
