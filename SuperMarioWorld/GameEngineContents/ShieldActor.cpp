#include "ShieldActor.h"
#include "ContentsEnums.h"
ShieldActor::ShieldActor() {

}

ShieldActor::~ShieldActor() {

}

void ShieldActor::SetOwner(GameEngineActor* _Owner)
{
	Owner = _Owner;
}

void ShieldActor::SetCollision(const float4& _Scale, const float4 _Pos)
{
	Collision = CreateCollision(CollisionOrder::Monster);
	Collision->SetScale(_Scale);
	Collision->SetPosition(_Pos);
	Collision->SetDebugRenderType(CT_Rect);
}

bool ShieldActor::IsCollisionAttack()
{
	return false;
}

bool ShieldActor::IsHold()
{
	return false;
}

bool ShieldActor::IsHoldable()
{
	return false;
}

bool ShieldActor::IsDie()
{
	return false;
}

void ShieldActor::Start()
{
}

void ShieldActor::Update(float _DeltaTime)
{
	if (nullptr == Owner)
	{
		return;
	}
	SetPos(Owner->GetPos());
}
