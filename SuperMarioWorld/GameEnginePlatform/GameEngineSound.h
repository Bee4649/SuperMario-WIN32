#pragma once
#include <string_view>

// ����
#include <GameEnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

// ���� : ��ü�� ���� ���� ����
class GameEngineSound
{
public:	
	void SoundLoad(const std::string_view& _Path);
	FMOD::Channel* Play();
	
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

protected:

private:
	FMOD::Sound* FMODSound = nullptr;
};
