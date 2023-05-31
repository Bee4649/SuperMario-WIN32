#pragma once

#include <string>
#include <string_view>


// ���� : ��� �⺻���� �ൿ�� �����ϴ� Ŭ����
// �����ϴ� Ŭ����
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
		//         ����          ?              true �϶�                                      :         false �϶�
		// �θ� �ִٸ�
		// ((true == ObjectUpdate && false == IsDeath()) && true == Parent->IsUpdate())
		// ���� �����־���ϰ� true == ObjectUpdate
		// ���� ������ �ʾҾ�� �Ѵ�. false == IsDeath()
		// �θ� �����־�� �Ѵ� true == Parent->IsUpdate()

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

	// �ڱ⸦ �����ϰų� �ڱ⸦ ������ ������Ʈ���� �θ��� ������ �ϴ°�.
	GameEngineObject* Parent = nullptr;

	bool IsUpdateValue = true; // �̰� false�� ����� �˴ϴ�.
	bool IsDeathValue = false; // �ƿ� �޸𸮿��� ���������� �;�.

	std::string Name;
};

