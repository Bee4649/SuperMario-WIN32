#pragma once
#include <GameEngineCore/GameEngineCore.h>

#include "ContentsEnums.h"

// ������ü�� ����� Ŭ����
// �̱������� �����Ǿ� ��ü�� �ϳ��� �����ȴ�
// ������ �ε��ϴ� ��ɰ� ���� ������ �����͸� �����ϴ� ����� �ִ�
class MarioGameCore : public GameEngineCore
{
public:
	// delete Function
	MarioGameCore(const MarioGameCore& _Other) = delete;
	MarioGameCore(MarioGameCore&& _Other) noexcept = delete;
	MarioGameCore& operator=(const MarioGameCore& _Other) = delete;
	MarioGameCore& operator=(MarioGameCore&& _Other) noexcept = delete;


	// �̱���
	inline static MarioGameCore& GetInst()
	{
		return Core;
	}
	void ResetGame();
	bool GetCollisionDebug()
	{
		return CollisionDebug;
	}

#pragma region __________������ ������ Setter Getter__________
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

#pragma endregion

protected:
	void Start() override;
	void Update() override;
	void End() override;


private:
	// �����ڰ� �������� ����
	MarioGameCore();
	~MarioGameCore();

	// �̱��� ��ü
	static MarioGameCore Core;
		
	// ������ ������
	PowerState MarioStateData = PowerState::Normal;	// �������� �Ŀ�
	ItemType StockStateData = ItemType::Coin;		// �����ϰ� �ִ� ������
	const int StartLife = 3;						// ���۽� �������� ���
	int Life = StartLife;							// �������� ���
	int	Star = 0;									// ���ʽ� ���� ����
	int	CoinNum = 0;								// ������ ����
	int	Score = 0;									// ����
	bool CollisionDebug = false;					// �浹ü�� ������ ǥ�� ����
	void ResourcesLoad();
};
