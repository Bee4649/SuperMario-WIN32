#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineCollision.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>


bool GameEngineLevel::IsDebugRender = false;
float4 GameEngineLevel::TextOutStart = float4::ZERO;
std::vector<std::string> GameEngineLevel::DebugTexts;

GameEngineLevel::GameEngineLevel() 
{
	
}

GameEngineLevel::~GameEngineLevel() 
{

	for (const std::pair<int, std::list<GameEngineActor*>>& Pair : Actors)
	{
		const std::list<GameEngineActor*>& ActorList = Pair.second;

		for (GameEngineActor* Actor: ActorList)
		{
			if (nullptr != Actor)
			{
				delete Actor;
				Actor = nullptr;
			}
		}
	}

	Actors.clear();
}


float4 GameEngineLevel::GetMousePos()
{
	return GameEngineWindow::GetMousePos();
}
float4 GameEngineLevel::GetMousePosToCamera()
{
	return GameEngineWindow::GetMousePos() + CameraPos;
}

void GameEngineLevel::ActorStart(GameEngineActor* _Actor, int _Order) 
{
	if (nullptr == _Actor)
	{
		MsgAssert("Nullptr 엑터를 Start하려고 했습니다.");
		return;
	}

	_Actor->SetOwner(this);
	_Actor->SetOrder(_Order);
	_Actor->Start();

}

void GameEngineLevel::ActorsUpdate(float _DeltaTime)
{
	std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = Actors.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = Actors.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		const std::list<GameEngineActor*>& ActorList = GroupStartIter->second;
		int Order = GroupStartIter->first;
		float CurTimeScale = 1.0f;
		if (TimeScales.end() != TimeScales.find(Order))
		{
			CurTimeScale = TimeScales[Order];
		}

		for (GameEngineActor* Actor : ActorList)
		{
			// Actors.erase()
			if (nullptr == Actor || false == Actor->IsUpdate())
			{
				continue;
			}

			Actor->TimeScale = CurTimeScale;
			Actor->LiveTime += _DeltaTime;
			Actor->Update(_DeltaTime * CurTimeScale);
		}
	}
}
void GameEngineLevel::ActorsRender(float _DeltaTime)
{
	std::map<int, std::list<GameEngineRenderer*>>::iterator GroupStartIter = Renders.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator GroupEndIter = Renders.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::list<GameEngineRenderer*>& RenderList = GroupStartIter->second;

		for (GameEngineRenderer* Renderer : RenderList)
		{
			// Actors.erase()
			if (nullptr == Renderer || false == Renderer->IsUpdate())
			{
				continue;
			}

			Renderer->Render(_DeltaTime * Renderer->GetActor()->TimeScale);
		}
	}

	{
		std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineActor*>& ActorList = GroupStartIter->second;

			for (GameEngineActor* Actor : ActorList)
			{
				// Actors.erase()
				if (nullptr == Actor || false == Actor->IsUpdate())
				{
					continue;
				}

				Actor->Render(_DeltaTime);
			}
		}
	}

	{ 
		// CollisionDebugRender
		// Collision 삭제
		if (true == IsDebugRender)
		{
			std::map<int, std::list<GameEngineCollision*>>::iterator GroupStartIter = Collisions.begin();
			std::map<int, std::list<GameEngineCollision*>>::iterator GroupEndIter = Collisions.end();

			for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
			{
				std::list<GameEngineCollision*>& CollisionList = GroupStartIter->second;
				std::list<GameEngineCollision*>::iterator CollisionIterStart = CollisionList.begin();
				std::list<GameEngineCollision*>::iterator CollisionIterEnd = CollisionList.end();

				for (; CollisionIterStart != CollisionIterEnd; ++CollisionIterStart)
				{
					GameEngineCollision* DebugCollision = *CollisionIterStart;
					if (nullptr == DebugCollision || false == DebugCollision->IsUpdate())
					{
						continue;
					}
					DebugCollision->DebugRender();
				}
			}
		}
	}

	{
		// Text 출력
		TextOutStart = float4::ZERO;

		for (size_t i = 0; i < DebugTexts.size(); i++)
		{
			HDC ImageDc = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();

			// TextOutStart.ix(), TextOutStart.iy(),

			RECT Rect;
			Rect.left = TextOutStart.ix();
			Rect.top = TextOutStart.iy();
			Rect.right = TextOutStart.ix() + 1000;
			Rect.bottom = TextOutStart.iy() + 1000;

			DrawTextA(ImageDc, DebugTexts[i].c_str(), static_cast<int>(DebugTexts[i].size()), &Rect, DT_LEFT);

			// TextOutA(ImageDc, TextOutStart.ix(), TextOutStart.iy(), DebugTexts[i].c_str(), static_cast<int>(DebugTexts[i].size()));
			TextOutStart.Y += 20.0f;
		}

		DebugTexts.clear();
	}
}

void GameEngineLevel::Release()
{

	{
		// Collision 삭제

		std::map<int, std::list<GameEngineCollision*>>::iterator GroupStartIter = Collisions.begin();
		std::map<int, std::list<GameEngineCollision*>>::iterator GroupEndIter = Collisions.end();

		// 눈꼽 만큼이라도 연산을 줄이려는 거죠.

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineCollision*>& CollisionList = GroupStartIter->second;

			std::list<GameEngineCollision*>::iterator CollisionIterStart = CollisionList.begin();
			std::list<GameEngineCollision*>::iterator CollisionIterEnd = CollisionList.end();

			for (; CollisionIterStart != CollisionIterEnd; )
			{
				GameEngineCollision* ReleaseCollision = *CollisionIterStart;

				// Actors.erase()
				if (nullptr != ReleaseCollision && false == ReleaseCollision->IsDeath())
				{
					++CollisionIterStart;
					continue;
				}

				CollisionIterStart = CollisionList.erase(CollisionIterStart);

			}
		}
	}

	{
		// Render 삭제

		std::map<int, std::list<GameEngineRenderer*>>::iterator GroupStartIter = Renders.begin();
		std::map<int, std::list<GameEngineRenderer*>>::iterator GroupEndIter = Renders.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineRenderer*>& RenderList = GroupStartIter->second;

			std::list<GameEngineRenderer*>::iterator RenderIterStart = RenderList.begin();
			std::list<GameEngineRenderer*>::iterator RenderIterEnd = RenderList.end();

			for (; RenderIterStart != RenderIterEnd; )
			{
				GameEngineRenderer* ReleaseRender = *RenderIterStart;

				// Actors.erase()
				if (nullptr != ReleaseRender && false == ReleaseRender->IsDeath())
				{
					++RenderIterStart;
					continue;
				}

				RenderIterStart = RenderList.erase(RenderIterStart);
			}
		}
	}

	{ 
		// Actor 삭제

		std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineActor*>& ActorList = GroupStartIter->second;

			std::list<GameEngineActor*>::iterator ActorIterStart = ActorList.begin();
			std::list<GameEngineActor*>::iterator ActorIterEnd = ActorList.end();

			for (; ActorIterStart != ActorIterEnd; )
			{
				GameEngineActor* ReleaseActor = *ActorIterStart;

				// Actors.erase()
				if (nullptr != ReleaseActor && false == ReleaseActor->IsDeath())
				{
					ReleaseActor->Release();
					++ActorIterStart;
					continue;
				}

				ActorIterStart = ActorList.erase(ActorIterStart);

				delete ReleaseActor;
				ReleaseActor = nullptr;
			}
		}
	}
}

void GameEngineLevel::ActorLevelChangeEnd(GameEngineLevel* _NextLevel)
{
	{
		std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<GameEngineActor*>& ActorList = GroupStartIter->second;

			for (GameEngineActor* Actor : ActorList)
			{
				Actor->LevelChangeEnd(_NextLevel);
			}
		}
	}
}
void GameEngineLevel::ActorLevelChangeStart(GameEngineLevel* _PrevLevel) 
{
	std::map<int, std::list<GameEngineActor*>>::iterator GroupStartIter = Actors.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator GroupEndIter = Actors.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::list<GameEngineActor*>& ActorList = GroupStartIter->second;

		for (GameEngineActor* Actor : ActorList)
		{
			Actor->LevelChangeStart(_PrevLevel);
		}
	}
}

void GameEngineLevel::PushRender(GameEngineRenderer* _Render, int _ChangeOrder)
{
	// 0 => 10
	Renders[_Render->GetOrder()].remove(_Render);

	_Render->GameEngineObject::SetOrder(_ChangeOrder);

	if (nullptr == _Render)
	{
		MsgAssert("nullptr인 랜더를 랜더링 그룹속에 넣으려고 했습니다.");
	}

	// 먼저 이미 들어가있을수도 있다.
	Renders[_Render->GetOrder()].push_back(_Render);
}

void GameEngineLevel::PushCollision(GameEngineCollision* _Collision, int _ChangeOrder)
{
	Collisions[_Collision->GetOrder()].remove(_Collision);

	_Collision->GameEngineObject::SetOrder(_ChangeOrder);

	if (nullptr == _Collision)
	{
		MsgAssert("nullptr인 충돌체를 충돌 그룹속에 넣으려고 했습니다.");
	}

	// 먼저 이미 들어가있을수도 있다.
	Collisions[_Collision->GetOrder()].push_back(_Collision);
}