#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor() 
{
}

GameEngineActor::~GameEngineActor() 
{
	for (GameEngineRenderer* Render : RenderList)
	{
		if (nullptr == Render)
		{
			continue;
		}

		delete Render;
		Render = nullptr;
	}

	for (GameEngineCollision* Collision : CollisionList)
	{
		if (nullptr == Collision)
		{
			continue;
		}

		delete Collision;
		Collision = nullptr;
	}

}
GameEngineLevel* GameEngineActor::GetLevel()
{
	return GetOwner<GameEngineLevel>();
}

GameEngineRenderer* GameEngineActor::CreateRender(const std::string_view& _Image, int _Order /*= 0*/)
{
	GameEngineRenderer* Render = CreateRender(_Order);
	Render->SetImage(_Image);
	return Render;
}

GameEngineRenderer* GameEngineActor::CreateRender(int _Order /*= 0*/)
{
	GameEngineRenderer* Render = new GameEngineRenderer();
	Render->SetOwner(this);
	Render->SetOrder(_Order);
	RenderList.push_back(Render);
	return Render;
}

GameEngineCollision* GameEngineActor::CreateCollision(int _GroupIndex)
{
	GameEngineCollision* Collision = new GameEngineCollision();
	Collision->SetOwner(this);
	Collision->SetOrder(_GroupIndex);
	CollisionList.push_back(Collision);
	return Collision;
}


void GameEngineActor::Release()
{
	{
		std::list<GameEngineRenderer*>::iterator StartIter = RenderList.begin();
		std::list<GameEngineRenderer*>::iterator EndIter = RenderList.end();

		for (; StartIter != EndIter; )
		{
			GameEngineRenderer* ReleaseRender = *StartIter;

			if (nullptr == ReleaseRender)
			{
				MsgAssert("nullptr 인 Render가 내부에 들어있습니다.");
			}

			if (false == ReleaseRender->IsDeath())
			{
				++StartIter;
				continue;
			}

			StartIter = RenderList.erase(StartIter);

			delete ReleaseRender;
			ReleaseRender = nullptr;
		}
	}
	{
		std::list<GameEngineCollision*>::iterator StartIter = CollisionList.begin();
		std::list<GameEngineCollision*>::iterator EndIter = CollisionList.end();

		for (; StartIter != EndIter; )
		{
			GameEngineCollision* ReleaseCollision = *StartIter;

			if (nullptr == ReleaseCollision)
			{
				MsgAssert("nullptr 인 Render가 내부에 들어있습니다.");
			}

			if (false == ReleaseCollision->IsDeath())
			{
				++StartIter;
				continue;
			}

			StartIter = CollisionList.erase(StartIter);

			delete ReleaseCollision;
			ReleaseCollision = nullptr;
		}
	}
}

void GameEngineActor::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{

}

void GameEngineActor::LevelChangeStart(GameEngineLevel* _PrevLevel)
{

}