#include "StageLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineActor.h>
#include "Mario.h"
#include "LevelLoader.h"
#include "SuperMushroom.h"
#include "FireFlower.h"
#include "WorldLevel.h"


StageLevel::StageLevel()
{ 
}

StageLevel::~StageLevel() 
{
}

void StageLevel::NewStockItem(ItemType _Item)
{
	switch (_Item)
	{
	case ItemType::UpMushroom:
		break;
	case ItemType::SuperMushroom:
		break;
	case ItemType::FireFlower:
		break;
	default:
		break;
	}
	GameEngineResources::GetInst().SoundPlay("ItemGet.wav");
	Item = _Item;
	UI->SetStockItem(_Item);
}

void StageLevel::DropStockItem()
{
	GameEngineActor* NewActor = nullptr;
	switch (Item)
	{
	case ItemType::Coin:
		return;
	case ItemType::UpMushroom:
		return;
	case ItemType::SuperMushroom:
		NewActor = CreateActor<SuperMushroom>(RenderOrder::Item);
		break;
	case ItemType::FireFlower:
	{
		FireFlower* NewFlower = CreateActor<FireFlower>(RenderOrder::Item);
		NewFlower->SetFall();
		NewActor = NewFlower;
		break;
	}
	default:
		return;
	}

	float4 SpawnPos = Mario::MainPlayer->GetPos();
	SpawnPos.Y = GetCameraPos().Y;

	NewActor->SetPos(SpawnPos);
	Item = ItemType::Coin;

	UI->SetStockItem(ItemType::Coin);
}

void StageLevel::AddCoin()
{
	GameEngineResources::GetInst().SoundPlay("Coin.WAV");
	while (++CoinNum >= 100)
	{
		CoinNum -= 100;
		AddLife();
	}
	UI->SetCoin(CoinNum);
	AddScore(100);
}

void StageLevel::AddLife()
{
	GameEngineResources::GetInst().SoundPlay("1UP.WAV");
	if (++Life >= 10)
	{
		Life = 9;
	}
	UI->SetLife(Life);
}

void StageLevel::AddScore(int _Score)
{
	Score += _Score;
	UI->SetScore(Score);
}

void StageLevel::MarioDie()
{
	Life--;
	if (0 >= Life)
	{
		LevelLoader::ChangeLevel("GameOver");
		return;
	}

	LevelLoader::ChangeLevel("World");
}

void StageLevel::LevelPlay()
{
	SetTimeScale(RenderOrder::Monster, 1);
	SetTimeScale(RenderOrder::Item, 1);
	SetTimeScale(RenderOrder::PlayerAttack, 1);
	SetTimeScale(RenderOrder::Map, 1);
}

void StageLevel::GoalEvent(int _Score)
{
	// »ç¿îµå
	BGMPlayer.Stop();
	GameEngineResources::GetInst().SoundPlay("CourseClear.mp3");
	Mario::MainPlayer->StageClear();
	LevelLoader::SetOrder(RenderOrder::Monster);
	StarBonus = _Score;
	ClearEventTimer = 0;
	IsClear = true;
	SetCheckPoint(0);
	WorldLevel::GetInstance()->StageClear(GetName());
}

void StageLevel::SetTimer(float _Time)
{
	Timer = _Time;
}

void StageLevel::SetCheckPoint(int _CheckPoint)
{
	CheckPoint = _CheckPoint;
}

int StageLevel::GetCheckPoint()
{
	return CheckPoint;
}

float StageLevel::GetTimer()
{
	return Timer;
}

void StageLevel::LevelPause()
{
	SetTimeScale(RenderOrder::Monster, 0);
	SetTimeScale(RenderOrder::Item, 0);
	SetTimeScale(RenderOrder::PlayerAttack, 0);
	SetTimeScale(RenderOrder::Map, 0);
}

void StageLevel::Loading()
{

}

void StageLevel::Update(float _DeltaTime)
{
	if (true == IsClear)
	{
		ClearEventTimer += _DeltaTime;
		switch (State)
		{
		case ClearState::None:
			NoneUpdate();
			break;
		case ClearState::FadeOut:
			FadeOutUpdate();
			break;
		case ClearState::ClearBonus:
			ClearBonusUpdate();
			break;
		case ClearState::FadeIn:
			FadeInUpdate();
			break;
		case ClearState::WorldLoad:
			WorldLoadUpdate();
			break;
		default:
			break;
		}
		return;
	}

	CountTime(_DeltaTime);
}

void StageLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	if ("World" == _PrevLevel->GetName())
	{
		Timer = Time;
	}
	Life = ContentCore::GetInst().GetLife();
	Star = ContentCore::GetInst().GetStar();
	CoinNum = ContentCore::GetInst().GetCoin();
	Score = ContentCore::GetInst().GetScore();
	Item = ContentCore::GetInst().GetStockStateData();
	IsClear = false;

	UI->SetValue(Life, Star, CoinNum, Score);
	UI->SetStockItem(Item);

	State = ClearState::None;

	CreateActor<LevelLoader>();

	LevelPlay();
}

void StageLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	for (int i = 0; i < static_cast<int>(RenderOrder::UI); i++)
	{
		std::vector<GameEngineActor*> Actors = GetActors(i);
		std::vector<GameEngineActor*>::iterator Start = Actors.begin();
		std::vector<GameEngineActor*>::iterator End = Actors.end();

		for (; Start != End; Start++)
		{
			if (nullptr != *Start)
			{
				(*Start)->Death();
			}
		}
		Mario::MainPlayer = nullptr;
	}
	SetCameraPos(float4::ZERO);

	ContentCore::GetInst().SetCoin(CoinNum);
	ContentCore::GetInst().SetLife(Life);
	ContentCore::GetInst().SetScore(Score);
	ContentCore::GetInst().SetStar(Star);
	ContentCore::GetInst().SetStockStateData(Item);

	BGMPlayer.Stop();
}

void StageLevel::CountTime(float _DeltaTime)
{
	Timer -= _DeltaTime;
	if (0 > Timer) {
		Mario::MainPlayer->Die();
		UI->SetTime(0);
		return;
	}
	UI->SetTime(static_cast<int>(Timer));
}

void StageLevel::NoneUpdate()
{
	if (1 < ClearEventTimer)
	{
		LevelLoader::FadeOut();
		State = ClearState::FadeOut;
		ClearEventTimer = 0;
	}
}

void StageLevel::FadeOutUpdate()
{
	if (1 < ClearEventTimer)
	{
		UI->ClearUIOn(0 < StarBonus);
		UI->SetClearBonus(static_cast<int>(Timer), static_cast<int>(Timer) * TimeBonus, StarBonus);
		State = ClearState::ClearBonus;
		ClearEventTimer = 0;
	}
}

void StageLevel::ClearBonusUpdate()
{
	if (1 > ClearEventTimer) { return; }

	bool Wait = false;
	if (1 <= Timer)
	{
		Timer -= 1;
		Score += TimeBonus;
		UI->SetTimeBonus(static_cast<int>(Timer) * TimeBonus);
		UI->SetScore(static_cast<int>(Score));
		Wait = true;
	}
	if (0 < StarBonus)
	{
		StarBonus--;
		Star++;
		if (100 <= Star)
		{
			Star -= 100;
			Life++;
			UI->SetLife(Life);
		}
		UI->SetStar(Star);
		UI->SetStarBonus(StarBonus);
		Wait = true;
	}
	if (4 < ClearEventTimer && false == Wait)
	{
		UI->ClearUIOff();
		LevelLoader::FadeIn();
		State = ClearState::FadeIn;
		ClearEventTimer = 0;
	}
}

void StageLevel::FadeInUpdate()
{
	if (2 < ClearEventTimer)
	{
		LevelLoader::SetOrder(RenderOrder::Fade);
		LevelLoader::ChangeLevel("World");
		State = ClearState::WorldLoad;
		ClearEventTimer = 0;
	}
}

void StageLevel::WorldLoadUpdate()
{
}

