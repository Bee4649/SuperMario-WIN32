#include "GameEngineCore.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineTime.h>
#include "GameEngineLevel.h"
#include "GameEngineResources.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineSound.h>

GameEngineCore* Core;

GameEngineCore* GameEngineCore::GetInst()
{
	return Core;
}

void GameEngineCore::GlobalStart()
{
	Core->Start();

	GameEngineTime::MainTimer.Reset();
}


void GameEngineCore::GlobalUpdate()
{

	// ������ �����Ҷ� �ѹ� ��ŸŸ���� ���ϰ�
	GameEngineSound::SoundUpdate();
	float TimeDeltaTime = GameEngineTime::MainTimer.Update();
	GameEngineInput::Update(TimeDeltaTime);

	// ���⿡�� ó���Ѵ�
	if (nullptr != Core->NextLevel)
	{
		GameEngineLevel* PrevLevel = Core->CurLevel;
		GameEngineLevel* NextLevel = Core->NextLevel;

		if (nullptr != PrevLevel)
		{
			PrevLevel->LevelChangeEnd(NextLevel);
			PrevLevel->ActorLevelChangeEnd(NextLevel);
		}

		Core->CurLevel = NextLevel;
		Core->NextLevel = nullptr;

		if (nullptr != NextLevel)
		{
			NextLevel->LevelChangeStart(PrevLevel);
			NextLevel->ActorLevelChangeStart(PrevLevel);
		}
	}

	if (1.0f / 60.0f <= TimeDeltaTime)
	{
		TimeDeltaTime = 1.0f / 60.0f;
	}

	Core->Update();
	if (nullptr == Core->CurLevel)
	{
		MsgAssert("������ ���������� ���� ���·� �ھ �����߽��ϴ�");
		return;
	}

	Core->CurLevel->Update(TimeDeltaTime);
	Core->CurLevel->ActorsUpdate(TimeDeltaTime);
	GameEngineWindow::DoubleBufferClear();
	Core->CurLevel->ActorsRender(TimeDeltaTime);
	GameEngineWindow::DoubleBufferRender();
	Core->CurLevel->Release();
}


void GameEngineCore::GlobalEnd()
{
	Core->End();

	GameEngineResources::GetInst().Release();
}


GameEngineCore::GameEngineCore() 
{
	GameEngineDebug::LeakCheck();
	// ���� �ڽ��߿� �ϳ��ϼ��ۿ� ����.
	// ���� ���� ������� �� ���� �����̴�.
	Core = this;
}

GameEngineCore::~GameEngineCore() 
{
	std::map<std::string, GameEngineLevel*>::iterator StartIter = Levels.begin();
	std::map<std::string, GameEngineLevel*>::iterator EndIter = Levels.end();

	for (size_t i = 0; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
		}
	}

	Levels.clear();
}

void GameEngineCore::CoreStart(HINSTANCE _Instance)
{
	if (false == GameEngineInput::IsKey("EngineMouseLeft"))
	{
		GameEngineInput::CreateKey("EngineMouseLeft", VK_LBUTTON);
		GameEngineInput::CreateKey("EngineMouseRight", VK_RBUTTON);
	}

	GameEngineWindow::WindowCreate(_Instance, "Super Mario Wolrd", { 1280, 720 }, { 0, 0 });
	GameEngineWindow::MessageLoop(GameEngineCore::GlobalStart, GameEngineCore::GlobalUpdate, GameEngineCore::GlobalEnd);
}

void GameEngineCore::ChangeLevel(const std::string_view& _Name)
{
	std::map<std::string, GameEngineLevel*>::iterator FindIter = Levels.find(_Name.data());

	if (FindIter == Levels.end())
	{
		std::string Name = _Name.data();
		MsgAssert(Name + "�������� �ʴ� ������ �����Ű���� �߽��ϴ�");
		return;
	}

	NextLevel = FindIter->second;
}

void GameEngineCore::LevelLoading(GameEngineLevel* _Level, const std::string_view& _Name)
{
	if (nullptr == _Level)
	{
		MsgAssert("nullptr �� ������ �ε��Ϸ��� �߽��ϴ�.");
		return;
	}

	_Level->SetName(_Name);
	_Level->Loading();
}