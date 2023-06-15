#pragma once
#include <GameEngineBase/GameEngineDebug.h>
#include <Windows.h>
#include <string_view>
#include <map>

// ���� :
class GameEngineLevel;
class GameEngineCore
{

private:

	static void GlobalStart();
	static void GlobalUpdate();
	static void GlobalEnd();

public:

	// constrcuter destructer
	GameEngineCore();
	~GameEngineCore();

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	void CoreStart(HINSTANCE _Instance);

	void ChangeLevel(const std::string_view& _Name);


	static GameEngineCore* GetInst();

	void DebugSwitch()
	{
		IsDebugValue = !IsDebugValue;
	}

	void SetDebugMode(bool _IsDebug)
	{
		IsDebugValue = _IsDebug;
	}

	bool IsDebug()
	{
		return IsDebugValue;
	}


protected:

	template<typename LevelType>
	void CreateLevel(const std::string& _Name) 
	{
		// Title�� ������µ�
		// �� Title�� ����ڴٰ� �� ��Ȳ
		// �̹� ���ο� TitleLevel�� �����Ѵ�.
		if (Levels.end() != Levels.find(_Name.data()))
		{
			std::string Name = _Name.data();
			MsgAssert(Name + "�� �̸��� ���� GameEngineLevel�� �̹� �����մϴ�.");
			return;
		}

		// ��ĳ������ �������ϱ�
		GameEngineLevel* NewLevel = new LevelType();

		LevelLoading(NewLevel, _Name);
		// Level->Loading();
		// insert�Ҷ����� ���ο� string�� ����鼭 �ڽŸ��� �޸𸮸� ������ �˴ϴ�.
		Levels.insert(std::make_pair(_Name.data(), NewLevel));
		
		//std::pair<std::map<std::string, class GameEngineLevel*>::iterator, bool> Pair 
		//	= AllLevel.insert(std::make_pair(_Title, nullptr));

		//if (false == Pair.second)
		//{
		//	MsgBoxAssert("�̹� �����ϴ� �̸��� ������ �� ������� �߽��ϴ�" + _Title);
		//	return;
		//}
	}

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;

private:

	// �׷��� map�� ����Ѵ�. �����̶�°��� ����̰�,
	// GameEngineLevel�� "��� �̸�"���� ã�� �̸����� �����Ű��.
	std::map<std::string, GameEngineLevel*> Levels;
	
	GameEngineLevel* CurLevel = nullptr;

	GameEngineLevel* NextLevel = nullptr;

	void LevelLoading(GameEngineLevel* _Level, const std::string_view& _Name);

	bool IsDebugValue = false;



};
