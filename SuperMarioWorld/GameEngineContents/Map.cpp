#include "Map.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include "ContentsEnum.h"
#include "Mario.h"
#include "ContentCore.h"

Map* Map::MainMap = nullptr;

Map::Map()
{
}

Map::~Map()
{
}


void Map::SetImage(const std::string_view& _BackGroundName, const std::string_view& _StageName, const std::string_view& _StageColName)
{
	BackGroundName = _BackGroundName;
	StageName = _StageName;
	StageColName = _StageColName;

	if ("" != StageName)
	{
		StageRender = CreateRender(StageName, RenderOrder::Map);
		StageSize = StageRender->GetImage()->GetImageScale();
		StageRender->SetScale(StageSize);
		StageRender->SetPosition(StageSize.half());
		StageRender->Off();
	}

	if ("" != BackGroundName)
	{
		BackGroundRender = CreateRender(BackGroundName, RenderOrder::BackGround);
		if (true == IsBackAnim)
		{
			BackGroundSize = BackGroundRender->GetImage()->GetImageScale();
			BackGroundSize.X /= 4;
		}
		else
		{
			BackGroundSize = BackGroundRender->GetImage()->GetImageScale();
		}
		BackGroundRender->SetScale(BackGroundSize);
		BackGroundRender->SetPosition(BackGroundSize.half());
		BackGroundRender->Off();

		int BackGroundNum = StageSize.ix() / BackGroundSize.ix();
		for (int i = 1; i <= BackGroundNum; i++)
		{
			GameEngineRenderer* Render = CreateRender(BackGroundName, RenderOrder::BackGround);
			if (true == IsBackAnim)
			{
				Render->CreateAnimation({ .AnimationName = "BackGroundAnim", .ImageName = BackGroundName, .Start = 0, .End = 3 });
				Render->ChangeAnimation("BackGroundAnim");
			}
			Render->SetScale(BackGroundSize);
			Render->SetPosition(BackGroundSize.half());
			Render->SetMove({ BackGroundSize.X * i, 0 });
		}
	}
}

void Map::SetDebugMap(const std::string_view& _DebugMapName)
{
	DebugRender = CreateRender(_DebugMapName, RenderOrder::Map);
	DebugRender->SetScale(StageSize);
	DebugRender->SetPosition(StageSize.half());
	DebugRender->Off();
}


void Map::SetStartPos(const std::vector<float4>& _StartPos)
{
}


void Map::MoveMap(int _StartPosIndex)
{
	if (StartPos.size() < _StartPosIndex)
	{
		MsgAssert("시작 위치 Vector의 범위를 벗어났습니다")
	}
	Mario::MainPlayer->SetPos(StartPos[_StartPosIndex]);

	if (nullptr != MainMap)
	{
		if (this == MainMap)
		{
			MsgAssert("현재 사용중인 맵을 다시 사용하려 했습니다");
		}
	
	}
	MainMap = this;
	if (nullptr != BackGroundRender)
	{
		BackGroundRender->On();
	}
	if (nullptr != StageRender)
	{
		StageRender->On();
	}
	
}


void Map::Update(float _DeltaTime)
{
	if (nullptr == DebugRender)
	{
		return;
	}
	if (true == ContentCore::GetInst().GetCollisionDebug())
	{
		DebugRender->On();
	}
	else
	{
		DebugRender->Off();
	}
}
