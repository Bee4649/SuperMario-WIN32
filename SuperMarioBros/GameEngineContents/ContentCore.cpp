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
		GameEngineInput::CreateKey("CollisionDebug", 'J');
		GameEngineInput::CreateKey("1", '1');
		GameEngineInput::CreateKey("2", '2');
		GameEngineInput::CreateKey("3", '3');
		GameEngineInput::CreateKey("4", '4');
		
	}


	new int();

	ResourcesLoad();

	// ��� ������ ����
	CreateLevel<TitleLevel>("Title");
	CreateLevel<GameOverLevel>("GameOver");
	CreateLevel<EndingLevel>("Ending");

	// ���� (Title) ������ ����
	ChangeLevel("Title");







	// �� ������ ȭ�鿡 ������.
	GameEngineCore::ChangeLevel("PlayLevel");
}
//
// �ൿ�Ѱ�.
//void ContentCore::Update(float _Delta)
//{
//}
//
// �׷�����.
//void ContentCore::Render(float _Delta)
//{
//}
//
// �����ȴ�.
//void ContentCore::Release() 
//{
//}