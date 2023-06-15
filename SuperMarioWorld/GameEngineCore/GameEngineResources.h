#pragma once

#include <map>
#include <string>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEnginePlatform/GameEngineSound.h>


// ���� : �������� ���������� �˷��ִ� �̱���
// �̱����� ����?
// �̱����� ������������� 1990�� ���α׷��� ������
// ��ټ��� ���α׷��Ӱ� ����� �԰��� �ڵ��� �ϴ°��� �����ϰ� �̸��� ���ΰ��� 
// ������ �����̶�� �ϴµ� ���߿��� �ϳ��� ���ϸ�
// ���� UML�� �Բ� ���°��� ����.
// ���� ���п� �ش�
// ���� ��ü�� �����ϴ� ����� 
// ���� �Ҿ� �����θ� �˰������� �˴ϴ�.
// �� ���α׷����� �� ��ü�� 1���� �־����� ���ڴ�.


// �̹����� ����� ���δ� �̳༮�� �����ϰ� ����.

class GameEngineSoundPlayer
{
public:
	GameEngineSoundPlayer() {}
	GameEngineSoundPlayer(FMOD::Channel* _Channel);

	void LoopCount(int _Count);
	void Volume(float _Volume);
	void Stop();
	void PauseOn();
	void PauseOff();

	bool GetPause();

private:
	FMOD::Channel* ControlChannel = nullptr;

};

// ���� :
class GameEnginePath;
class GameEngineWindowTexture;
class GameEngineSound;
class GameEngineResources
{
public:
	GameEngineResources(const GameEngineResources& _Other) = delete;
	GameEngineResources(GameEngineResources&& _Other) noexcept = delete;
	GameEngineResources& operator=(const GameEngineResources& _Other) = delete;
	GameEngineResources& operator=(GameEngineResources&& _Other) noexcept = delete;

	static GameEngineResources& GetInst()
	{
		return Inst;
	}

	GameEngineWindowTexture* TextureLoad(const GameEnginePath& _Path);

	GameEngineWindowTexture* TextureLoad(const std::string_view& _Path, const std::string_view& _Name);

	GameEngineWindowTexture* TextureFind(const std::string_view& _Name);

	GameEngineSound* SoundLoad(const GameEnginePath& _Path);

	GameEngineSound* SoundLoad(const std::string_view& _Path, const std::string_view& _Name);

	GameEngineSound* SoundFind(const std::string_view& _Name);

	void SoundPlay(const std::string_view& _Name);

	GameEngineSoundPlayer SoundPlayToControl(const std::string_view& _Name);

	void Release();

protected:
	

private:
	static GameEngineResources Inst;

	GameEngineResources();
	~GameEngineResources();

	//        xxxx.bmp
	std::map<std::string, GameEngineWindowTexture*> AllImage;
	std::map<std::string, GameEngineSound*> AllSound;

};

