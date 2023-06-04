#pragma once
#include <string>
// c++ 실행되는 곳에서 사용할 수 있는 모든 문자열 관련 처리방식을 지원해준다.
#include <filesystem>

// 설명 : 경로에 관련된 기능을 대표하는 킆래스
//class GameEngineFile;
//class GameEngineDirectory;

class GameEnginePath
{
	friend class GameEngineFile;
	friend class GameEngineDirectory;

public:
	// constrcuter destructer
	GameEnginePath();
	GameEnginePath(std::filesystem::path _Path);
	GameEnginePath(const std::string& _path);
	~GameEnginePath();

	// delete Function
	GameEnginePath(const GameEnginePath& _Other) = delete;
	GameEnginePath(GameEnginePath&& _Other) noexcept = delete;
	GameEnginePath& operator=(const GameEnginePath& _Other) = delete;
	GameEnginePath& operator=(GameEnginePath&& _Other) noexcept = delete;

	std::string GetFileName() const;
	std::string GetPathToString() const;

	void MoveParent();

	// 내자식중 특정 경로나 특정 파일이 있는곳까지 자동 move
	void MoveParentToChildPath(const std::string_view& _String);

	bool IsExists();
	bool IsExistsToPlusString(const std::string_view& _String);

	bool IsRoot();

	bool Move(const std::string_view& _Path);

	void SetPath(const std::string_view& _Path);

protected:


private:
	std::filesystem::path Path;
	// std::string Path;
};

