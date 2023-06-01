#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineObject.h"
#include <string_view>
#include <list>
#include <Windows.h>


// 설명 : 화면안에 존재하는 플레이어 몬스터 총알 등등등 존재한다고 치고
// 위치가 있다면 이 녀석으로 표현해야 합니다.
// 화면에 존재하고 위치가 있어야하는 모든것들의 기본기능을 지원
class GameEngineLevel;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineActor : public GameEngineObject
{
	friend GameEngineLevel;
public:
	// constrcuter destructer
	GameEngineActor();
	~GameEngineActor();

	// delete Function
	GameEngineActor(const GameEngineActor& _Other) = delete;
	GameEngineActor(GameEngineActor&& _Other) noexcept = delete;
	GameEngineActor& operator=(const GameEngineActor& _Other) = delete;
	GameEngineActor& operator=(GameEngineActor&& _Other) noexcept = delete;

	inline void SetPos(const float4& _Pos)
	{
		Pos = _Pos;
	}

	inline void SetMove(const float4& _MovePos)
	{
		Pos += _MovePos;
	}

	inline float4 GetPos() 
	{
		return Pos;
	}

	template<typename EnumType>
	GameEngineRenderer* CreateRender(const std::string_view& _Image, EnumType _Order)
	{
		return CreateRender(_Image, static_cast<int>(_Order));
	}

	template<typename EnumType>
	GameEngineRenderer* CreateRender(EnumType _Order)
	{
		return CreateRender(static_cast<int>(_Order));
	}

	GameEngineRenderer* CreateRender(const std::string_view& _Image, int _Order = 0);
	GameEngineRenderer* CreateRender(int _Order = 0);

	template<typename EnumType>
	GameEngineCollision* CreateCollision(EnumType _GroupIndex)
	{
		return CreateCollision(static_cast<int>(_GroupIndex));
	}

	GameEngineCollision* CreateCollision(int _GroupIndex = 0);

	GameEngineLevel* GetLevel();

protected:

	// 시작하기전에 준비해야할것들을 실행할 곳 
	virtual void Start() {}

	// 매 프레임마다 처리되야할 것들
	virtual void Update(float _DeltaTime) {}

	// 순서를 제어 함수를 한번 더실행하고 
	virtual void LateUpdate(float _DeltaTime) {}

	// 화면에 그려지는 기능들을 여기서 처리
	virtual void Render(float _DeltaTime) {}

	inline float GetLiveTime()
	{
		return LiveTime;
	}

	virtual void LevelChangeEnd(GameEngineLevel* _NextLevel);
	virtual void LevelChangeStart(GameEngineLevel* _PrevLevel);

private:

	float TimeScale = 0.0;
	float LiveTime = 0.0;
	float4 Pos = { 0.0f, 0.0f };

	std::list<GameEngineRenderer*> RenderList;
	std::list<GameEngineCollision*> CollisionList;

	void Release();

};

