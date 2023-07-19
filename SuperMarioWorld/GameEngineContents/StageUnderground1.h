#pragma once
#include "StageLevel.h"

class StageUnderground1 : public StageLevel
{
public:
	StageUnderground1();
	~StageUnderground1();

	StageUnderground1(const StageUnderground1& _Other) = delete;
	StageUnderground1(StageUnderground1&& _Other) noexcept = delete;
	StageUnderground1& operator=(const StageUnderground1& _Other) = delete;
	StageUnderground1& operator=(StageUnderground1&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void LevelChangeStart(GameEngineLevel* _Prev) override;
private:
	const std::string_view BackGroundName = "BACKGROUND3.BMP";
	const std::string_view StageName = "Underground1.BMP";
	const std::string_view StageColName = "Underground1COL.BMP";
};

