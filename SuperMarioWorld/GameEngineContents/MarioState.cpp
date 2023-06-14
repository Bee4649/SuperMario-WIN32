#include "Mario.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"
#include "Particle.h"
#include "StageLevel.h"
#include "Map.h"
#include "LevelLoader.h"


void Mario::ChangeState(MarioState _State)
{
	MarioState NextState = _State;
	MarioState PrevState = StateValue;
	StateValue = _State;

	if (MarioState::CHANGEPOWER == StateValue)
	{
		ChangePowerStart(PrevState);
		return;
	}
	if (MarioState::GameOver == StateValue)
	{
		GameOverStart();
		return;
	}
	switch (StateValue)
	{
	case MarioState::IDLE:
		IdleEnd();
		break;
	case MarioState::WALK:
		WalkEnd();
		break;
	case MarioState::RUN:
		RunEnd();
		break;
	case MarioState::JUMP:
		JumpEnd();
		break;
	case MarioState::SPIN:
		SpinEnd();
		break;
	case MarioState::BRAKE:
		BrakeEnd();
		break;
	case MarioState::CROUCH:
		CrouchEnd();
		break;
	case MarioState::LOOKUP:
		LookUpEnd();
		break;
	case MarioState::RUNJUMP:
		RunJumpEnd();
		break;
	case MarioState::FALL:
		FallEnd();
		break;
	case MarioState::SLIDE:
		SlideEnd();
		break;
	case MarioState::KICK:
		break;
	case MarioState::CHANGEPOWER:
		ChangePowerEnd();
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverEnd();
		break;
	default:
		break;
	}

	switch (NextState)
	{
	case MarioState::IDLE:
		IdleStart();
		break;
	case MarioState::WALK:
		WalkStart();
		break;
	case MarioState::RUN:
		RunStart();
		break;
	case MarioState::JUMP:
		JumpStart();
		break;
	case MarioState::SPIN:
		SpinStart();
		break;
	case MarioState::BRAKE:
		BrakeStart();
		break;
	case MarioState::CROUCH:
		CrouchStart();
		break;
	case MarioState::LOOKUP:
		LookUpStart();
		break;
	case MarioState::RUNJUMP:
		RunJumpStart();
		break;
	case MarioState::FALL:
		FallStart();
		break;
	case MarioState::SLIDE:
		SlideStart();
		break;
	case MarioState::KICK:
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverStart();
		break;
	default:
		break;
	}

}

void Mario::UpdateState(float _DeltaTime)
{
	switch (StateValue)
	{
	case MarioState::IDLE:
		IdleUpdate(_DeltaTime);
		break;
	case MarioState::WALK:
		WalkUpdate(_DeltaTime);
		break;
	case MarioState::RUN:
		RunUpdate(_DeltaTime);
		break;
	case MarioState::JUMP:
		JumpUpdate(_DeltaTime);
		break;
	case MarioState::SPIN:
		SpinUpdate(_DeltaTime);
		break;
	case MarioState::BRAKE:
		BrakeUpdate(_DeltaTime);
		break;
	case MarioState::CROUCH:
		CrouchUpdate(_DeltaTime);
		break;
	case MarioState::LOOKUP:
		LookUpUpdate(_DeltaTime);
		break;
	case MarioState::RUNJUMP:
		RunJumpUpdate(_DeltaTime);
		break;
	case MarioState::FALL:
		FallUpdate(_DeltaTime);
		break;
	case MarioState::SLIDE:
		SlideUpdate(_DeltaTime);
		break;
	case MarioState::KICK:
		break;
	case MarioState::CHANGEPOWER:
		ChangePowerUpdate(_DeltaTime);
		break;
	case MarioState::VICTORY:
		break;
	case MarioState::GameOver:
		GameOverUpdate(_DeltaTime);
		break;
	case MarioState::Pipe:
		PipeUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Mario::IdleStart()
{
	HorizontalForce = 0;
}

void Mario::IdleUpdate(float _DeltaTime)
{
	// 대시 키를 입력한 프레임
	if (PowerState::Fire == MarioPower && GameEngineInput::IsDown("Dash"))
	{
		ChangeAnimation("JumpAttack");
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	else if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 점프 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	// 아래 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Down"))
	{
		// 앉은 상태로 전환
		ChangeState(MarioState::CROUCH);
		return;
	}
	// 양쪽 방향키를 동시에 입력하거나 둘다 입력하지 않은 경우
	else if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (false == GameEngineInput::IsPress("Left") && false == GameEngineInput::IsPress("Right")))
	{
		// 비탈길에 있는 경우
		if (true == IsSlope)
		{
			ChangeState(MarioState::WALK);
			return;
		}
		// 위를 입력한 경우
		else if (GameEngineInput::IsPress("Up"))
		{
			ChangeState(MarioState::LOOKUP);
			return;
		}
	}
	// 왼쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Left"))
	{
		if (Black == ColMap->GetPixelColor(GetPos() + float4::LEFT * 4, Black))
		{
			return;
		}
		// 왼쪽으로 방향전환 및 걷기상태로 전환
		DirValue = Dir::Left;
		ChangeState(MarioState::WALK);
		return;
	}
	// 오른쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		if (Black == ColMap->GetPixelColor(GetPos() + float4::RIGHT * 4, Black))
		{
			return;
		}
		// 오른쪽으로 방향전환 및 걷기상태로 전환
		DirValue = Dir::Right;
		ChangeState(MarioState::WALK);
		return;
	}
	ChangeAnimation("Idle");
}

void Mario::IdleEnd()
{
}

void Mario::WalkStart()
{
}

void Mario::WalkUpdate(float _DeltaTime)
{
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	// 아래 방향기를 입력한 경우
	if (GameEngineInput::IsPress("Down"))
	{
		// 앉은 상태로 전환
		ChangeState(MarioState::CROUCH);
		return;
	}


	// 미입력 (방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 비탈길에 있는 상황이 아닌 경우
		if (false == IsSlope)
		{
			// 이전까지 오른쪽으로 이동하고 있던 경우
			if (0.1f < HorizontalForce)
			{
				// 서서히 속도를 낮춤
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
			}
			// 이전까지 왼쪽으로 이동하고 있던 경우
			else if (-0.1f > HorizontalForce)
			{
				// 서서히 속도를 낮춤
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
			}
			// 일정속도 이하 (멈춘경우)
			else
			{
				// IDLE 상태로 전환
				ChangeState(MarioState::IDLE);
				return;
			}
		}
	}
	// 왼쪽 방향키를 누른경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고있던 경우
		if (Dir::Right == DirValue)
		{
			// 방향전환
			DirValue = Dir::Left;
			// 이전까지 오른쪽으로 이동하고 있던 경우
			if (0.1 < HorizontalForce)
			{
				// 브레이크 상태로 변경
				ChangeState(MarioState::BRAKE);
				return;
			}
			RunChargeTime = 0;
		}
		// 대시 키를 누르고 있는 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시처리
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
			RunChargeTime += _DeltaTime;	// 대시를 한 시간 기록
		}
		// 대시 키를 누르지 않는 경우 (걷기)
		else
		{
			// 걷기처리

			// 경사로인 경우
			if (true == IsSlope)
			{
				// 걷기 처리
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -1.0f);
			}
			// 이전까지 대시로 이동중에는
			else if (-0.5f > HorizontalForce)
			{
				// 서서히 느리게
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷는 속도 지정
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
			RunChargeTime = 0;
		}
	}
	// 오른쪽 방향키를 누른 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽 방향을 본 경우
		if (Dir::Left == DirValue)
		{
			// 방향전환
			DirValue = Dir::Right;
			// 이전까지 왼쪽 방향으로 이동한 경우
			if (-0.1 > HorizontalForce)
			{
				// 브레이크 상태로 전환
				ChangeState(MarioState::BRAKE);
				return;
			}
			RunChargeTime = 0;
		}

		// 대시키를 누른 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시 처리
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
			RunChargeTime += _DeltaTime;	// 대시를 한 시간 기록
		}
		// 대시 키를 누르지 않은 경우 (일반 걷기 처리)
		else
		{
			// 경사로인 경우
			if (true == IsSlope)
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 1.0f);
			}
			// 이전까지 대시를 한 경우
			else if (0.5f < HorizontalForce)
			{
				// 서서히 속도를 줄임
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
			RunChargeTime = 0;
		}
	}

	if (true == IsSlope)
	{
		// 왼쪽으로 미끄러지는 경우
		if (Dir::Left == SlopeDir)
		{
			// 오른쪽으로 이동중인 경우
			if (0 < HorizontalForce)
			{
				if (GameEngineInput::IsPress("Dash"))
				{
					HorizontalForce = std::min<float>(HorizontalForce - _DeltaTime, 0.8f);
				}
				else
				{
					HorizontalForce = std::min<float>(HorizontalForce - _DeltaTime, 0.4f);
				}
			}
			// 왼쪽으로 이동중인 경우
			else
			{
				HorizontalForce = std::max<float>(HorizontalForce - _DeltaTime, -1.0f);
			}
		}
		// 오른쪽으로 미끄러지는 경우
		else
		{
			// 왼쪽으로 이동중인 경우
			if (0 > HorizontalForce)
			{
				if (GameEngineInput::IsPress("Dash"))
				{
					HorizontalForce = std::max<float>(HorizontalForce + _DeltaTime, -0.8f);
				}
				else
				{
					HorizontalForce = std::max<float>(HorizontalForce + _DeltaTime, -0.4f);
				}
			}
			// 오른쪽으로 이동중인 경우
			else
			{
				HorizontalForce = std::min<float>(HorizontalForce + _DeltaTime, 1.0f);
			}
		}
	}

	// 속도가 0.7이상인 경우
	if (0.7f < std::abs(HorizontalForce))
	{
		// 대시를 입력한 시간이 1초를 넘긴 경우
		if (1 < RunChargeTime)
		{
			// 런 상태로 전환
			ChangeState(MarioState::RUN);
			return;
		}
		ChangeAnimation("Dash");
	}
	else
	{
		ChangeAnimation("Walk");
	}

}

void Mario::WalkEnd()
{
	RunChargeTime = 0;
}

void Mario::RunStart()
{
	HorizontalForce = DirValue == Dir::Right ? 1.0f : -1.0f;
}

void Mario::RunUpdate(float _DeltaTime)
{
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::RUNJUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	// 아래 방향기를 입력한 경우
	if (GameEngineInput::IsPress("Down"))
	{
		// 앉은 상태로 전환
		ChangeState(MarioState::CROUCH);
		return;
	}
	// 방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우
	else if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 걷기상태로 전환
		ChangeState(MarioState::WALK);
		return;
	}
	// 왼쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고 있던 경우
		if (Dir::Right == DirValue)
		{
			// 왼쪽으로 방향전환 및 브레이크로 상태 전환
			DirValue = Dir::Left;
			ChangeState(MarioState::BRAKE);
			return;
		}
	}
	// 오른쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽방향을 보고 있던 경우
		if (Dir::Left == DirValue)
		{
			// 오른쪽으로 방향전환 및 브레이크로 상태전환
			DirValue = Dir::Right;
			ChangeState(MarioState::BRAKE);
			return;
		}
	}
	// 대시 키 입력을 땐 경우
	if (false == GameEngineInput::IsPress("Dash"))
	{
		// 걷기로 상태 전환
		ChangeState(MarioState::WALK);
		return;
	}

	if (true == IsSlope)
	{
		// 왼쪽으로 미끄러지는 경우
		if (Dir::Left == SlopeDir)
		{
			// 오른쪽으로 이동중인 경우
			if (0 < HorizontalForce)
			{
				HorizontalForce = 0.8f;
			}
			// 왼쪽으로 이동중인 경우
			else
			{
				HorizontalForce = -1.0f;
			}
		}
		// 오른쪽으로 미끄러지는 경우
		else
		{
			// 왼쪽으로 이동중인 경우
			if (0 > HorizontalForce)
			{
				HorizontalForce = -0.8f;
			}
			// 오른쪽으로 이동중인 경우
			else
			{
				HorizontalForce = 1.0f;
			}
		}
	}
	else
	{
		// 오른쪽으로 이동중인 경우
		if (0 < HorizontalForce)
		{
			HorizontalForce = 1.0f;
		}
		// 왼쪽으로 이동중인 경우
		else
		{
			HorizontalForce = -1.0f;
		}
	}

	ChangeAnimation("Run");

}

void Mario::RunEnd()
{
}

void Mario::BrakeStart()
{
	ParticleDelayTimer = 0;
}

void Mario::BrakeUpdate(float _DeltaTime)
{
	ParticleDelayTimer -= _DeltaTime;
	if (0 > ParticleDelayTimer)
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SKID");
		ParticleDelayTimer = ParticleDelay;
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	// 아래 방향기를 입력한 경우
	if (GameEngineInput::IsPress("Down"))
	{
		// 앉은 상태로 전환
		ChangeState(MarioState::CROUCH);
		return;
	}
	// 좌우키를 동시에 누르거나 좌우키 둘다 누르지 않은 경우
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// IDLE 상태로 전환
		ChangeState(MarioState::WALK);
		return;
	}
	// 왼쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽을 보고있는 경우
		if (Dir::Right == DirValue)
		{
			// 왼쪽으로 방향을 전환 및 걷기 상태로 전환
			DirValue = Dir::Left;
			ChangeState(MarioState::WALK);
			return;
		}
		// 관성으로 오른쪽으로 이동중인 경우
		if (0.1f < HorizontalForce)
		{
			// 속도를 점차 줄임
			HorizontalForce = std::max<float>(HorizontalForce - (BrakingForce * _DeltaTime), 0);
		}
		// 관성으로 왼쪽으로 이동중인 경우
		else if (-0.1f > HorizontalForce)
		{
			// 속도를 점차 줄임
			HorizontalForce = std::min<float>(HorizontalForce + (BrakingForce * _DeltaTime), 0);
		}
		// 현재 멈춘 경우
		else
		{
			// IDLE 상태로 전환
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// 오른쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽을 본 경우
		if (Dir::Left == DirValue)
		{
			// 오른쪽으로 방향 전환 및 걷기 상태로 전환
			DirValue = Dir::Right;
			ChangeState(MarioState::WALK);
			return;
		}
		// 관성으로 오른쪽으로 이동중인 경우
		if (0.1f < HorizontalForce)
		{
			// 속도를 줄임
			HorizontalForce = std::max<float>(HorizontalForce - (BrakingForce * _DeltaTime), 0);
		}
		// 관성으로 왼쪽으로 이동중인 경우
		else if (-0.1f > HorizontalForce)
		{
			// 속도를 줄임
			HorizontalForce = std::min<float>(HorizontalForce + (BrakingForce * _DeltaTime), 0);
		}
		// 멈춘 상태인 경우
		else
		{
			// IDLE 상태로 전환
			ChangeState(MarioState::IDLE);
			return;
		}
	}

	ChangeAnimation("Brake");
}

void Mario::BrakeEnd()
{
}

void Mario::JumpStart()
{
	IsGrounded = false;

	JumpTimeCounter = JumpTime;
	if (0.5f < std::abs(HorizontalForce))
	{
		MoveDir += float4::UP * DashJumpForce;
	}
	else
	{
		MoveDir += float4::UP * JumpForce;
	}

	GameEngineResources::GetInst().SoundPlay("jump.wav");
}

void Mario::JumpUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Jump"))
	{
		JumpTimeCounter = 0;
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsPress("Jump"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * JumpPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// 미입력 (방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 이전까지 오른쪽으로 이동하고 있던 경우
		if (0.1f < HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// 이전까지 왼쪽으로 이동하고 있던 경우
		else if (-0.1f > HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// 일정속도 이하 (멈춘경우)
		else
		{
			HorizontalForce = 0;
		}
	}
	// 왼쪽 방향키를 누른경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고있던 경우
		if (Dir::Right == DirValue)
		{
			// 방향전환
			DirValue = Dir::Left;
			ChangeAnimation("Jump");
		}
		// 대시 키를 누르고 있는 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시처리
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// 대시 키를 누르지 않는 경우 (걷기)
		else
		{
			// 걷기처리

			// 이전까지 대시로 이동중에는
			if (-0.5f > HorizontalForce)
			{
				// 서서히 느리게
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷는 속도 지정
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// 오른쪽 방향키를 누른 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽 방향을 본 경우
		if (Dir::Left == DirValue)
		{
			// 방향전환
			DirValue = Dir::Right;
			ChangeAnimation("Jump");
		}

		// 대시키를 누른 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시 처리
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// 대시 키를 누르지 않은 경우 (일반 걷기 처리)
		else
		{
			// 이전까지 대시를 한 경우
			if (0.5f < HorizontalForce)
			{
				// 서서히 속도를 줄임
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("Jump");
}

void Mario::JumpEnd()
{
}

void Mario::SpinStart()
{
	IsGrounded = false;

	MoveDir += float4::UP * JumpForce;
	JumpTimeCounter = JumpTime;
	GameEngineResources::GetInst().SoundPlay("spin.wav");
}

void Mario::SpinUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Spin"))
	{
		JumpTimeCounter = 0;
	}
	if (true == IsGrounded)
	{
		if (0 < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsPress("Spin"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * SpinPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// 미입력 (방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 이전까지 오른쪽으로 이동하고 있던 경우
		if (0.1f < HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// 이전까지 왼쪽으로 이동하고 있던 경우
		else if (-0.1f > HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// 일정속도 이하 (멈춘경우)
		else
		{
			HorizontalForce = 0;
		}
	}
	// 왼쪽 방향키를 누른경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고있던 경우
		if (Dir::Right == DirValue)
		{
			// 방향전환
			DirValue = Dir::Left;
		}
		// 대시 키를 누르고 있는 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시처리
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// 대시 키를 누르지 않는 경우 (걷기)
		else
		{
			// 걷기처리

			// 이전까지 대시로 이동중에는
			if (-0.5f > HorizontalForce)
			{
				// 서서히 느리게
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷는 속도 지정
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// 오른쪽 방향키를 누른 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽 방향을 본 경우
		if (Dir::Left == DirValue)
		{
			// 방향전환
			DirValue = Dir::Right;
		}

		// 대시키를 누른 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시 처리
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// 대시 키를 누르지 않은 경우 (일반 걷기 처리)
		else
		{
			// 이전까지 대시를 한 경우
			if (0.5f < HorizontalForce)
			{
				// 서서히 속도를 줄임
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}
	ChangeAnimation("Spin");
}

void Mario::SpinEnd()
{
}

void Mario::CrouchStart()
{

}

void Mario::CrouchUpdate(float _DeltaTime)
{

	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	if (false == GameEngineInput::IsPress("Down"))
	{
		if (0.1f < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// 경사로인 경우
	if (true == IsSlope && false == IsHold)
	{
		ChangeState(MarioState::SLIDE);
		return;
	}

	// 현재 관성으로 오른쪽으로 이동하는 경우
	if (0.1f < HorizontalForce)
	{
		// 속도를 점차 줄인다
		HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
	}
	// 현재 관성으로 왼쪽으로 이동하는 경우
	else if (-0.1f > HorizontalForce)
	{
		// 속도를 점차 줄인다
		HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
	}
	else
	{
		HorizontalForce = 0;
	}

	ChangeAnimation("Crouch");

}

void Mario::CrouchEnd()
{
}

void Mario::LookUpStart()
{

}

void Mario::LookUpUpdate(float _DeltaTime)
{
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	// 위쪽 방향키를 땐 경우
	if (false == GameEngineInput::IsPress("Up"))
	{
		ChangeState(MarioState::IDLE);
		return;
	}
	// 양쪽 방향키를 동시에 입력한 경우
	else if (GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right"))
	{
		return;
	}
	// 왼쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 왼쪽으로 방향전환 및 걷기상태로 전환
		DirValue = Dir::Left;
		ChangeState(MarioState::WALK);
		return;
	}
	// 오른쪽 방향키를 입력한 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 오른쪽으로 방향전환 및 걷기상태로 전환
		DirValue = Dir::Right;
		ChangeState(MarioState::WALK);
		return;
	}
	ChangeAnimation("LookUp");
}

void Mario::LookUpEnd()
{
}

void Mario::RunJumpStart()
{
	IsGrounded = false;
	JumpTimeCounter = JumpTime;
	MoveDir += float4::UP * RunJumpForce;
	GameEngineResources::GetInst().SoundPlay("jump.wav");
}

void Mario::RunJumpUpdate(float _DeltaTime)
{
	if (GameEngineInput::IsUp("Jump"))
	{
		JumpTimeCounter = 0;
	}
	if (true == IsGrounded)
	{
		if ((Dir::Right == DirValue && 0.5f < HorizontalForce) || (Dir::Left == DirValue && -0.5f > HorizontalForce))
		{
			ChangeState(MarioState::RUN);
			return;
		}
		else if (0 < std::abs(HorizontalForce))
		{
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsPress("Jump"))
	{
		if (0 < JumpTimeCounter)
		{
			MoveDir += float4::UP * JumpPressForce * _DeltaTime;
			JumpTimeCounter -= _DeltaTime;
		}
	}
	// 미입력 (방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 이전까지 오른쪽으로 이동하고 있던 경우
		if (0.1f < HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// 이전까지 왼쪽으로 이동하고 있던 경우
		else if (-0.1f > HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// 일정속도 이하 (멈춘경우)
		else
		{
			HorizontalForce = 0;
		}
	}
	// 왼쪽 방향키를 누른경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고있던 경우
		if (Dir::Right == DirValue)
		{
			// 방향전환
			DirValue = Dir::Left;
			ChangeAnimation("RunJump");
		}
		// 대시 키를 누르고 있는 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시처리
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// 대시 키를 누르지 않는 경우 (걷기)
		else
		{
			// 걷기처리

			// 이전까지 대시로 이동중에는
			if (-0.5f > HorizontalForce)
			{
				// 서서히 느리게
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷는 속도 지정
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// 오른쪽 방향키를 누른 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽 방향을 본 경우
		if (Dir::Left == DirValue)
		{
			// 방향전환
			DirValue = Dir::Right;
			ChangeAnimation("RunJump");
		}

		// 대시키를 누른 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시 처리
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// 대시 키를 누르지 않은 경우 (일반 걷기 처리)
		else
		{
			// 이전까지 대시를 한 경우
			if (0.5f < HorizontalForce)
			{
				// 서서히 속도를 줄임
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("RunJump");
}

void Mario::RunJumpEnd()
{
}

void Mario::FallStart()
{
}

void Mario::FallUpdate(float _DeltaTime)
{

	if (true == IsGrounded)
	{
		if (0 < std::abs(HorizontalForce))
		{
			if (-0.1f > HorizontalForce && true == GameEngineInput::IsPress("Right"))
			{
				ChangeState(MarioState::BRAKE);
				return;
			}
			if (0.1f < HorizontalForce && true == GameEngineInput::IsPress("Left"))
			{
				ChangeState(MarioState::BRAKE);
				return;
			}
			ChangeState(MarioState::WALK);
			return;
		}
		else
		{
			ChangeState(MarioState::IDLE);
			return;
		}
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsPress("Jump"))
	{
	}
	// 미입력 (방향키를 입력하지 않는경우 or 양쪽 방향키를 동시에 입력한 경우)
	if ((GameEngineInput::IsPress("Left") && GameEngineInput::IsPress("Right")) || (!GameEngineInput::IsPress("Left") && !GameEngineInput::IsPress("Right")))
	{
		// 이전까지 오른쪽으로 이동하고 있던 경우
		if (0.1f < HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
		}
		// 이전까지 왼쪽으로 이동하고 있던 경우
		else if (-0.1f > HorizontalForce)
		{
			// 서서히 속도를 낮춤
			HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
		}
		// 일정속도 이하 (멈춘경우)
		else
		{
			HorizontalForce = 0;
		}
	}
	// 왼쪽 방향키를 누른경우
	else if (GameEngineInput::IsPress("Left"))
	{
		// 이전까지 오른쪽 방향을 보고있던 경우
		if (Dir::Right == DirValue)
		{
			// 방향전환
			DirValue = Dir::Left;
			ChangeAnimation("Fall");
		}
		// 대시 키를 누르고 있는 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시처리
			HorizontalForce = std::max<float>(HorizontalForce - (DashAcceleration * _DeltaTime), -1);
		}
		// 대시 키를 누르지 않는 경우 (걷기)
		else
		{
			// 걷기처리

			// 이전까지 대시로 이동중에는
			if (-0.5f > HorizontalForce)
			{
				// 서서히 느리게
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), -0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷는 속도 지정
				HorizontalForce = std::max<float>(HorizontalForce - (Acceleration * _DeltaTime), -0.5f);
			}
		}
	}
	// 오른쪽 방향키를 누른 경우
	else if (GameEngineInput::IsPress("Right"))
	{
		// 이전까지 왼쪽 방향을 본 경우
		if (Dir::Left == DirValue)
		{
			// 방향전환
			DirValue = Dir::Right;
			ChangeAnimation("Fall");
		}

		// 대시키를 누른 경우
		if (GameEngineInput::IsPress("Dash"))
		{
			// 대시 처리
			HorizontalForce = std::min<float>(HorizontalForce + (DashAcceleration * _DeltaTime), 1);
		}
		// 대시 키를 누르지 않은 경우 (일반 걷기 처리)
		else
		{
			// 이전까지 대시를 한 경우
			if (0.5f < HorizontalForce)
			{
				// 서서히 속도를 줄임
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0.5f);
			}
			// 그외 경우 (일반적인 걷기)
			else
			{
				// 걷기 처리
				HorizontalForce = std::min<float>(HorizontalForce + (Acceleration * _DeltaTime), 0.5f);
			}
		}
	}

	ChangeAnimation("Fall");
}

void Mario::FallEnd()
{
}

void Mario::SlideStart()
{
	ParticleDelayTimer = 0;
}

void Mario::SlideUpdate(float _DeltaTime)
{
	ParticleDelayTimer -= _DeltaTime;
	if (0 > ParticleDelayTimer)
	{
		Particle::CreateParticle(GetLevel(), GetPos(), "SKID");
		ParticleDelayTimer = ParticleDelay;
	}
	// 점프 키를 입력한 경우
	if (GameEngineInput::IsDown("Jump"))
	{
		// 점프 상태로 전환
		ChangeState(MarioState::JUMP);
		return;
	}
	// 스핀 키를 입력한 경우
	if (GameEngineInput::IsDown("Spin") && false == IsHold)
	{
		// 스핀 상태로 전환
		ChangeState(MarioState::SPIN);
		return;
	}
	if (false == GameEngineInput::IsPress("Down"))
	{
		ChangeState(MarioState::WALK);
		return;
	}
	if (false == IsSlope)
	{
		if (0 != std::abs(HorizontalForce))
		{
			// 이전까지 오른쪽으로 이동하고 있던 경우
			if (0.1f < HorizontalForce)
			{
				// 서서히 속도를 낮춤
				HorizontalForce = std::max<float>(HorizontalForce - (StoppingForce * _DeltaTime), 0);
			}
			// 이전까지 왼쪽으로 이동하고 있던 경우
			else if (-0.1f > HorizontalForce)
			{
				// 서서히 속도를 낮춤
				HorizontalForce = std::min<float>(HorizontalForce + (StoppingForce * _DeltaTime), 0);
			}
			// 일정속도 이하 (멈춘경우)
			else
			{
				// IDLE 상태로 전환
				ChangeState(MarioState::CROUCH);
			}
		}
		else
		{
			ChangeState(MarioState::CROUCH);
		}
		return;
	}

	// 왼쪽으로 미끄러지는 경우
	if (Dir::Left == SlopeDir)
	{
		// 오른쪽으로 이동중인 경우
		if (0 < HorizontalForce)
		{
			HorizontalForce = HorizontalForce - DashAcceleration * _DeltaTime;
		}
		// 왼쪽으로 이동중인 경우
		else
		{
			HorizontalForce = std::max<float>(HorizontalForce - DashAcceleration * _DeltaTime, -1.2f);
		}
	}
	// 오른쪽으로 미끄러지는 경우
	else
	{
		// 왼쪽으로 이동중인 경우
		if (0 > HorizontalForce)
		{
			HorizontalForce = HorizontalForce + DashAcceleration * _DeltaTime;
		}
		// 오른쪽으로 이동중인 경우
		else
		{
			HorizontalForce = std::min<float>(HorizontalForce + DashAcceleration * _DeltaTime, 1.2f);
		}
	}

	ChangeAnimation("Slide");
}

void Mario::SlideEnd()
{
}

void Mario::ChangePowerStart(MarioState _BeforeState)
{
	CurrentLevel->LevelPause();
	BeforeState = _BeforeState;

	std::string AnimStr = DirValue == Dir::Left ? "Left_" : "Right_";
	Timer = ChangePowerTime;
	AnimationRender->On();

	if (PowerState::Super == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "Grow");
		return;
	}
	if (PowerState::Normal == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "Shrink");
	}
	if (PowerState::Fire == MarioPower)
	{
		AnimationRender->ChangeAnimation(AnimStr + "GetFire");
	}
}

void Mario::ChangePowerUpdate(float _DeltaTime)
{
	Timer -= _DeltaTime;
	if (0 >= Timer) {
		if (PowerState::Normal == MarioPower)
		{
			CurrentLevel->DropStockItem();
			Timer = InvincibilityTime;
		}
		switch (BeforeState)
		{
		case MarioState::IDLE:
			ChangeAnimation("Idle");
			break;
		case MarioState::WALK:
			ChangeAnimation("Walk");
			break;
		case MarioState::RUN:
			ChangeAnimation("RUN");
			break;
		case MarioState::BRAKE:
			ChangeAnimation("BRAKE");
			break;
		case MarioState::CROUCH:
			ChangeAnimation("CROUCH");
			break;
		case MarioState::LOOKUP:
			ChangeAnimation("LOOKUP");
			break;
		case MarioState::JUMP:
			ChangeAnimation("JUMP");
			break;
		case MarioState::FALL:
			ChangeAnimation("FALL");
			break;
		case MarioState::RUNJUMP:
			ChangeAnimation("RUNJUMP");
			break;
		case MarioState::SPIN:
			ChangeAnimation("SPIN");
			break;
		case MarioState::SLIDE:
			ChangeAnimation("SLIDE");
			break;
		case MarioState::KICK:
			ChangeAnimation("KICK");
			break;
		default:
			ChangeState(MarioState::FALL);
			return;
		}
		StateValue = BeforeState;
		ChangePowerEnd();
	}
}

void Mario::ChangePowerEnd()
{
	CurrentLevel->LevelPlay();
}

void Mario::GameOverStart()
{
	CurrentLevel->LevelPause();
	AnimationRender->ChangeAnimation("GameOver1");
	Timer = 0;
	MoveDir = float4::ZERO;
	CurrentLevel->GetBGMPlayer().Stop();
	GameEngineResources::GetInst().SoundPlay("PlayerDown.mp3");
}

void Mario::GameOverUpdate(float _DeltaTime)
{
	if (true == IsDie) { return; }
	Timer += _DeltaTime;

	if (3.0f < Timer)
	{
		CurrentLevel->MarioDie();
		IsDie = true;
	}
	else if (0.8f < Timer)
	{
		MoveDir.Y -= _DeltaTime;
	}
	else if (0.5f < Timer)
	{
		AnimationRender->ChangeAnimation("GameOver2");
		MoveDir.Y += _DeltaTime;
	}

	if (1 < MoveDir.Y)
	{
		MoveDir.Y = 1;
	}
	SetMove(float4::UP * 2000 * MoveDir.Y * _DeltaTime);
}

void Mario::GameOverEnd()
{
}

void Mario::VictoryUpdate(float _DeltaTime)
{
	if (Timer < 3)
	{
		SetMove(float4::RIGHT * ClearWalkSpeed * _DeltaTime);
	}
	else if (7.2 < Timer)
	{
		AnimationRender->ChangeAnimation(BeforeAnim.data() + std::string("Goal"));
	}
	else if (4 < Timer)
	{
		ChangeAnimation("Idle");
	}
	Timer += _DeltaTime;


	// 맵 충돌 체크

	if (nullptr == ColMap)
	{
		MsgAssert("충돌용 맵 이미지가 없습니다.");
	}

	float4 DownPos = GetPos() + float4::DOWN * 10;
	// 맵 충돌 체크용 컬러 변수
	DWORD PixelColor = ColMap->GetPixelColor(DownPos, White);
	if (Black == PixelColor)
	{
		DownPos.Y = std::round(DownPos.Y);
		// 바닥에서 제일 위로 올라간다
		while (true)
		{
			DownPos.Y -= 1;
			PixelColor = ColMap->GetPixelColor(DownPos, Black);
			if (Black != PixelColor)
			{
				SetPos(DownPos);
				MoveDir.Y = 0.0f;
				break;
			}
		}
	}
	else
	{
		MoveDir.Y += GravityAcceleration * _DeltaTime;
		if (GravityMax < MoveDir.Y)
		{
			MoveDir.Y = GravityMax;
		}
		SetMove(float4::DOWN * MoveDir.Y * _DeltaTime);
	}

}

void Mario::PipeStart()
{
}

void Mario::PipeUpdate(float _DeltaTime)
{
	Timer -= _DeltaTime;
	if (0 > Timer)
	{
		ChangeState(MarioState::IDLE);
		AnimationRender->SetOrder(static_cast<int>(RenderOrder::Player));
	}
	SetMove(MoveDir * _DeltaTime * 192);
}
