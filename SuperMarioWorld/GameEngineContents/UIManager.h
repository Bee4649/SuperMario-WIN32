#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>
#include "ContentsEnums.h"
class UIManager : public GameEngineActor
{
public:
	UIManager();
	~UIManager();

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

	UIManager(const UIManager& _Other) = delete;
	UIManager(UIManager&& _Other) noexcept = delete;
	UIManager& operator=(const UIManager& _Other) = delete;
	UIManager& operator=(UIManager&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:

	GameEngineRender* UIImage = nullptr;
	GameEngineRender* StockImageRender = nullptr;
	GameEngineRender* BlackRender = nullptr;
	GameEngineRender* ClearRender = nullptr;
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

