#pragma once
#include "StageLevel.h"

class StageLevel1 : public StageLevel
{
public:
	StageLevel1();
	~StageLevel1();

	StageLevel1(const StageLevel1& _Other) = delete;
	StageLevel1(StageLevel1&& _Other) noexcept = delete;
	StageLevel1& operator=(const StageLevel1& _Other) = delete;
	StageLevel1& operator=(StageLevel1&& _Other) noexcept = delete;

protected:
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
private:
	const std::string_view BackGroundName = "BACKGROUND1.BMP";
	const std::string_view StageName = "STAGE1.BMP";
	const std::string_view StageColName = "STAGE1COL.BMP";
};

