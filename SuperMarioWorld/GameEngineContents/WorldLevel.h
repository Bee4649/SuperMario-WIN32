#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineResources.h>

// 월드맵이 구현 될때 레벨
// 이 레벨이 구현될때 ContentCore로 부터 스테이지 클리어 정보를
//  받아와서 입장할 수 있는 스테이지를 구현한다.
class WorldMario;
class WorldImage;
class WorldLevel : public GameEngineLevel
{
public:

	static WorldLevel* GetInstance()
	{
		return Instance;
	}

	void StageClear(const std::string_view& _StageName);

	WorldLevel();
	~WorldLevel();

	WorldLevel(const WorldLevel& _Other) = delete;
	WorldLevel(WorldLevel&& _Other) noexcept = delete;
	WorldLevel& operator=(const WorldLevel& _Other) = delete;
	WorldLevel& operator=(WorldLevel&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;

private:
	static WorldLevel* Instance;
	WorldMario* WorldData = nullptr;
	WorldImage* ImageActor = nullptr;
	GameEngineSoundPlayer BGMPlayer;
};


