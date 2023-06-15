#include "GameEngineSound.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineDebug.h>

// lib�� ������ ����ϰڴٴ� ��ó�����Դϴ�.
// lib�� �����̴�.
#ifdef _DEBUG
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmodL_vc.lib")
#else
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmod_vc.lib")
#endif


// FMOD�� ����ϱ� ���� �ڵ� �� ��ü
FMOD::System* SoundSystem = nullptr;

class SoundSystemCreator
{
public:
	SoundSystemCreator()
	{
		// �̷� �ܺ� �Լ��� ���ο��� new�� �ϰ� �ֽ��ϴ�.
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MsgAssert("���� �ý��� ������ �����߽��ϴ�.");
		}

		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgAssert("���� �ý��� �̴ϼȶ���� �����߽��ϴ�.");
		}
	}

	~SoundSystemCreator()
	{


		SoundSystem->release();
	}
};


void GameEngineSound::SoundUpdate()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("���� ������Ʈ�� �Ұ����մϴ�.");
		return;
	}

	SoundSystem->update();
}



SoundSystemCreator SoundInitObject = SoundSystemCreator();


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
		MsgAssert(std::string("���� �ε忡 �����߽��ϴ�.\n ��� : ") + _Path.data());
	}

	return;
}

FMOD::Channel* GameEngineSound::Play()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound�� nullptr �Դϴ� "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;
}