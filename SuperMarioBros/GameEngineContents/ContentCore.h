#pragma once
#include <GameEngineCore/GameEngineCore.h>
#include "ContentsEnum.h"


// 설명 : 게임전체를 통괄할 클래스
// 싱글톤으로 구성되어 객체로 하나만 구현된다
// 레벨을 로딩하는 기능과 게임 전반의 데이터를 저장하는 기능이 있다
class ContentCore : public GameEngineCore
{
public:
	
	// delete Function
	ContentCore(const ContentCore& _Other) = delete;
	ContentCore(ContentCore&& _Other) noexcept = delete;
	ContentCore& operator=(const ContentCore& _Other) = delete;
	ContentCore& operator=(ContentCore&& _Other) noexcept = delete;

	// 싱글톤
	inline static ContentCore& GetInst()
	{
		return Core;
	}
	void ResetGame();
	bool GetCollisionDebug()
	{
		return CollisionDebug;
	}

	// 마리오 DATA Setter Getter
	inline PowerState GetMarioStateData() const {
		return MarioStateData;
	}
	inline ItemType GetStockStateData() const {
		return StockStateData;
	}
	inline int GetLife() {
		return Life;
	}
	inline int GetStar() {
		return Star;
	}
	inline int GetCoin() {
		return CoinNum;
	}
	inline int GetScore() {
		return Score;
	}
	inline void SetLife(int _Life) {
		Life = _Life;
	}
	inline void SetStar(int _Star) {
		Star = _Star;
	}
	inline void SetCoin(int _Coin) {
		CoinNum = _Coin;
	}
	inline void SetScore(int _Score) {
		Score = _Score;
	}
	inline void SetMarioStateData(PowerState _StateData) {
		MarioStateData = _StateData;
	}
	inline void SetStockStateData(ItemType _StateData) {
		StockStateData = _StateData;
	}


protected:

	// 시작하고 화면에 보여지고,
	void Start() override;

	// 행동한고.
	void Update() override;

	// 끝.
	void End() override;

private:

	// 생성자가 존재하지 않음
	ContentCore();
	~ContentCore();

	// 싱글톤 객체
	static ContentCore Core;


	// 마리오 데이터
	PowerState MarioStateData = PowerState::Normal;	// 마리오의 파워
	ItemType StockStateData = ItemType::Coin;		// 소지하고 있는 아이템
	const int StartLife = 3;						// 시작시 마리오의 목숨
	int Life = StartLife;							// 마리오의 목숨
	int	Star = 0;									// 보너스 별의 갯수
	int	CoinNum = 0;								// 코인의 갯수
	int	Score = 0;									// 점수
	bool CollisionDebug = false;					// 충돌체의 범위를 표시 여부
	
	void ResourcesLoad();
};
	

