#pragma once
#include "GameEnginePath.h"

// Ό³Έν :
class GameEngineFile
{
public:
	// constrcuter destructer
	GameEngineFile();
	GameEngineFile(std::filesystem::path _Path);
	~GameEngineFile();

	//// delete Function
	//GameEngineFile(const GameEngineFile& _Other) = delete;
	//GameEngineFile(GameEngineFile&& _Other) noexcept = delete;
	//GameEngineFile& operator=(const GameEngineFile& _Other) = delete;
	//GameEngineFile& operator=(GameEngineFile&& _Other) noexcept = delete;

	std::string GetFullPath()
	{
		return Path.Path.string();
	}

protected:

private:
	GameEnginePath Path;
};

