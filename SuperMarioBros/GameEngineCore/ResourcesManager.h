#pragma once
#include <string>
#include <map>
#include <GameEngineBase/GameEnginePath.h>

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
class GameEngineWindowTexture;
class ResourcesManager
{
public:
	// delete Function
	ResourcesManager(const ResourcesManager& _Other) = delete;
	ResourcesManager(ResourcesManager&& _Other) noexcept = delete;
	ResourcesManager& operator=(const ResourcesManager& _Other) = delete;
	ResourcesManager& operator=(ResourcesManager&& _Other) noexcept = delete;

	// 3. 리턴해준다.
	static ResourcesManager& GetInst() 
	{
		return Inst;
	}

	//static ResourcesManager* GetInst()
	//{
	//	if (nullptr == Inst)
	//	{
	//		Inst = new ResourcesManager();
	//	}
	//	return Inst;
	//}

	//static void ResetInst() 
	//{
	//	if (nullptr != Inst)
	//	{
	//		delete Inst;
	//  Inst = new ResourcesManager();
	//}

	//static void Destroy() 
	//{
	//	if (nullptr != Inst)
	//	{
	//		delete Inst;
	//	}
	//}

	// 파일명이 곧 찾기위한 이름이 된다.
	GameEngineWindowTexture* TextureLoad(const std::string& _Path) 
	{
		GameEnginePath LoadPath = _Path;
		return TextureLoad(LoadPath.GetFileName(), _Path);
	}

	GameEngineWindowTexture* TextureLoad(const std::string& _Name, const std::string& _Path);

	GameEngineWindowTexture* FindTexture(const std::string& _Name);

	bool IsLoadTexture(const std::string& _Image);


protected:

private:
	// 2. 내부에 static 포인터든 값이든 뭐든 1개를 둔다.
	static ResourcesManager Inst;

	// static ResourcesManager* Inst;

	// 1. 생성자를 private안에 넣는다.
	ResourcesManager();
	~ResourcesManager();

	std::map<std::string, GameEngineWindowTexture*> AllTexture;
};

