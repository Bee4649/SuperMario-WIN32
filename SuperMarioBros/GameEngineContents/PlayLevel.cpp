#include "PlayLevel.h"
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/ResourcesManager.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include "Monster.h"
#include "PlayUIManager.h"


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
	GameEngineSound::SetGlobalVolume(0.05f);

	if (false == ResourcesManager::GetInst().IsLoadTexture("Test.Bmp"))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");

		GameEnginePath FolderPath = FilePath;

		FilePath.MoveChild("ContentsResources\\Texture\\");
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("StageTestPixel.bmp"));
	}

	if (nullptr == GameEngineSound::FindSound("BGMTest.mp3"))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Sound\\");

		GameEngineSound::SoundLoad(FilePath.PlusFilePath("BGMTest.mp3"));
	}




	// ResourcesManager::GetInst().TextureLoad("AAA.Png", ���);

	// �÷��� ������ ���������.
	// �� �������� ���� �־����?
	// �÷��̾� �����
	// �ʸ����
	// ���� �����
	// ����

	// �ڱ� ���Ǵ�� ����ڴٴ� ���̰� xxxxx
	// Player* NewPlayer = new Player();

	BackGroundPtr = CreateActor<BackGround>();
	BackGroundPtr->Init("StageTest.Bmp", "StageTestPixel.bmp");

	LevelPlayer = CreateActor<Player>();
	LevelPlayer->SetGroundTexture("StageTestPixel.bmp");

	CreateActor<PlayUIManager>();

}


void PlayLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown('O'))
	{
		BGMPlayer.Stop();

		// GameEngineCore::ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::IsDown('J'))
	{
		BackGroundPtr->SwitchRender();
	}

	if (0.0f <= GetLiveTime())
	{
		Monster* NewMonster = CreateActor<Monster>();
		ResetLiveTime();
	}

	// GameEngineCore::ChangeLevel("TitleLevel");
}
void PlayLevel::Release() 
{
}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel) 
{


	if (nullptr == LevelPlayer)
	{
		MsgBoxAssert("�÷��̾ ���������� �ʾҽ��ϴ�");
	}

	BGMPlayer = GameEngineSound::SoundPlay("BGMTest.mp3");

	LevelPlayer->SetGroundTexture("StageTestPixel.bmp");

	//float4 WinScale = GameEngineWindow::MainWindow.GetScale();
	////LevelPlayer->SetPos(WinScale.Half());
	//// 0 0
	//// x y
	//GetMainCamera()->SetPos(LevelPlayer->GetPos() - WinScale.Half());

}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}