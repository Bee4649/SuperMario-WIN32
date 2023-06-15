#include "Particle.h"
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"

Particle::Particle() 
{
}
Particle::~Particle() 
{
}

void Particle::CreateParticle(GameEngineLevel* _Level, const float4& _Pos, const std::string_view& _Anim)
{
	Particle* NewParticle = _Level->CreateActor<Particle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetAnimation(_Anim);
}

void Particle::CreateParticle(GameEngineLevel* _Level, const float4& _Pos, const float4& _Scale, const std::string_view& _Anim)
{
	Particle* NewParticle = _Level->CreateActor<Particle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetScale(_Scale);
	NewParticle->SetAnimation(_Anim);
}

void Particle::CreateMovingParticle(GameEngineLevel* _Level, const float4& _Pos, const float4& _Dir, const std::string_view& _Anim, bool _Gravity, bool _Brake, float _LiveTime)
{
	MovingParticle* NewParticle = _Level->CreateActor<MovingParticle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetMoveDir(_Dir);
	NewParticle->SetAnimation(_Anim);
	NewParticle->SetMoveOption(_Gravity, _Brake);
	NewParticle->SetTime(_LiveTime);

	float4 LeftUp = _Dir;
	LeftUp.X *= -1;

	NewParticle = _Level->CreateActor<MovingParticle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetMoveDir(LeftUp);
	NewParticle->SetAnimation(_Anim);
	NewParticle->SetMoveOption(_Gravity, _Brake);
	NewParticle->SetTime(_LiveTime);

	float4 LeftDown = LeftUp;
	LeftDown.Y = _Gravity == true ? 0 : -_Dir.Y;

	NewParticle = _Level->CreateActor<MovingParticle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetMoveDir(LeftDown);
	NewParticle->SetAnimation(_Anim);
	NewParticle->SetMoveOption(_Gravity, _Brake);
	NewParticle->SetTime(_LiveTime);

	float4 RightDown = _Dir;
	RightDown.Y = _Gravity == true ? 0 : -_Dir.Y;

	NewParticle = _Level->CreateActor<MovingParticle>(RenderOrder::Player);
	NewParticle->SetPos(_Pos);
	NewParticle->SetMoveDir(RightDown);
	NewParticle->SetAnimation(_Anim);
	NewParticle->SetMoveOption(_Gravity, _Brake);
	NewParticle->SetTime(_LiveTime);

}


void Particle::Start()
{
	AnimationRender = CreateRender(RenderOrder::Player);
	AnimationRender->CreateAnimation({ .AnimationName = "SMOKE1", .ImageName = "SMOKE.BMP", .Start = 0, .End = 4, .InterTime = 0.07f ,.Loop = false });
	AnimationRender->CreateAnimation({ .AnimationName = "SKID", .ImageName = "SKIDDUST.BMP", .Start = 0, .End = 3, .InterTime = 0.07f ,.Loop = false });
	AnimationRender->CreateAnimation({ .AnimationName = "KICK", .ImageName = "KICK.BMP", .Start = 0, .End = 2, .InterTime = 0.05f ,.Loop = false });
	AnimationRender->CreateAnimation({ .AnimationName = "Blockdebrits", .ImageName = "Blockdebrits.BMP", .Start = 0, .End = 5, .InterTime = 0.05f ,.Loop = true });

	AnimationRender->SetScale({ 64, 64 });
}

void Particle::Update(float _DeltaTime)
{
	if (1 < GetLiveTime())
	{
		Death();
	}
}

// MovingParticle

Particle::MovingParticle::MovingParticle()
{
}

Particle::MovingParticle::~MovingParticle()
{
}

void Particle::MovingParticle::SetAnimation(const std::string_view& _Str)
{

	if (nullptr != AnimationRender)
	{
		AnimationRender->ChangeAnimation(_Str);
	}
}

void Particle::MovingParticle::SetMoveDir(const float4& _Dir)
{
	MoveDir = _Dir;
}

void Particle::MovingParticle::SetMoveOption(bool _IsGravity, bool _IsBrake)
{
	IsGravity = _IsGravity;
	IsBrake = _IsBrake;
}

void Particle::MovingParticle::SetTime(float _Time)
{
	Time = _Time;
}

void Particle::MovingParticle::Start()
{
	AnimationRender = CreateRender(RenderOrder::Player);
	AnimationRender->CreateAnimation({ .AnimationName = "Blockdebrits", .ImageName = "Blockdebrits.BMP", .Start = 0, .End = 5, .InterTime = 0.05f ,.Loop = true });
	AnimationRender->CreateAnimation({ .AnimationName = "StarParticle", .ImageName = "StarParticle.BMP", .Start = 0, .End = 0, });

	AnimationRender->SetScale({ 32, 32 });
}

void Particle::MovingParticle::Update(float _DeltaTime)
{
	if (true == IsGravity)
	{
		MoveDir.Y += GravityForce * _DeltaTime;
		if (MoveDir.Y > MaxGravity)
		{
			MoveDir.Y = MaxGravity;
		}
	}

	if (true == IsBrake)
	{
		if (0 < MoveDir.X)
		{
			MoveDir.X -= BrakeForce * _DeltaTime;
		}
		else
		{
			MoveDir.X += BrakeForce * _DeltaTime;
		}


		if (0 < MoveDir.Y)
		{
			MoveDir.Y -= BrakeForce * _DeltaTime;
		}
		else
		{
			MoveDir.Y += BrakeForce * _DeltaTime;
		}

	}

	SetMove(MoveDir * _DeltaTime);

	if (Time < GetLiveTime())
	{
		Death();
	}
}