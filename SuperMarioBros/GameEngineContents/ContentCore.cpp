#include "ContentCore.h"
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>

#include "TitleLevel.h"
#include "EndingLevel.h"
#include "GameOverLevel.h"

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
		GameEngineInput::CreateKey("CollisionDebug", 'J');
		GameEngineInput::CreateKey("1", '1');
		GameEngineInput::CreateKey("2", '2');
		GameEngineInput::CreateKey("3", '3');
		GameEngineInput::CreateKey("4", '4');
		
	}


	new int();

	ResourcesLoad();

	// 모든 레벨을 생성
	CreateLevel<TitleLevel>("Title");
	CreateLevel<GameOverLevel>("GameOver");
	CreateLevel<EndingLevel>("Ending");

	// 시작 (Title) 레벨로 변경
	ChangeLevel("Title");







	// 이 레벨이 화면에 보여라.
	GameEngineCore::ChangeLevel("PlayLevel");
}
//
// 행동한고.
//void ContentCore::Update(float _Delta)
//{
//}
//
// 그려지고.
//void ContentCore::Render(float _Delta)
//{
//}
//
// 정리된다.
//void ContentCore::Release() 
//{
//}