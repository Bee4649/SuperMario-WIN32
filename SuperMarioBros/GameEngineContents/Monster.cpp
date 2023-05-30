#include "Monster.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"
#include <GameEngineCore/ResourcesManager.h>
#include "Player.h"

std::list<Monster*> Monster::AllMonster;

Monster::Monster() 
{
	AllMonster.push_back(this);
}

Monster::~Monster() 
{
}

void Monster::AllMonsterDeath()
{
	for (Monster* Monster : AllMonster)
	{
		Monster->Death();
	}

	AllMonster.clear();
}

void Monster::Update(float _Delta)
{
	// Player::MainPlayer = nullptr;

	float4 Dir = Player::GetMainPlayer()->GetPos() - GetPos();

	Dir.Normalize();

	// Dir <= 거리가 일정하지 않다는 게 문제에요.

	// Dir *= 0.1f;

	AddPos(Dir * _Delta * 100.0f);
}

void Monster::Start() 
{
	if (false == ResourcesManager::GetInst().IsLoadTexture("Test.Bmp"))
	{
		GameEnginePath FilePath;
		FilePath.SetCurrentPath();
		FilePath.MoveParentToExistsChild("ContentsResources");
		FilePath.MoveChild("ContentsResources\\Texture\\Player\\");
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("Test.bmp"));
		ResourcesManager::GetInst().TextureLoad(FilePath.PlusFilePath("HPBar.bmp"));
	}

	{
		GameEngineRenderer* Ptr = CreateRenderer("HPBar.bmp", RenderOrder::Play);
		Ptr->SetRenderScale({ 150, 150 });
		Ptr->SetTexture("HPBar.bmp");
	}

	GameEngineCollision* BodyCollsion = CreateCollision(CollisionOrder::MonsterBody);
	BodyCollsion->SetCollisionScale({ 100, 100 });
	BodyCollsion->SetCollisionType(CollisionType::CirCle);
}