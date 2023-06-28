#pragma once
#include "StageLevel.h"

class StageLevel4 : public StageLevel
{
public:
	StageLevel4();
	~StageLevel4();

	StageLevel4(const StageLevel4& _Other) = delete;
	StageLevel4(StageLevel4&& _Other) noexcept = delete;
	StageLevel4& operator=(const StageLevel4& _Other) = delete;
	StageLevel4& operator=(StageLevel4&& _Other) noexcept = delete;

protected:
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
private:
	const std::string_view BackGroundName = "BACKGROUND4.BMP";
	const std::string_view StageName = "STAGE4.BMP";
	const std::string_view StageColName = "STAGE4COL.BMP";
};

