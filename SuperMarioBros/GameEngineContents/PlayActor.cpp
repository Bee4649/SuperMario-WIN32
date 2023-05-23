#include "PlayActor.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/ResourcesManager.h>

PlayActor::PlayActor() 
{
}

PlayActor::~PlayActor() 
{
}

void PlayActor::Gravity(float _Delta) 
{
	if (false == IsGravity)
	{
		return;
	}

	// 가장기본적인 물리는 모든힘이 다 +진게 나의 이동 방햐이 된다.
	GravityVector += float4::DOWN * GravityPower * _Delta;

	AddPos(GravityVector);
}

void PlayActor::CameraFocus() 
{
	float4 WindowScale = GameEngineWindow::MainWindow.GetScale();
	GetLevel()->GetMainCamera()->SetPos(GetPos() + float4{ -WindowScale.hX(), -WindowScale.hY() });
}

void PlayActor::SetGroundTexture(const std::string& _GroundTextureName)
{
	GroundTexture = ResourcesManager::GetInst().FindTexture(_GroundTextureName);

	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("존재하지 않는 텍스처로 픽셀충돌을 하려고 했습니다.");
	}

}

int PlayActor::GetGroundColor(unsigned int _DefaultColor, float4 _Pos)
{
	if (nullptr == GroundTexture)
	{
		MsgBoxAssert("땅 체크용 텍스처가 존재하지 않습니다.");
	}

	return GroundTexture->GetColor(_DefaultColor, GetPos() + _Pos);
}