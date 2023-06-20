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


// FMOD를 사용하기 위한 핸들 및 객체
FMOD::System* SoundSystem = nullptr;

class SoundSystemCreator
{
public:
	SoundSystemCreator()
	{
		FMOD::System_Create(&SoundSystem);

		if (nullptr == SoundSystem)
		{
			MsgAssert("사운드 초기화가 제대로 되지 않았습니다.");
		}

		if (FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgAssert("사운드 시스템 이니셜라이즈에 실패했습니다.");
		}

	}

	~SoundSystemCreator()
	{


		SoundSystem->release();
	}
};

SoundSystemCreator SoundInitObject = SoundSystemCreator();

void GameEngineSound::SoundUpdate()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("사운드 업데이트가 불가능합니다.");
		return;
	}

	SoundSystem->update();
}

GameEngineSound::GameEngineSound()
{
}

GameEngineSound::~GameEngineSound()
{
	
}


void GameEngineSound::SoundLoad(const std::string_view& _Path)
{
	std::string UTF8Path = GameEngineString::AnsiToUTF8(_Path);

	if (FMOD_OK != SoundSystem->createSound(UTF8Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &FMODSound))
	{
		MsgAssert(std::string("사운드 로드에 실패했습니다.\n 경로 : ") + _Path.data());
	}

	return;
}

FMOD::Channel* GameEngineSound::Play()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound가 nullptr 입니다 "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;
}