#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Bullet : public GameEngineActor
{
public:
	// constrcuter destructer
	Bullet();
	~Bullet();

	// delete Function
	Bullet(const Bullet& _Other) = delete;
	Bullet(Bullet&& _Other) noexcept = delete;
	Bullet& operator=(const Bullet& _Other) = delete;
	Bullet& operator=(Bullet&& _Other) noexcept = delete;

	class GameEngineRenderer* Renderer;

	void SetDir(const float4& _Dir) 
	{
		Dir = _Dir;
	}

	void SetSpeed(const float _Speed)
	{
		Speed = _Speed;
	}

protected:

private:
	float4 Dir;
	float Speed = 500.0f;

	void Start() override;
	void Update(float _Delta) override;
};

