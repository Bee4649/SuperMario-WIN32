#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PlayUIManager : public GameEngineActor
{
public:
	static PlayUIManager* UI;

public:
	// constrcuter destructer
	PlayUIManager();
	~PlayUIManager();

	// delete Function
	PlayUIManager(const PlayUIManager& _Other) = delete;
	PlayUIManager(PlayUIManager&& _Other) noexcept = delete;
	PlayUIManager& operator=(const PlayUIManager& _Other) = delete;
	PlayUIManager& operator=(PlayUIManager&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

