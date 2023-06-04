#pragma once
#include <string>
// c++ ����Ǵ� ������ ����� �� �ִ� ��� ���ڿ� ���� ó������� �������ش�.
#include <filesystem>

// ���� : ��ο� ���õ� ����� ��ǥ�ϴ� ������
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

	// ���ڽ��� Ư�� ��γ� Ư�� ������ �ִ°����� �ڵ� move
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

