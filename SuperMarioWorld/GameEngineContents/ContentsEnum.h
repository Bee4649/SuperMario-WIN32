#pragma once
enum class PowerState
{
	Normal, Super, Fire, Cape
};
enum class ItemType
{
	Coin, UpMushroom, SuperMushroom, FireFlower , Feather
};
enum class RenderOrder
{
	Sky,
	BackGround,
	Map,
	Block,
	Item,
	Monster,
	Player,
	PlayerAttack,
	ForeGround,
	UI,
	Fade
};
enum class CollisionOrder
{
	Check,
	Player,
	Block,
	Item,
	Monster,
	PlayerAttack,
	MonsterAttack
};