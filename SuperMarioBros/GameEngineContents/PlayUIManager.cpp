#include "PlayUIManager.h"

#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineRenderer.h>

PlayUIManager* PlayUIManager::UI = nullptr;

PlayUIManager::PlayUIManager() 
{
	UI = this;
}

PlayUIManager::~PlayUIManager() 
{
}

void PlayUIManager::Start() 
{
	// 위치를 옮기지 않았다.
	// GetPos();
	// 카메라가 안움직여

	{
		GameEngineRenderer* Ptr = CreateUIRenderer("HPBar.bmp", RenderOrder::Play);
		Ptr->SetRenderPos({ 100, 100 });
		Ptr->SetRenderScale({ 40, 40 });
		Ptr->SetTexture("HPBar.bmp");
	}
}