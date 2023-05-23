#include "TitleLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include "BackGround.h"
#include "ContentCore.h"
#include "PlayLevel.h"
#include "ContentsEnum.h"



TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}



void TitleLevel::Update(float _DeltaTime) 
{
	if (true == GameEngineInput::IsDown('P'))
	{
		GameEngineCore::ChangeLevel("PlayLevel");
	}
}


void LevelStart(GameEngineLevel* _PrevLevel) 
{
}
void LevelEnd(GameEngineLevel* _NextLevel) 
{
}