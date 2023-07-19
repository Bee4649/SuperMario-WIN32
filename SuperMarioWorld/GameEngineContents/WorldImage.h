#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/NumberRenderObject.h>

class WorldLevel;
class WorldImage : public GameEngineActor
{
	friend WorldLevel;
public:
	WorldImage();
	~WorldImage();

	WorldImage(const WorldImage& _Other) = delete;
	WorldImage(WorldImage&& _Other) noexcept = delete;
	WorldImage& operator=(const WorldImage& _Other) = delete;
	WorldImage& operator=(WorldImage&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart(GameEngineLevel* _Prev);

private:
	GameEngineRender* AnimationRender = nullptr;
	GameEngineRender* BlockRender1 = nullptr;
	GameEngineRender* BlockRender2 = nullptr;
	GameEngineRender* LockStage1 = nullptr;
	GameEngineRender* LockStage2 = nullptr;
	NumberRenderObject LifeNum;
};

