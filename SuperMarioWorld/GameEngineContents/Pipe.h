#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>

enum Direction
{
	Up,
	Down,
	Left,
	Right
};
class Pipe : public GameEngineActor
{
public:

	Pipe();
	~Pipe();

	void SetPipe(const std::string_view& _Level, Direction _Dir)
	{
		Level = _Level;
		SetDir(_Dir);
	}
	void SetDir(Direction _Dir)
	{
		Dir = _Dir;
		switch (Dir)
		{
		case Up:
			Key = "Up";
			Collision->SetScale(VerticalCollisionSize);
			Collision->SetPosition(VerticalCollisionPosition);
			break;
		case Down:
			Key = "Down";
			Collision->SetScale(VerticalCollisionSize);
			Collision->SetPosition(VerticalCollisionPosition);
			break;
		case Left:
			Key = "Left";
			Collision->SetScale(HorizontalCollisionSize);
			Collision->SetPosition(HorizontalCollisionPosition);
			Collision->SetMove(float4::LEFT * 24);
			break;
		case Right:
			Key = "Right";
			Collision->SetScale(HorizontalCollisionSize);
			Collision->SetPosition(HorizontalCollisionPosition);
			Collision->SetMove(float4::RIGHT * 24);
			break;
		default:
			break;
		}
	}

	Pipe(const Pipe& _Other) = delete;
	Pipe(Pipe&& _Other) noexcept = delete;
	Pipe& operator=(const Pipe& _Other) = delete;
	Pipe& operator=(Pipe&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	Direction Dir = Down;
	std::string_view Key = "Down";
	std::string_view Level = "";
	GameEngineCollision* Collision = nullptr;
	bool IsEnter = false;
	float Timer = 0;
	const float EnterTime = 0.5f;
	const float4 VerticalCollisionSize = { 32, 16 };
	const float4 VerticalCollisionPosition = { 0, 0 };
	const float4 HorizontalCollisionSize = { 16, 32 };
	const float4 HorizontalCollisionPosition = { 0, -16 };
};

