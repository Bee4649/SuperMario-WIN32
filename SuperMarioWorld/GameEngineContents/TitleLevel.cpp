#include "TitleLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include "ContentCore.h"
#include "LevelLoader.h"

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}


void TitleLevel::Loading()
{
	// Ÿ��Ʋ �̹������� ǥ���� ���� ����
	GameEngineRenderer* TitleRender = CreateActor<GameEngineActor>()->CreateRender("TITLE.BMP", RenderOrder::BackGround);
	TitleRender->SetScaleToImage();
	TitleRender->SetPosition(TitleRender->GetScale().half());

	CreateActor<LevelLoader>();

}

void TitleLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsAnyKey()) {
		LevelLoader::ChangeLevel("World");
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	BGMPlayer = GameEngineResources::GetInst().SoundPlayToControl("Title.mp3");
	BGMPlayer.LoopCount(0);
}

void TitleLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	BGMPlayer.Stop();
}

