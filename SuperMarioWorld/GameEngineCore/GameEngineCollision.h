#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineComponent.h"
#include <string>
#include <map>
#include <vector>


enum CollisionType 
{
	Point, // 점
	Rect, // 사각형
	CirCle, // 원
	Max, // 원
	
};

class CollisionCheckParameter
{
public:
	int TargetGroup = -342367842;
	CollisionType TargetColType = CollisionType::CirCle;
	CollisionType ThisColType = CollisionType::CirCle;
};

// 설명 :
class CollisionFunctionInit;
class GameEngineCollision : public GameEngineComponent
{

	friend CollisionFunctionInit;
	
public:
	static bool CollisionCirCleToCirCle(const CollisionData& _Left, const CollisionData& _Right);
	static bool CollisionCirCleToPoint(const CollisionData& _Left, const CollisionData& _Right);

	static bool CollisionRectToRect(const CollisionData& _Left, const CollisionData& _Right);
	static bool CollisionRectToPoint(const CollisionData& _Left, const CollisionData& _Right);

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	bool Collision(const CollisionCheckParameter& _CollisionCheck);

	bool Collision(const CollisionCheckParameter& _CollisionCheck, std::vector<GameEngineCollision*>& _Collision);

	void SetOrder(int _Order) override;

	CollisionData GetCollisionData();

	void SetDebugRenderType(CollisionType _DebugRenderType)
	{
		DebugRenderType = _DebugRenderType;
	}

	void DebugRender();


protected:

private:
	CollisionType DebugRenderType = CollisionType::CirCle;

};

