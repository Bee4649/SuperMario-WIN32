#pragma once

#include <string>
#include <string_view>


// 설명 : 모든 기본적인 행동을 제안하는 클래스
// 제안하는 클래스
class GameEngineObject
{
	friend class GameEngineLevel;
	friend class GameEngineCore;

public:
	// constrcuter destructer
	GameEngineObject();
	virtual ~GameEngineObject();

	// delete Function
	GameEngineObject(const GameEngineObject& _Other) = delete;
	GameEngineObject(GameEngineObject&& _Other) noexcept = delete;
	GameEngineObject& operator=(const GameEngineObject& _Other) = delete;
	GameEngineObject& operator=(GameEngineObject&& _Other) noexcept = delete;

	bool IsUpdate() 
	{
		//         조건          ?              true 일때                                      :         false 일때
		// 부모가 있다면
		// ((true == ObjectUpdate && false == IsDeath()) && true == Parent->IsUpdate())
		// 내가 켜져있어야하고 true == ObjectUpdate
		// 내가 죽지도 않았어야 한다. false == IsDeath()
		// 부모도 켜져있어야 한다 true == Parent->IsUpdate()

		return nullptr != Parent ? ((true == IsUpdateValue && false == IsDeath()) && true == Parent->IsUpdate()) : (IsUpdateValue && false == IsDeath());

	}

	virtual bool IsDeath()
	{
		return nullptr != Parent ? (true == IsDeathValue || Parent->IsDeath()) : (true == IsDeathValue);
	}

	void Death()
	{
		IsDeathValue = true;
	}

	virtual void On()
	{
		IsUpdateValue = true;
	}

	virtual void Off()
	{
		IsUpdateValue = false;
	}

	int GetOrder()
	{
		return Order;
	}

	virtual void SetOrder(int _Order)
	{
		Order = _Order;
	}

	virtual void OnOffSwtich()
	{
		IsUpdateValue = !IsUpdateValue;
	}

	virtual void SetOwner(GameEngineObject* _Parent)
	{
		Parent = _Parent;
	}

	template<typename ConvertType>
	ConvertType* GetOwner()
	{
		return dynamic_cast<ConvertType*>(Parent);
	}

	GameEngineObject* GetOwner()
	{
		return Parent;
	}

	void SetName(const std::string_view& _View)
	{
		Name = _View;
	}

	const std::string& GetName()
	{
		return Name;
	}

	std::string GetNameCopy()
	{
		return Name;
	}

protected:


private:
	// float LiveTime = 0.0f;

	int Order = 0;

	// 자기를 관리하거나 자기를 소유한 오브젝트들을 부모라는 보려고 하는것.
	GameEngineObject* Parent = nullptr;

	bool IsUpdateValue = true; // 이걸 false로 만들면 됩니다.
	bool IsDeathValue = false; // 아예 메모리에서 날려버리고 싶어.

	std::string Name;
};

