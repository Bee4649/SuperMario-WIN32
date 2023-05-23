#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <string>

// Ό³Έν :
class BackGround : public GameEngineActor
{
public:
	// constrcuter destructer
	BackGround();
	~BackGround();

	// delete Function
	BackGround(const BackGround& _Other) = delete;
	BackGround(BackGround&& _Other) noexcept = delete;
	BackGround& operator=(const BackGround& _Other) = delete;
	BackGround& operator=(BackGround&& _Other) noexcept = delete;

	void Init(const std::string& _FileName, const std::string& _DebugFileName);

	void SwitchRender();

protected:

private:
	bool SwitchRenderValue = true;
	std::string FileName;
	class GameEngineRenderer* Renderer;
	class GameEngineRenderer* DebugRenderer;

	void Start() override;
	void Update(float _Delta) override;
	void Render() override;
	void Release() override;

};

