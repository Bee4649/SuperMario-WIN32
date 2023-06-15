#include "GameEngineCollision.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineWindowTexture.h>

static bool(*ColFunctionPtr[Max][Max])(const CollisionData& _Left, const CollisionData& _Right);


class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		ColFunctionPtr[CirCle][CirCle] = GameEngineCollision::CollisionCirCleToCirCle;
		ColFunctionPtr[CirCle][Point] = GameEngineCollision::CollisionCirCleToPoint;
		ColFunctionPtr[Rect][Rect] = GameEngineCollision::CollisionRectToRect;
		ColFunctionPtr[Rect][Point] = GameEngineCollision::CollisionRectToPoint;
	}
	~CollisionFunctionInit()
	{

	}
};

CollisionFunctionInit Init = CollisionFunctionInit();

GameEngineCollision::GameEngineCollision()
{
}

GameEngineCollision::~GameEngineCollision()
{
}


bool GameEngineCollision::CollisionCirCleToPoint(const CollisionData& _Left, const CollisionData& _Right)
{
	float4 Len = _Left.Position - _Right.Position;
	float Size = Len.Size();
	float RadiusSum = _Left.Scale.hX();
	return RadiusSum > Size;
}

bool GameEngineCollision::CollisionCirCleToCirCle(const CollisionData& _Left, const CollisionData& _Right)
{
	float4 Len = _Left.Position - _Right.Position;
	float Size = Len.Size();
	float RadiusSum = _Left.Scale.hX() + _Right.Scale.hX();
	return RadiusSum > Size;
}

bool GameEngineCollision::CollisionRectToRect(const CollisionData& _Left, const CollisionData& _Right)
{
	if (_Left.Bot() <= _Right.Top())
	{
		return false;
	}

	if (_Left.Top() >= _Right.Bot())
	{
		return false;
	}

	if (_Left.Left() >= _Right.Right())
	{
		return false;
	}

	if (_Left.Right() <= _Right.Left())
	{
		return false;
	}

	return true;
}

bool GameEngineCollision::CollisionRectToPoint(const CollisionData& _Left, const CollisionData& _Right)
{
	if (_Left.Bot() <= _Right.Position.Y)
	{
		return false;
	}

	if (_Left.Top() >= _Right.Position.Y)
	{
		return false;
	}

	if (_Left.Left() >= _Right.Position.X)
	{
		return false;
	}

	if (_Left.Right() <= _Right.Position.X)
	{
		return false;
	}

	return true;
}

void GameEngineCollision::SetOrder(int _Order)
{
	GetActor()->GetLevel()->PushCollision(this, _Order);
}

bool GameEngineCollision::Collision(const CollisionCheckParameter& _Parameter)
{
	if (false == IsUpdate())
	{
		return false;
	}

	std::list<GameEngineCollision*>& _TargetGroup = GetActor()->GetLevel()->Collisions[_Parameter.TargetGroup];

	for (GameEngineCollision* OtherCollision : _TargetGroup)
	{
		if (OtherCollision == this)
		{
			continue;
		}

		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		CollisionType Type = _Parameter.ThisColType;
		CollisionType OtherType = _Parameter.TargetColType;

		if (nullptr == ColFunctionPtr[Type][OtherType])
		{
			MsgAssert("아직 충돌함수를 만들지 않은 충돌 타입입니다");
		}

		if (true == ColFunctionPtr[Type][OtherType](GetCollisionData(), OtherCollision->GetCollisionData()))
		{
			return true;
		}
	}
	return false;
}

bool GameEngineCollision::Collision(const CollisionCheckParameter& _Parameter, std::vector<GameEngineCollision*>& _Collision)
{
	if (false == IsUpdate())
	{
		return false;
	}

	_Collision.clear();

	std::list<GameEngineCollision*>& _TargetGroup = GetActor()->GetLevel()->Collisions[_Parameter.TargetGroup];

	SetDebugRenderType(_Parameter.ThisColType);

	for (GameEngineCollision* OtherCollision : _TargetGroup)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		CollisionType Type = _Parameter.ThisColType;
		CollisionType OtherType = _Parameter.TargetColType;

		OtherCollision->SetDebugRenderType(OtherType);

		if (nullptr == ColFunctionPtr[Type][OtherType])
		{
			MsgAssert("아직 충돌함수를 만들지 않은 충돌 타입입니다");
		}

		if (true == ColFunctionPtr[Type][OtherType](GetCollisionData(), OtherCollision->GetCollisionData()))
		{
			_Collision.push_back(OtherCollision);
		}
	}

	return _Collision.size() != 0;
}

CollisionData GameEngineCollision::GetCollisionData()
{
	return { GetActorPlusPos(), GetScale() };
}

void GameEngineCollision::DebugRender()
{
	HDC BackBufferDc = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	float4 DebugRenderPos = GetActorPlusPos() - GetActor()->GetLevel()->GetCameraPos();
	switch (DebugRenderType)
	{
	case Point:
		break;
	case CirCle:
	{
		int Radius = GetScale().ihX();
		Ellipse(BackBufferDc,
			DebugRenderPos.iX() - Radius,
			DebugRenderPos.iY() - Radius,
			DebugRenderPos.iX() + Radius,
			DebugRenderPos.iY() + Radius);
		break;
	}
	case Rect:
	{
		Rectangle(BackBufferDc,
			DebugRenderPos.iX() - GetScale().ihX(),
			DebugRenderPos.iY() - GetScale().ihY(),
			DebugRenderPos.iX() + GetScale().ihX(),
			DebugRenderPos.iY() + GetScale().ihY());
		break;
	}
	case Max:
		break;
	default:
		break;
	}
}