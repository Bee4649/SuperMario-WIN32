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
	// 게임을 초기화한다
	Life = StartLife;
	Star = 0;
	CoinNum = 0;
	Score = 0;
}
void ContentCore::Start()
{
	// 해상도 설정
	// (256 x 224) * 4
	GameEngineWindow::SettingWindowSize({ 1024, 896 });


	// 키 설정
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

	// 모든 레벨을 생성
	CreateLevel<TitleLevel>("Title");
	CreateLevel<WorldLevel>("World");
	CreateLevel<GameOverLevel>("GameOver");
	CreateLevel<EndingLevel>("Ending");


	// 시작 (Title) 레벨로 변경
	ChangeLevel("Title");


	
}

// 매 프레임마다 실행되는 함수
void ContentCore::Update()
{
	if (GameEngineInput::IsDown("CollisionDebug"))
	{
		CollisionDebug = !CollisionDebug;
	}
}

// 프로그램이 꺼질때 실행되는 함수
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
	// World 폴더에 있는 리소스를 불러온다
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
	// Title 폴더에 있는 리소스를 불러온다
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("GAMEOVER.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("TITLE.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("ENDING.BMP"));

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Sound");
	// Sound 폴더에 있는 모든 리소스를 불러온다
	std::vector<GameEngineFile> Files = Dir.GetAllFile();

	for (size_t i = 0; i < Files.size(); i++)
	{
		GameEngineResources::GetInst().SoundLoad(Files[i].GetFullPath());
	}

}