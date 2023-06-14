#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRenderer.h>


// fade 메서드는 요소를 서서히 나타내지고,서서히 없어지게 해주는 메서드
// (선택요소).메서드(스피드,콜백)

enum class FadeState {
	FadeIn, // 서서히 보이게 한다.
	FadeOut, // 서서히 없어지게 한다.
	FadeOutLoad,
	STOP,
};

class LevelLoader : public GameEngineActor
{
public:
	LevelLoader();
	~LevelLoader();

	static void Goal();
	static void FadeIn();
	static void FadeOut();
	static void ChangeLevel(const std::string_view& _Name);

	template <typename OrderType>
	static void SetOrder(OrderType _Order)
	{
		MainLoader->FadeRender->SetOrder(static_cast<int>(_Order));
	}

	LevelLoader(const LevelLoader& _Other) = delete;
	LevelLoader(LevelLoader&& _Other) noexcept = delete;
	LevelLoader& operator=(const LevelLoader& _Other) = delete;
	LevelLoader& operator=(LevelLoader&& _Other) noexcept = delete;

protected:
	void Start() override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void Update(float _DeltaTime);
private:

	static LevelLoader* MainLoader;
	std::string_view LevelName = "";
	GameEngineRenderer* FadeRender = nullptr;
	FadeState State = FadeState::STOP;

	bool IsFadeIn;
	bool IsFadeOut;
	float Speed = 1.5f;
	float Timer = 0;

	void FadeInUpdate(float _DeltaTime);
	void FadeOutUpdate(float _DeltaTime);
	void FadeOutLoadUpdate(float _DeltaTime);
	void ClearUpdate(float _DeltaTime);
};