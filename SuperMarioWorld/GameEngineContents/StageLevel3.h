#pragma once
#include "StageLevel.h"

class StageLevel3 : public StageLevel
{
public:
	StageLevel3();
	~StageLevel3();

	StageLevel3(const StageLevel3& _Other) = delete;
	StageLevel3(StageLevel3&& _Other) noexcept = delete;
	StageLevel3& operator=(const StageLevel3& _Other) = delete;
	StageLevel3& operator=(StageLevel3&& _Other) noexcept = delete;

protected:
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
private:
	const std::string_view BackGroundName = "BACKGROUND1.BMP";
	const std::string_view StageName = "STAGE3.BMP";
	const std::string_view StageColName = "STAGE3COL.BMP";
};