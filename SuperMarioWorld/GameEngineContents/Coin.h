#pragma once
#include "ItemActor.h"
class Coin : public ItemActor
{
public:
	Coin();
	~Coin();
	void OffCamera() override {};
	void OnCamera() override {};
	Coin(const Coin& _Other) = delete;
	Coin(Coin&& _Other) noexcept = delete;
	Coin& operator=(const Coin& _Other) = delete;
	Coin& operator=(Coin&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

