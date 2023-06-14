#include "GameOverLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include "LevelLoader.h"
#include "ContentsEnum.h"
#include "ContentCore.h"

GameOverLevel::GameOverLevel() {

}

GameOverLevel::~GameOverLevel() {

}

void GameOverLevel::Loading()
{
	GameEngineRenderer* ImageRender = CreateActor<GameEngineActor>()->CreateRender("GameOver.BMP", RenderOrder::BackGround);
	ImageRender->SetScaleToImage();
	ImageRender->SetPosition(ImageRender->GetScale().half());
	CreateActor<LevelLoader>();

}
void GameOverLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsAnyKey())
	{
		BGMPlayer.Stop();
		LevelLoader::ChangeLevel("Title");
	}
}

void GameOverLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("GameOver.mp3");
	BGMPlayer.LoopCount(1);

	ContentCore::GetInst().ResetGame();
}
