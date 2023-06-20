#include "ContentCore.h"
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include "TitleLevel.h"
#include "WorldLevel.h"
#include "StageLevel1.h"
#include "StageUnderground1.h"
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
		GameEngineInput::CreateKey("Start", VK_SPACE);
		GameEngineInput::CreateKey("Up", VK_UP);
		GameEngineInput::CreateKey("Left", VK_LEFT);
		GameEngineInput::CreateKey("Down", VK_DOWN);
		GameEngineInput::CreateKey("Right", VK_RIGHT);
		GameEngineInput::CreateKey("Dash", 'Z');
		GameEngineInput::CreateKey("Jump", 'X');
		GameEngineInput::CreateKey("Spin", 'C');
		GameEngineInput::CreateKey("O", 'O');
		GameEngineInput::CreateKey("U", 'U');
		GameEngineInput::CreateKey("I", 'I');
		GameEngineInput::CreateKey("P", 'P');


	}


	new int();

	ResourcesLoad();

	// 모든 레벨을 생성
	CreateLevel<TitleLevel>("Title");
	CreateLevel<WorldLevel>("World");
	CreateLevel<StageLevel1>("Stage1");
	CreateLevel<StageUnderground1>("Underground1");
	CreateLevel<GameOverLevel>("GameOver");
	CreateLevel<EndingLevel>("Ending");


	// 시작 (Title) 레벨로 변경
	ChangeLevel("Title");

}

// 매 프레임마다 실행되는 함수
void ContentCore::Update()
{
	if (GameEngineInput::IsDown("O"))
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
	Dir.Move("Play");
	// Play 폴더에 있는 리소스를 불러온다
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("RIGHT_MARIO.BMP"))->Cut(17, 9);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("LEFT_MARIO.BMP"))->Cut(17, 9);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("BACKGROUND1.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("BACKGROUND3.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGE1.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGE1DEBUG.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Underground1.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGE0COL.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("STAGE1COL.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Underground1COL.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("ALLBLACK.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("NUMBER.BMP"))->Cut(10, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("NUMBERBIG.BMP"))->Cut(10, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("NUMBERYELLOW.BMP"))->Cut(10, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("CheckPoint.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("CheckPointPass.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("CheckPointFront.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("BAMBA.BMP"))->Cut(8, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("RIGHT_NOKONOKO.BMP"))->Cut(3, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("RIGHT_NOKONOKO_HADAKA.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("LEFT_NOKONOKO.BMP"))->Cut(3, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("LEFT_NOKONOKO_HADAKA.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SHELL.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SUPERMUSHROOM.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("KILLER.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("FIRE.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("FIREFLOWER.BMP"))->Cut(2, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("FIREFLOWERUI.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SMOKE.BMP"))->Cut(5, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SKIDDUST1.BMP"))->Cut(5, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("SKIDDUST.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("KICK.BMP"))->Cut(3, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("BLOCK.BMP"))->Cut(4, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Blockdebrits.BMP"))->Cut(6, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("PLAYUI.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("NONE.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Clear.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("ClearBar.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Goal.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("ClearBonus.BMP"));
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("StarParticle.BMP"))->Cut(1, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("QuestionBlock.BMP"))->Cut(5, 1);
	GameEngineResources::GetInst().TextureLoad(Dir.GetPlusFileName("Coin.BMP"))->Cut(4, 1);

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