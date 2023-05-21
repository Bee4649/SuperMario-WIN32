#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class PlayerState 
{
	Idle,
	Run,
	Max, // 일반적으로 사용하지 않는 값.
};

enum class PlayerDir
{
	Right,
	Left,
	Max,
};

// 설명 :
class Player : public GameEngineActor
{
public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	GameEngineRenderer* MainRenderer = nullptr;

protected:
	void StateUpdate(float _Delta);

	void IdleStart();
	void RunStart();

	// 클래스로 만들어도 되고.
	void IdleUpdate(float _Delta);
	void RunUpdate(float _Delta);

	void ChanageState(PlayerState State);

	PlayerState State = PlayerState::Max;
	PlayerDir Dir = PlayerDir::Right;
	std::string CurState = "";

	void DirCheck();

	void ChangeAnimationState(const std::string& _StateName);

private:
	void Start() override;
	void Update(float _Delta) override;
};

