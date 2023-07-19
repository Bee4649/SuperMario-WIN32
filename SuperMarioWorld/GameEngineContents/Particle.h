#pragma once
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineActor.h>

class Particle : public GameEngineActor
{
public:
	static void CreateParticle(GameEngineLevel* _Level , const float4& _Pos, const std::string_view& _Anim);
	static void CreateParticle(GameEngineLevel* _Level , const float4& _Pos, const float4& _Scale, const std::string_view& _Anim);
	static void CreateMovingParticle(GameEngineLevel* _Level, const float4& _Pos, const float4& _Dir, const std::string_view& _Anim, bool _Gravity, bool _Brake, float LiveTime);
	Particle();
	~Particle();

	void SetAnimation(const std::string_view& _Str)
	{
		if (nullptr != AnimationRender)
		{
			AnimationRender->ChangeAnimation(_Str);
		}
	}
	void SetScale(const float4& _Scale)
	{
		AnimationRender->SetScale(_Scale);
	}

	Particle(const Particle& _Other) = delete;
	Particle(Particle&& _Other) noexcept = delete;
	Particle& operator=(const Particle& _Other) = delete;
	Particle& operator=(Particle&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	GameEngineRender* AnimationRender = nullptr;


	class MovingParticle : public GameEngineActor
	{
	public:
		MovingParticle();
		~MovingParticle();

		void SetAnimation(const std::string_view& _Str);
		void SetMoveDir(const float4& _Dir);
		void SetMoveOption(bool _IsGravity, bool _IsBrake);
		void SetTime(float _Time);

		MovingParticle(const MovingParticle& _Other) = delete;
		MovingParticle(MovingParticle&& _Other) noexcept = delete;
		MovingParticle& operator=(const MovingParticle& _Other) = delete;
		MovingParticle& operator=(MovingParticle&& _Other) noexcept = delete;
	protected:
		void Start();
		void Update(float _DeltaTime);
	private:
		GameEngineRender* AnimationRender = nullptr;
		float4 MoveDir;
		const float MaxGravity = 2500.0f;
		const float GravityForce = 3000.0f;
		const float BrakeForce = 1500;
		float Time = 1.0f;
		bool IsGravity = false;
		bool IsBrake = false;
	};
};

