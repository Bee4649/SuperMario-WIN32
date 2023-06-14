#pragma once

#include <GameEngineCore/GameEngineActor.h>
#include "ContentCore.h"
#include "ContentsEnum.h"

enum class MarioState
{
	IDLE,
	WALK,
	RUN,
	BRAKE,
	CROUCH,
	LOOKUP,
	JUMP,
	FALL,
	RUNJUMP,
	SPIN,
	SLIDE,
	KICK,
	CHANGEPOWER,
	Pipe,
	VICTORY,
	GameOver
};

enum class Dir
{
	Left,
	Right
};


// Stage 마리오 케릭터
// 기본 조작 이동, 점프, 대시, 스핀점프
// 불꽃 상태에서 불꽃쏘기 가능

class StageLevel;
class GameEngineWindowTexture;
class EnemyActor;

class Mario:public GameEngineActor
{
public:

	Mario();
	~Mario();

	Mario(const Mario& _Other) = delete;
	Mario(Mario&& _Other) noexcept = delete;
	Mario& operator=(const Mario& _Other) = delete;
	Mario& operator=(Mario&& _Other) noexcept = delete;

	static Mario* MainPlayer;



	void NewItem(ItemType _Item);
	void Die();
	void StageClear();
	bool GetIsGameOver() 
	{
		return StateValue == MarioState::GameOver || StateValue == MarioState::VICTORY;
	}
	PowerState GetPowerState()
	{
		return MarioPower;
	}
	void DropHold();
	void PipeIn(const float4& _PipePos, const float4& _Dir);
	void PipeOut(const float4& _Dir);
	void AddScore(int _Score);


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:

	MarioState StateValue = MarioState::IDLE;
	MarioState BeforeState = MarioState::IDLE;
	Dir DirValue = Dir::Right;
	Dir SlopeDir = Dir::Right;

	GameEngineRenderer* AnimationRender = nullptr;
	std::string_view BeforeAnim = "";
	GameEngineWindowTexture* ColMap = nullptr;
	GameEngineCollision* Collision = nullptr;
	GameEngineCollision* FootCollision = nullptr;
	StageLevel* CurrentLevel = nullptr;
	EnemyActor* HoldActor = nullptr;


	float Speed = 705;						// 이동속도
	float RunSpeed = 900.0f;				// 달릴때 속도
	const float ClearWalkSpeed = 45;		// 달릴때 속도
	const float JumpForce = 950;			// 점프력
	const float DashJumpForce = 1100;		// 대시 시 점프력
	const float RunJumpForce = 1200;		// 달릴시 점프력
	const float JumpPressForce = 3350;		// 점프 유지력
	const float SpinPressForce = 2750;		// 스핀 점프 유지력
	const float SlopeForce = 1000;			// 비탈길에서 가해지는 힘
	const float JumpTime = 0.325f;			// 점프 유지 지속시간
	const float GravityMax = 1275;			// 최대 중력
	const float GravityAcceleration = 4600;	// 중력 가속도
	const float BrakingForce = 5;			// 브레이크 상태시 제동력
	const float StoppingForce = 1;			// 정지시 제동력
	const float Acceleration = 1.5;			// 걷고 있을때 가속력
	const float DashAcceleration = 2;		// 대시 시 가속력
	const float InvincibilityTime = 3;		// 피격시 무적 시간
	const float ChangePowerTime = 0.9f;		// 파워 변경 애니메이션 지속시간
	const float GameOverTime = 2.0f;		// 게임오버시 애니메이션 지속시간
	const float FireAnimTime = 0.2f;		// 불 쏠때 애니메이션 지속시간
	const float KickAnimTime = 0.25f;		// 발 차기 할때 애니메이션 지속시간
	const float PipeOutTime = 1.0f;
	const float ParticleDelay = 0.1f;		// 파티클 재생성 딜레이
	const float HeadingReaction = 150.0f;	// 헤딩시 반작용 힘


	const float4 FireCreatePos = { 30, 40 };		// 불꽃 생성시 위치 (오른쪽 기준)
	const float4 RenderScale = { 192,192 };			// 렌더 크기
	const float4 RenderPos = { 0, -56 };			// 렌더 위치
	const float4 CollisionScale = { 52, 64 };		// 충돌체 크기
	const float4 CollisionPos = { 0, -32 };			// 충돌체 위치 
	const float4 FootCollisionScale = { 24, 60 };	// 충돌체 크기
	const float4 FootCollisionPos = { 0, -30 };		// 충돌체 위치 
	const float4 HoldPos = { 40, -8 };				// 충돌체 위치 

	float4 ScreenSize = float4::ZERO;
	float4 MapSize = float4::ZERO;

	//			MarioPower
	// Normal : 기본마리오	
	// Super  : 슈퍼마리오	
	// Fire   : 파이어마리오	 

	PowerState MarioPower = PowerState::Normal;

	bool IsGrounded = true;			// 마리오가 땅 위에 있는지 여부
	bool IsSlope = false;			// 마리오가 비탈길 위에 있는지 여부
	bool IsInvincibility = false;	// 무적 시간 여부
	bool IsOnBlock = false;			// 마리오가 블록 위에 있는지 여부
	bool IsDie = false;
	bool IsHold = false;

	float4 MoveDir = float4::ZERO;		

	float HorizontalForce = 0;			// 수평으로 가해지는 힘
	float JumpTimeCounter = 0;
	float RunChargeTime = 0;			// 대시를 한 시간을 기록해서 달리기 전환을 판단하는 변수
	float Timer = 0;
	float FireAnimTimer = 0;			// 불 쏠때 애니메이션 지속시간
	float KickAnimTimer = KickAnimTime;	// 발차기 할 때 애니메이션 지속시간
	float ParticleDelayTimer = 0;		// 파티클 재생성 딜레이

	// 상태 관련 함수

	void ChangeState(MarioState _State);
	void UpdateState(float _DeltaTime);

	void IdleStart();
	void IdleUpdate(float _DeltaTime);
	void IdleEnd();

	void WalkStart();
	void WalkUpdate(float _DeltaTime);
	void WalkEnd();

	void RunStart();
	void RunUpdate(float _DeltaTime);
	void RunEnd();

	void BrakeStart();
	void BrakeUpdate(float _DeltaTime);
	void BrakeEnd();

	void JumpStart();
	void JumpUpdate(float _DeltaTime);
	void JumpEnd();

	void SpinStart();
	void SpinUpdate(float _DeltaTime);
	void SpinEnd();

	void CrouchStart();
	void CrouchUpdate(float _DeltaTime);
	void CrouchEnd();

	void LookUpStart();
	void LookUpUpdate(float _DeltaTime);
	void LookUpEnd();

	void RunJumpStart();
	void RunJumpUpdate(float _DeltaTime);
	void RunJumpEnd();

	void FallStart();
	void FallUpdate(float _DeltaTime);
	void FallEnd();

	void SlideStart();
	void SlideUpdate(float _DeltaTime);
	void SlideEnd();

	void ChangePowerStart(MarioState _BeforeState);
	void ChangePowerUpdate(float _DeltaTime);
	void ChangePowerEnd();

	void GameOverStart();
	void GameOverUpdate(float _DeltaTime);
	void GameOverEnd();

	void VictoryUpdate(float _DeltaTime);

	void PipeStart();
	void PipeUpdate(float _DeltaTime);

	// 충돌 관련 함수
	
	void CheckCollision();
	void BlockCheck();

	// 아니메 관련 함수
	void ChangeAnimation(const std::string_view& _AnimationName);

	// 이동 관련 함수
	void MoveCalculation(float _DeltaTime);
	void CameraMove(float _DeltaTime);

	// 피격 관련 함수
	
	void GetDamaged();	// 부딛혀서 데미지를 받는 경우 실행 되는 함수
						// 추락 및 normal 상태에서 데미지 받으면 게임오버 함수

	// 퐈워 관련 함수
	void FireAttack();
	void NewPower(PowerState _Power);

	void KickAttack();
};

