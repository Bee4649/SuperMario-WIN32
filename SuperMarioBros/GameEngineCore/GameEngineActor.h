#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineObject.h"
#include <string_view>
#include <list>
#include <Windows.h>


// ���� : ȭ��ȿ� �����ϴ� �÷��̾� ���� �Ѿ� ���� �����Ѵٰ� ġ��
// ��ġ�� �ִٸ� �� �༮���� ǥ���ؾ� �մϴ�.
// ȭ�鿡 �����ϰ� ��ġ�� �־���ϴ� ���͵��� �⺻����� ����
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

	// �����ϱ����� �غ��ؾ��Ұ͵��� ������ �� 
	virtual void Start() {}

	// �� �����Ӹ��� ó���Ǿ��� �͵�
	virtual void Update(float _DeltaTime) {}

	// ������ ���� �Լ��� �ѹ� �������ϰ� 
	virtual void LateUpdate(float _DeltaTime) {}

	// ȭ�鿡 �׷����� ��ɵ��� ���⼭ ó��
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

