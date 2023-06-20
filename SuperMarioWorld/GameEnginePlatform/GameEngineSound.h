#pragma once
#include <string_view>

// 선언
#include <GameEnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

// 설명 : 자체가 사운드 파일 역할
class GameEngineSound
{
public:	
	static void SoundUpdate();

public:
	// constrcuter destructer
	GameEngineSound();
	~GameEngineSound();

	// delete Function
	GameEngineSound(const GameEngineSound& _Other) = delete;
	GameEngineSound(GameEngineSound&& _Other) noexcept = delete;
	GameEngineSound& operator=(const GameEngineSound& _Other) = delete;
	GameEngineSound& operator=(GameEngineSound&& _Other) noexcept = delete;

	void SoundLoad(const std::string_view& _Path);
	FMOD::Channel* Play();

protected:

private:
	FMOD::Sound* FMODSound = nullptr;
};

