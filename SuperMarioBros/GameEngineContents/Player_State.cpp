#include "Player.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

void Player::IdleStart()
{
	ChangeAnimationState("Idle");

	// MainRenderer->ChangeAnimation("Right_Idle");
}

void Player::RunStart()
{
	ChangeAnimationState("Run");
	// MainRenderer->ChangeAnimation("Right_Run");
}

void Player::IdleUpdate(float _Delta)
{
	if (true == GameEngineInput::IsDown('A')
		|| true == GameEngineInput::IsDown('W')
		|| true == GameEngineInput::IsDown('S')
		|| true == GameEngineInput::IsDown('D'))
	{
		DirCheck();
		ChanageState(PlayerState::Run);
		return;
	}

	// 줄줄이 사탕으로 
	//if (true)
	//{
	//	ChanageState(PlayerState::Idle);
	//}

}


void Player::RunUpdate(float _Delta)
{
	DirCheck();

	float Speed = 1000.0f;

	float4 MovePos = float4::ZERO;

	if (true == GameEngineInput::IsPress('A') && Dir == PlayerDir::Left)
	{
		MovePos = { -Speed * _Delta, 0.0f };
	} else if (true == GameEngineInput::IsPress('D') && Dir == PlayerDir::Right)
	{
		MovePos = { Speed * _Delta, 0.0f };
	}

	if (true == GameEngineInput::IsPress('W'))
	{
		MovePos = { 0.0f, -Speed * _Delta };
	}
	if (true == GameEngineInput::IsPress('S'))
	{
		MovePos = { 0.0f, Speed * _Delta };
	}

	if (MovePos == float4::ZERO)
	{
		DirCheck();
		ChanageState(PlayerState::Idle);
	}

	AddPos(MovePos);
	GetLevel()->GetMainCamera()->AddPos(MovePos);


}