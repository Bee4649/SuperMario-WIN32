#include "ContentCore.h"
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "TitleLevel.h"
#include "WorldLevel.h"
#include "EndingLevel.h"
#include "GameOverLevel.h"
#include "EndingLevel.h"


ContentCore ContentCore :: Core;


ContentCore::ContentCore()
{
}

ContentCore::~ContentCore() 
{
}

void ContentCore::ResetGame()
{
	// ������ �ʱ�ȭ�Ѵ�
	Life = StartLife;
	Star = 0;
	CoinNum = 0;
	Score = 0;
}
void ContentCore::Start()
{
	// �ػ� ����
	// (256 x 224) * 4
	GameEngineWindow::SettingWindowSize({ 1024, 896 });


	// Ű ����
	if (false == GameEngineInput::IsKey("Start")) {
		GameEngineInput::CreateKey("Start", '-');
		GameEngineInput::CreateKey("Up", VK_UP);
		GameEngineInput::CreateKey("Left", VK_LEFT);
		GameEngineInput::CreateKey("Down", VK_DOWN);
		GameEngineInput::CreateKey("Right", VK_RIGHT);
		GameEngineInput::CreateKey("Dash", 'Z');
		GameEngineInput::CreateKey("Jump", 'X');
		GameEngineInput::CreateKey("Spin", 'C');
		GameEngineInput::CreateKey("CollisionDebug", 'O');
		GameEngineInput::CreateKey("U", 'U');
		GameEngineInput::CreateKey("I", 'I');
		GameEngineInput::CreateKey("MAPCHANGE", 'P');


	}


	new int();

	ResourcesLoad();

	// ��� ������ ����
	CreateLevel<TitleLevel>("Title");
	CreateLevel<WorldLevel>("World");
	CreateLevel<GameOverLevel>("GameOver");
	CreateLevel<EndingLevel>("Ending");


	// ���� (Title) ������ ����
	ChangeLevel("Title");


	
}

// �� �����Ӹ��� ����Ǵ� �Լ�
void ContentCore::Update()
{
	if (GameEngineInput::IsDown("CollisionDebug"))
	{
		CollisionDebug = !CollisionDebug;
	}
}

// ���α׷��� ������ ����Ǵ� �Լ�
void ContentCore::End()
{
}

void ContentCore::ResourcesLoad()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Texture");
	Dir.Move("World");
	// World ������ �ִ� ���ҽ��� �ҷ��´�
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("WORLD1.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("WORLD1FRONT.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGEBLOCK1.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGEBLOCK2.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SEA.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("MARIO.BMP"))->Cut(4, 5);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("CLOUD.BMP"))->Cut(1, 4);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("TIDE.BMP"))->Cut(1, 8);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGEBUTTON.BMP"))->Cut(1, 7);

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Texture");
	Dir.Move("TITLE");
	// Title ������ �ִ� ���ҽ��� �ҷ��´�
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("GAMEOVER.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("TITLE.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("ENDING.BMP"));

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Sound");
	// Sound ������ �ִ� ��� ���ҽ��� �ҷ��´�
	std::vector<GameEngineFile> Files = Dir.GetAllFile();

	for (size_t i = 0; i < Files.size(); i++)
	{
		GameEngineResources::GetInst().SoundLoad(Files[i].GetFullPath());
	}

}