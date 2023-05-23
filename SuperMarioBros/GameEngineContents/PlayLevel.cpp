#include "PlayLevel.h"
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "Monster.h"


// Contents
#include "Player.h"
#include "BackGround.h"

PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::Start() 
{

	if (false == ResourcesManager::GetInst().IsLoadTexture("Test.Bmp"))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");

		GameEnginePath FolderPath = FilePath;

		FilePath.MoveChild("ContentsResources\\Texture\\");
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("STAGE1COL.bmp"));
	}



	// ResourcesManager::GetInst().TextureLoad("AAA.Png", 경로);

	// 플레이 레벨이 만들어졌다.
	// 이 레벨에는 뭐가 있어야지?
	// 플레이어 만들고
	// 맵만들고
	// 몬스터 만들고
	// 액터

	// 자기 임의대로 만들겠다는 것이고 xxxxx
	// Player* NewPlayer = new Player();

	BackGroundPtr = CreateActor<BackGround>();
	BackGroundPtr->Init("Stage1Test.Bmp", "STAGE1COL.bmp");

	LevelPlayer = CreateActor<Player>();
	LevelPlayer->SetGroundTexture("STAGE1COL.bmp");
}


void PlayLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::IsDown('J'))
	{
		BackGroundPtr->SwitchRender();
	}

	if (1.0f <= GetLiveTime())
	{
		Monster* NewMonster = CreateActor<Monster>();
		ResetLiveTime();
	}

	// GameEngineCore::ChangeLevel("TitleLevel");
}
void PlayLevel::Render() 
{
}
void PlayLevel::Release() 
{
}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel) 
{
	if (nullptr == LevelPlayer)
	{
		MsgBoxAssert("플레이어를 세팅해주지 않았습니다");
	}

	LevelPlayer->SetGroundTexture("STAGE1COL.bmp");

	//float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	////LevelPlayer->SetPos(WinScale.Half());
	//// 0 0
	//// x y
	//GetMainCamera()->SetPos(LevelPlayer->GetPos() - WinScale.Half());

}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}