#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>

enum class BarState 
{
	Up,
	Down
};

class ClearBar;
class Goal : public GameEngineActor
{
	friend ClearBar;
public:
	Goal();
	~Goal();

	void SetGoal(const float4& _Pos);


	Goal(const Goal& _Other) = delete;
	Goal(Goal&& _Other) noexcept = delete;
	Goal& operator=(const Goal& _Other) = delete;
	Goal& operator=(Goal&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
private:
	bool IsClear = false;
	GameEngineActor* BarActor = nullptr;
	GameEngineRenderer* GoalRender = nullptr;
	GameEngineCollision* Collision = nullptr;
	const float4 CollisionScale = { 64, 5000 };
	const float4 CollisionPos = { 64, -2500 };

	void GoalEvent(int _Score);
};

class ClearBar : public GameEngineActor
{
	friend Goal;

public:
	GameEngineActor* SetGoalActor(Goal* _Goal);
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	Goal* GoalActor = nullptr;
	GameEngineRenderer* BarRender = nullptr;
	GameEngineCollision* Collision = nullptr;
	const float4 CollisionScale = { 96, 16 };
	const float Speed = 0.5f;
	const float Height = 500;
	const int HeightScore = 50;
	float4 UpPos = float4::ZERO;
	float4 DownPos = float4::ZERO;
	float Timer = 0;
	bool IsMoveUp = true;

	BarState State = BarState::Up;
	void UpUpdate(float _DeltaTime);
	void DownUpdate(float _DeltaTime);
};

