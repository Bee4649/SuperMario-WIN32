#pragma once
#include <string_view>
#include <vector>
#include "GameEngineFile.h"
#include "GameEnginePath.h"

// ���� :
class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	// constrcuter destructer
	GameEngineDirectory();
	GameEngineDirectory(const std::string& _path);
	~GameEngineDirectory();

	// delete Function
	GameEngineDirectory(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory(GameEngineDirectory&& _Other) noexcept = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory& operator=(GameEngineDirectory&& _Other) noexcept = delete;

	// �� ���丮�� �� ������ �ִ�ġ ��ũ!
	bool IsFile(const std::string_view& _FileName);

	bool MoveParent();

	void MoveParentToDirectory(const std::string_view& _String);

	bool Move(const std::string_view& _String);

	GameEnginePath GetPlusFileName(const std::string_view& _String);

	// ������ ���� �ʰ� �� ���丮�� �ش��ϴ� ��� ���ϸ� �������ִ� �Լ�.
	std::vector<GameEngineFile> GetAllFile(const std::string_view& _Ext = "");

protected:

private:
	// "D:\Project\AR47\WINAPI\APIApp\ContentsResources\Iamge\";
	GameEnginePath Path;
};

