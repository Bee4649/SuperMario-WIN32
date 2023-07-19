#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>
class GameOverLevel : public GameEngineLevel
{
public:
	GameOverLevel();
	~GameOverLevel();

	GameOverLevel(const GameOverLevel& _Other) = delete;
	GameOverLevel(GameOverLevel&& _Other) noexcept = delete;
	GameOverLevel& operator=(const GameOverLevel& _Other) = delete;
	GameOverLevel& operator=(GameOverLevel&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}
private:
	GameEngineSoundPlayer BGMPlayer;
};

