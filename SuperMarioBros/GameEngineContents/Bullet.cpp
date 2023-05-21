#include "Bullet.h"
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineCore.h>

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::Start()
{
	Renderer = CreateRenderer();
}

void Bullet::Update(float _Delta)
{
	
	// Bullet¿⁄√º∞° 
	AddPos(Dir * _Delta * Speed);

	if (1.0f < GetLiveTime())
	{
		if (nullptr != Renderer)
		{
			// Death();
			Renderer->Death();
			Renderer = nullptr;
		}
	}

}

void Bullet::Render()
{

}
