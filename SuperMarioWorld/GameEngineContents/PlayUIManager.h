#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>
#include "ContentsEnum.h"

// 설명 :
class PlayUIManager : public GameEngineActor
{
public:
	// constrcuter destructer
	PlayUIManager();
	~PlayUIManager();

	// delete Function
	PlayUIManager(const PlayUIManager& _Other) = delete;
	PlayUIManager(PlayUIManager&& _Other) noexcept = delete;
	PlayUIManager& operator=(const PlayUIManager& _Other) = delete;
	PlayUIManager& operator=(PlayUIManager&& _Other) noexcept = delete;

public:
	void SetLife(int Value);
	void SetStar(int Value);
	void SetTime(int Value);
	void SetCoin(int Value);
	void SetScore(int Value);
	void SetValue(int Life, int Star, int CoinNum, int Score);
	void SetStockItem(ItemType _Item);
	void ClearUIOn(bool _IsStarBonus);
	void ClearUIOff();
	void SetClearBonus(int _ClearTime, int _TimeBonus, int _StarBonus);
	void SetTimeBonus(int _TimeBonus);
	void SetStarBonus(int _StarBonus);



protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:

	GameEngineRenderer* UIImage = nullptr;
	GameEngineRenderer* StockImageRender = nullptr;
	GameEngineRenderer* BlackRender = nullptr;
	GameEngineRenderer* ClearRender = nullptr;
	NumberRenderObject LifeNumber;		// 최대 99
	NumberRenderObject StarNumber;		// 최대 99
	NumberRenderObject TimeNumber;		// 최대 999
	NumberRenderObject CoinNumber;		// 최대 99
	NumberRenderObject ScoreNumber;		// 최대 999만

	NumberRenderObject ClearTimeNumber;		// 최대 999만
	NumberRenderObject TimeBonus;		// 최대 999만
	NumberRenderObject StarBonus;		// 최대 999만

	const float4 NumberScale = { 32 ,28 };
	const float4 NumberBigScale = { 32 ,56 };
	int MarioLife = 0;


};

