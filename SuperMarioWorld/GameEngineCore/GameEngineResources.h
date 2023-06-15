#pragma once

#include <map>
#include <string>
#include <GameEnginePlatform/GameEngineWindowTexture.h>
#include <GameEnginePlatform/GameEngineSound.h>


// 설명 : 선생님이 마지막으로 알려주는 싱글톤
// 싱글톤이 뭐지?
// 싱글톤은 갱스오브포라는 1990대 프로그래머 집단이
// 대다수의 프로그래머가 비슷한 규격의 코딩을 하는것을 정리하고 이름을 붙인것을 
// 디자인 패턴이라고 하는데 그중에서 하나의 패턴명
// 보통 UML과 함께 배우는것이 좋다.
// 생성 구분에 해당
// 생성 객체를 생성하는 방법을 
// 패턴 소양 정도로만 알고있으면 됩니다.
// 이 프로그램에서 이 객체가 1개만 있었으면 좋겠다.


// 이미지랑 사운드는 전부다 이녀석이 관리하게 만듬.

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

// 설명 :
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

