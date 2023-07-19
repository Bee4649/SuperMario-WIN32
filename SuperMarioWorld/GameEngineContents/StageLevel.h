#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>
#include "UIManager.h"
#include "ContentsEnums.h"

enum class ClearState {
	None,
	FadeOut,
	ClearBonus,
	FadeIn,
	WorldLoad
};

static float4 GridPos(int x, int y)
{
	return float4(static_cast<float>(x * 64 + 32), static_cast<float>((-y + 24) * 64));
}
static float4 ToGridPos(const float4& _Pos)
{
	return float4(static_cast<float>((_Pos.x-32) / 64), static_cast<float>(-(_Pos.y / 64 - 24)));
}

class StageLevel : public GameEngineLevel
{
public:
	StageLevel();
	~StageLevel();

	GameEngineSoundPlayer GetBGMPlayer()
	{
		return BGMPlayer;
	}

	void NewStockItem(ItemType _Item);
	void DropStockItem();
	void AddCoin();
	void AddLife();
	void AddScore(int _Score);
	void MarioDie();
	void LevelPause();
	void LevelPlay();
	void GoalEvent(int _Score);
	void SetTimer(float _Time);
	void SetCheckPoint(int _CheckPoint);
	int GetCheckPoint();
	float GetTimer();

	StageLevel(const StageLevel& _Other) = delete;
	StageLevel(StageLevel&& _Other) noexcept = delete;
	StageLevel& operator=(const StageLevel& _Other) = delete;
	StageLevel& operator=(StageLevel&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

	GameEngineSoundPlayer BGMPlayer = GameEngineSoundPlayer();
	UIManager* UI = nullptr;
private:
	ItemType Item = ItemType::Coin;
	ClearState State = ClearState::None;
	int Life = 3;
	int Star = 0;
	int Score = 0;
	int CoinNum = 0;
	float Timer = 0;
	float ClearEventTimer = 0;
	const float Time = 300;
	const int TimeBonus = 50;
	int StarBonus = 0;
	int CheckPoint = 0;
	bool IsClear = false;
	void CountTime(float _DeltaTime);

	void NoneUpdate();
	void FadeOutUpdate();
	void ClearBonusUpdate();
	void FadeInUpdate();
	void WorldLoadUpdate();
};
