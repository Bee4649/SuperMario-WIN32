#include "PlayLevel.h"
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/ResourcesManager.h>


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
	// ResourcesManager::GetInst().TextureLoad("AAA.Png", 경로);

	// 플레이 레벨이 만들어졌다.
	// 이 레벨에는 뭐가 있어야지?
	// 플레이어 만들고
	// 맵만들고
	// 몬스터 만들고
	// 액터

	// 자기 임의대로 만들겠다는 것이고 xxxxx
	// Player* NewPlayer = new Player();

	BackGround* Back = CreateActor<BackGround>();
	Back->Init("StageTest.Bmp");

	CreateActor<Player>();
}


void PlayLevel::Update(float _Delta)
{
	// GameEngineCore::ChangeLevel("TitleLevel");
}
void PlayLevel::Render() 
{
}
void PlayLevel::Release() 
{
}