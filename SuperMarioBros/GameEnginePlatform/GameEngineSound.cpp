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

//////////////////////////////////////////////// ������ ���� �ڵ� 

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


void GameEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("Sound System�� �������� �ʾƼ� ���� ������Ʈ�� �������� �����ϴ�.");
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
//	// Fmod�� ����ϱ� ���� �غ� �ϴ� �Լ�
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
		MsgAssert(std::string("���� �ε忡 �����߽��ϴ�.\n ��� : ") + _Path.data());
	}

	return;
}
/*
GameEngineSoundPlayer GameEngineSound::SoundPlay(const std::string& _Name)
{
	GameEngineSound* FindSoundPtr = FindSound(_Name);

	if (nullptr == FindSoundPtr)
	{
		MsgAssert("�������� �ʴ� ���带 ����Ϸ��� �߽��ϴ�.");
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


/////////////////////////////// �ɹ�


FMOD::Channel* GameEngineSound::Play()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound�� nullptr �Դϴ� "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;

	FMOD::Channel* SoundControl = nullptr;

 	SoundSystem->playSound(FMODSound, nullptr, false, &SoundControl);

	return SoundControl;
}