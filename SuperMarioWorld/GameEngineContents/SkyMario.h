//#pragma once
//
//#include <GameEngineCore/GameEngineActor.h>
//#include "ContentCore.h"
//#include "ContentsEnum.h"
//
//enum class SkyMarioState
//{
//	IDLE,
//	WALK,
//	RUN,
//	BRAKE,
//	CROUCH,
//	LOOKUP,
//	JUMP,
//	FALL,
//	RUNJUMP,
//	SPIN,
//	SLIDE,
//	KICK,
//	CHANGEPOWER,
//	Pipe,
//	VICTORY,
//	GameOver,
//	Max
//};
//
//enum class Dir
//{
//	Left,
//	Right,
//	Max
//};
//
//
//// Stage ������ �ɸ���
//// �⺻ ���� �̵�, ����, ���, ��������
//// �Ҳ� ���¿��� �Ҳɽ�� ����
//
//class StageLevel;
//class GameEngineWindowTexture;
//class EnemyActor;
//
//class SkyMario :public GameEngineActor
//{
//public:
//
//	SkyMario();
//	~SkyMario();
//
//	SkyMario(const SkyMario& _Other) = delete;
//	SkyMario(SkyMario&& _Other) noexcept = delete;
//	SkyMario& operator=(const SkyMario& _Other) = delete;
//	SkyMario& operator=(SkyMario&& _Other) noexcept = delete;
//
//	static SkyMario* SecondPlayer;
//
//
//
//	void NewItem(ItemType _Item);
//	void Die();
//	void StageClear();
//
//	bool GetIsGameOver()
//	{
//		return StateValue == SkyMarioState::GameOver || StateValue == SkyMarioState::VICTORY;
//	}
//	PowerState GetPowerState()
//	{
//		return MarioPower;
//	}
//
//protected:
//	void Start() override;
//	void Update(float _DeltaTime) override;
//	void Render(float _DeltaTime) override;
//	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
//	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;
//
//private:
//
//	SkyMarioState StateValue = SkyMarioState::IDLE;
//	SkyMarioState BeforeState = SkyMarioState::IDLE;
//	Dir DirValue = Dir::Right;
//	Dir SlopeDir = Dir::Right;
//
//	GameEngineRenderer* AnimationRender = nullptr;
//	std::string_view BeforeAnim = "";
//	GameEngineWindowTexture* ColMap = nullptr;
//	GameEngineCollision* Collision = nullptr;
//	GameEngineCollision* FootCollision = nullptr;
//	StageLevel* CurrentLevel = nullptr;
//	EnemyActor* HoldActor = nullptr;
//
//
//	float Speed = 705;						// �̵��ӵ�
//	float RunSpeed = 900.0f;				// �޸��� �ӵ�
//	const float ClearWalkSpeed = 45;		// �޸��� �ӵ�
//	const float JumpForce = 950;			// ������
//	const float DashJumpForce = 1100;		// ��� �� ������
//	const float RunJumpForce = 1200;		// �޸��� ������
//	const float JumpPressForce = 3350;		// ���� ������
//	const float SpinPressForce = 2750;		// ���� ���� ������
//	const float SlopeForce = 1000;			// ��Ż�濡�� �������� ��
//	const float JumpTime = 0.325f;			// ���� ���� ���ӽð�
//	const float GravityMax = 1275;			// �ִ� �߷�
//	const float GravityAcceleration = 4600;	// �߷� ���ӵ�
//	const float BrakingForce = 5;			// �극��ũ ���½� ������
//	const float StoppingForce = 1;			// ������ ������
//	const float Acceleration = 1.5;			// �Ȱ� ������ ���ӷ�
//	const float DashAcceleration = 2;		// ��� �� ���ӷ�
//	const float InvincibilityTime = 3;		// �ǰݽ� ���� �ð�
//	const float ChangePowerTime = 0.9f;		// �Ŀ� ���� �ִϸ��̼� ���ӽð�
//	const float GameOverTime = 2.0f;		// ���ӿ����� �ִϸ��̼� ���ӽð�
//	const float FireAnimTime = 0.2f;		// �� �� �ִϸ��̼� ���ӽð�
//	const float KickAnimTime = 0.25f;		// �� ���� �Ҷ� �ִϸ��̼� ���ӽð�
//	const float PipeOutTime = 1.0f;
//	const float ParticleDelay = 0.1f;		// ��ƼŬ ����� ������
//	const float HeadingReaction = 150.0f;	// ����� ���ۿ� ��
//
//
//	const float4 FireCreatePos = { 30, 40 };		// �Ҳ� ������ ��ġ (������ ����)
//	const float4 RenderScale = { 192,192 };			// ���� ũ��
//	const float4 RenderPos = { 0, -56 };			// ���� ��ġ
//	const float4 CollisionScale = { 52, 64 };		// �浹ü ũ��
//	const float4 CollisionPos = { 0, -32 };			// �浹ü ��ġ 
//	const float4 FootCollisionScale = { 24, 60 };	// �浹ü ũ��
//	const float4 FootCollisionPos = { 0, -30 };		// �浹ü ��ġ 
//	const float4 HoldPos = { 40, -8 };				// �浹ü ��ġ 
//
//	float4 ScreenSize = float4::ZERO;
//	float4 MapSize = float4::ZERO;
//
//	//			MarioPower
//	// Normal : �⺻������	
//	// Super  : ���۸�����	
//	// Fire   : ���̾����	 
//
//	PowerState MarioPower = PowerState::Normal;
//
//	bool IsGrounded = true;			// �������� �� ���� �ִ��� ����
//	bool IsSlope = false;			// �������� ��Ż�� ���� �ִ��� ����
//	bool IsInvincibility = false;	// ���� �ð� ����
//	bool IsOnBlock = false;			// �������� ��� ���� �ִ��� ����
//	bool IsDie = false;
//	bool IsHold = false;
//
//	float4 MoveDir = float4::ZERO;
//
//	float HorizontalForce = 0;			// �������� �������� ��
//	float JumpTimeCounter = 0;
//	float RunChargeTime = 0;			// ��ø� �� �ð��� ����ؼ� �޸��� ��ȯ�� �Ǵ��ϴ� ����
//	float Timer = 0;
//	float FireAnimTimer = 0;			// �� �� �ִϸ��̼� ���ӽð�
//	float KickAnimTimer = KickAnimTime;	// ������ �� �� �ִϸ��̼� ���ӽð�
//	float ParticleDelayTimer = 0;		// ��ƼŬ ����� ������
//
//	// ���� ���� �Լ�
//	
//	void ChangeState(SkyMarioState _State);
//	void UpdateState(float _DeltaTime);
//
//	void IdleStart();
//	void IdleUpdate(float _DeltaTime);
//	void IdleEnd();
//
//	void WalkStart();
//	void WalkUpdate(float _DeltaTime);
//	void WalkEnd();
//
//	void RunStart();
//	void RunUpdate(float _DeltaTime);
//	void RunEnd();
//
//	void BrakeStart();
//	void BrakeUpdate(float _DeltaTime);
//	void BrakeEnd();
//
//	void JumpStart();
//	void JumpUpdate(float _DeltaTime);
//	void JumpEnd();
//
//	void SpinStart();
//	void SpinUpdate(float _DeltaTime);
//	void SpinEnd();
//
//	void CrouchStart();
//	void CrouchUpdate(float _DeltaTime);
//	void CrouchEnd();
//
//	void LookUpStart();
//	void LookUpUpdate(float _DeltaTime);
//	void LookUpEnd();
//
//	void RunJumpStart();
//	void RunJumpUpdate(float _DeltaTime);
//	void RunJumpEnd();
//
//	void FallStart();
//	void FallUpdate(float _DeltaTime);
//	void FallEnd();
//
//	void SlideStart();
//	void SlideUpdate(float _DeltaTime);
//	void SlideEnd();
//
//	void ChangePowerStart(SkyMarioState _BeforeState);
//	void ChangePowerUpdate(float _DeltaTime);
//	void ChangePowerEnd();
//
//	void GameOverStart();
//	void GameOverUpdate(float _DeltaTime);
//	void GameOverEnd();
//
//	void VictoryUpdate(float _DeltaTime);
//
//	// �浹 ���� �Լ�
//
//	void CheckCollision();
//
//
//	// �ƴϸ� ���� �Լ�
//	void ChangeAnimation(const std::string_view& _AnimationName);
//
//	// �̵� ���� �Լ�
//	void MoveCalculation(float _DeltaTime);
//	void CameraMove(float _DeltaTime);
//
//	// �ǰ� ���� �Լ�
//
//	void GetDamaged();	// �ε����� �������� �޴� ��� ���� �Ǵ� �Լ�
//	// �߶� �� normal ���¿��� ������ ������ ���ӿ��� �Լ�
//
//// ǡ�� ���� �Լ�
//	void FireAttack();
//	void NewPower(PowerState _Power);
//
//	void KickAttack();
//
//	////////////////////// DebugValue
//	float4 LeftCheck = { -100.0f, -50.0f };
//	float4 RightCheck = { 100.0f, -50.0f };
//};
//
