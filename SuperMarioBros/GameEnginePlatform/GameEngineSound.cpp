#include "GameEngineSound.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

// lib를 가져다 사용하겠다는 전처리문입니다.
// lib는 구현이다.
#ifdef _DEBUG
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmodL_vc.lib")
#else
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmod_vc.lib")
#endif

////////////////////////////////////////////////// SoundPlayer
/*
void GameEngineSoundPlayer::SetVolume(float _Volume)
{
	Control->setVolume(_Volume * GameEngineSound::GlobalVolume);
}

void GameEngineSoundPlayer::Stop()
{
	Control->stop();
}
*/

//////////////////////////////////////////////// 관리를 위한 코드 

// FMOD를 사용하기 위한 핸들 및 객체
FMOD::System* SoundSystem = nullptr;

class SoundSystemCreator
{
public:
	SoundSystemCreator()
	{
		// 이런 외부 함수는 내부에서 new를 하고 있습니다.
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MsgAssert("사운드 시스템 생성에 실패했습니다.");
		}

		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgAssert("사운드 시스템 이니셜라이즈에 실패했습니다.");
		}
	}

	~SoundSystemCreator()
	{

		SoundSystem->release();
	}
};


void GameEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("Sound System이 생성되지 않아서 사운드 업데이트를 돌릴수가 없습니다.");
	}

	SoundSystem->update();
}



SoundSystemCreator SoundInitObject = SoundSystemCreator();
// float GameEngineSound::GlobalVolume = 1.0f;

// std::map<std::string, GameEngineSound*> GameEngineSound::AllSound;


GameEngineSound::GameEngineSound() 
{
}

GameEngineSound::~GameEngineSound() 
{

}

//
//void GameEngineSound::Init()
//{
//	static bool IsOnce = false;
//
//	if (true == IsOnce)
//	{
//		return;
//	}
//
//
//
//
//	// Fmod를 사용하기 위한 준비를 하는 함수
//	IsOnce = true;
//}

/*
GameEngineSound* GameEngineSound::FindSound(const std::string& _Path)
{
	std::string UpperName = GameEngineString::AnsiToUTF8(_Path);

	std::map<std::string, GameEngineSound*>::iterator FindIter = AllSound.find(UpperName);

	if (FindIter == AllSound.end())
	{
		return nullptr;
	}
	
	return FindIter->second;
}
*/
void GameEngineSound::Load(const std::string_view& _Path)
{
	std::string UTF8Path = GameEngineString::AnsiToUTF8(_Path);

	if (FMOD_OK != SoundSystem->createSound(UTF8Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &FMODSound))
	{
		MsgAssert(std::string("사운드 로드에 실패했습니다.\n 경로 : ") + _Path.data());
	}

	return;
}
/*
GameEngineSoundPlayer GameEngineSound::SoundPlay(const std::string& _Name)
{
	GameEngineSound* FindSoundPtr = FindSound(_Name);

	if (nullptr == FindSoundPtr)
	{
		MsgAssert("존재하지 않는 사운드를 재생하려고 했습니다.");
		return nullptr;
	}

	GameEngineSoundPlayer Player = FindSoundPtr->Play();

	Player.SetVolume(1.0f);

	return Player;
}
*/

/*
void GameEngineSound::Release()
{
	for (std::pair<std::string, GameEngineSound*> Pair  : GameEngineSound::AllSound)
	{
		if (nullptr == Pair.second)
		{
			return;
		}

		delete Pair.second;
	}
}
*/


/////////////////////////////// 맴버


FMOD::Channel* GameEngineSound::Play()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound가 nullptr 입니다 "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;

	FMOD::Channel* SoundControl = nullptr;

 	SoundSystem->playSound(FMODSound, nullptr, false, &SoundControl);

	return SoundControl;
}