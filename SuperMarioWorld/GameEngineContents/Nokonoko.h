#pragma once
#include "EnemyActor.h"

enum class NokoState
{
	Normal,
	Overturn,
	Naked,
	Die
};

class Nokonoko : public EnemyActor
{
public:
	Nokonoko();
	~Nokonoko();


	bool IsCollisionAttack() override; // ���� ���Ͱ� ���������� ������ true�� �����ϴ� �Լ�
	bool IsHold() override; // ���� ���Ͱ� �÷��̾�� ���� ���¸� true�� �����ϴ� �Լ�
	bool IsHoldable() override; // ���� ���͸� ���� �� �ִ� ���¸� true�� �����ϴ� �Լ�
	bool IsDie() override;
	void SpinHit() override; // ���������� ������ ����Ǵ� �Լ�
	void JumpHit(bool IsLeft) override; // ������ ������ ����Ǵ� �Լ�
	void FireHit() override; // �Ҳɿ� ������ ����Ǵ� �Լ�
	void BlockHit() override; // ����� �������� �ش� ����� ��Ʈ�ϸ� ����Ǵ� �Լ�
	void MonsterHit(bool IsLeft) override; // �ٸ� ���͸� ������ �¾����� ����Ǵ� �Լ�, �� ���� �� ���ʿ� ������ true�� �޴´�
	void Hold() override; // �÷��̾�� ������ ����Ǵ� �Լ�
	void Kick(const float4& _Force) override; // �÷��̾�� �߷� ġ�̸� ����Ǵ� �Լ� (���� �� ���� ���)

	Nokonoko(const Nokonoko& _Other) = delete;
	Nokonoko(Nokonoko&& _Other) noexcept = delete;
	Nokonoko& operator=(const Nokonoko& _Other) = delete;
	Nokonoko& operator=(Nokonoko&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void Turn() override;
	void TurnLeft() override;
	void TurnRight() override;

private:
	GameEngineRenderer* AnimationRender = nullptr;
	NokoState StateValue = NokoState::Normal;
	const float4 RenderScale = { 64, 108 };
	const float4 RenderPos = { 0, -50 };
	const float4 CollisionScale = { 48, 84 };
	const float4 CollisionPos = { 0, -42 };
	const float4 MonsterHitForce = { 250, -750 };
	const float HeadingReaction = 100;
	const float ActorSpeed = 80;
	const float ActorSlopeSpeed = ActorSpeed * 0.6f;
	const int Score = 100;
	void OverturnUpdate(float _DeltaTime);
};