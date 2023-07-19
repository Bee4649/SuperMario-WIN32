#include "Coin.h"

Coin::Coin() {

}

Coin::~Coin() {

}

void Coin::Start()
{
	ItemActor::Start();
	ThisItemType = ItemType::Coin;
	ItemRender = CreateRender(RenderOrder::Item);
	ItemRender->SetScale({ 64, 64 });
	ItemRender->SetPosition({ 0, -32 });
	ItemRender->CreateAnimation({ .AnimationName = "Defalut", .ImageName = "Coin.BMP", .Start = 0, .End = 3, .InterTime = 0.2f });
	ItemRender->ChangeAnimation("Defalut");
}

void Coin::Update(float _DeltaTime)
{
	ItemActor::Update(_DeltaTime);
}
