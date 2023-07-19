#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineRender.h>
class WorldLevel;

// 월드맵 구성을 표현하는 클래스
class WorldMap
{
	
public:
	// 스테이지 지점을 표시하는 포인트
	class Point
	{
	public:
		Point(const float4& _Pos) {
			Pos = _Pos;
		}
		Point(const float4& _Pos, const std::string_view& _StageName, bool IsLock = false) {
			Pos = _Pos;
			StageName = _StageName;
			Lock = IsLock;
		}
		~Point() {
			if (Up != nullptr)
			{
				Point* DeletePoint = Up;
				DeletePoint->ErasePoint(this);
				this->ErasePoint(DeletePoint);
				delete DeletePoint;
			}
			if (Left != nullptr)
			{
				Point* DeletePoint = Left;
				DeletePoint->ErasePoint(this);
				this->ErasePoint(DeletePoint);
				delete DeletePoint;
			}
			if (Down != nullptr)
			{
				Point* DeletePoint = Down;
				DeletePoint->ErasePoint(this);
				this->ErasePoint(DeletePoint);
				delete DeletePoint;
			}
			if (Right != nullptr)
			{
				Point* DeletePoint = Right;
				DeletePoint->ErasePoint(this);
				this->ErasePoint(DeletePoint);
				delete DeletePoint;
			}
		}
		float4 Pos = float4::Zero;			// 스테이지의 위치
		std::string_view StageName = "";	// 스테이지의 이름 (Level의 Name)
		Point* Up = nullptr;				// 위에 연결된 스테이지
		Point* Down = nullptr;				// 밑에 연결된 스테이지
		Point* Left = nullptr;				// 왼쪽에 연결된 스테이지
		Point* Right = nullptr;				// 오른쪽에 연결된 스테이지
		bool Lock = false;					// 스테이지 잠금 여부

		// 소멸자 실행시 사용되는 상하좌우 포인트 제거 함수
		void ErasePoint(const Point* _Point)
		{
			if (Up == _Point)
			{
				Up = nullptr;
			}
			if (Down == _Point)
			{
				Down = nullptr;
			}
			if (Left == _Point)
			{
				Left = nullptr;
			}
			if (Right == _Point)
			{
				Right = nullptr;
			}
		}
		
		// 해당 포인트가 스테이지의 이름을 가지고 있는지 여부를 반환
		bool IsStage()
		{
			return StageName != "";
		}
		// 해당 포인트로 이동가능하게 잠금을 푸는 함수
		void Unlock()
		{
			Lock = false;
		}
		
		// 포인트를 연결하는 함수들
		void SetUp(Point* _Up)
		{
			Up = _Up;
			_Up->Down = this;
		}
		void SetDown(Point* _Down)
		{
			Down = _Down;
			_Down->Up = this;
		}
		void SetLeft(Point* _Left)
		{
			Left = _Left;
			_Left->Right = this;
		}
		void SetRight(Point* _Right)
		{
			Right = _Right;
			_Right->Left = this;
		}
	};

	// 월드맵 생성시 시작 위치를 지정해준다
	WorldMap(Point* StartPoint)
	{
		StagePointer = StartPoint;
	}
	~WorldMap()
	{
		if (nullptr != StagePointer)
		{
			delete StagePointer;
			StagePointer = nullptr;
		}
	}

	// 현재 나의 위치를 이동하는 함수들
	bool MoveUp()
	{
		if (nullptr != StagePointer->Up)
		{
			if (true == StagePointer->Up->Lock)
			{
				return false;
			}
			StagePointer = StagePointer->Up;
			return true;
		}
		return false;
	}
	bool MoveDown()
	{
		if (nullptr != StagePointer->Down)
		{
			if (true == StagePointer->Down->Lock)
			{
				return false;
			}
			StagePointer = StagePointer->Down;
			return true;
		}
		return false;
	}
	bool MoveLeft()
	{
		if (nullptr != StagePointer->Left)
		{
			if (true == StagePointer->Left->Lock)
			{
				return false;
			}
			StagePointer = StagePointer->Left;
			return true;
		}
		return false;
	}
	bool MoveRight()
	{
		if (nullptr != StagePointer->Right)
		{
			if (true == StagePointer->Right->Lock)
			{
				return false;
			}
			StagePointer = StagePointer->Right;
			return true;
		}
		return false;
	}

	// 현제 위치한 포인트가 스테이지 인지 반환하는 함수
	bool IsStage()
	{
		return StagePointer->IsStage();
	}
	// 스테이지 이름을 반환하는 함수
	const std::string_view& GetStage()
	{
		return StagePointer->StageName;
	}
	// 현제 위치한 포인트의 위치를 반환하는 함수
	const float4& GetPos()
	{
		return StagePointer->Pos;
	}
private:
	Point* StagePointer = nullptr;	// 현제 위치한 포인트를 가르킨다
};

class WorldMario : public GameEngineActor
{
	friend WorldLevel;
public:
	WorldMario();
	~WorldMario();

	//static WorldMario* WorldData;
	//static void Reset();

	WorldMario(const WorldMario& _Other) = delete;
	WorldMario(WorldMario&& _Other) noexcept = delete;
	WorldMario& operator=(const WorldMario& _Other) = delete;
	WorldMario& operator=(WorldMario&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart(GameEngineLevel* _Prev) override;
private:
	const float speed = 2;
	bool IsStart = false;
	bool IsMove = false;
	float4 StartPos = float4::Zero;
	float4 EndPos = float4::Zero;
	float Timer = 0;
	GameEngineRender* AnimationRender = nullptr;
	WorldMap* Map = nullptr;

	// 스테이지 포인트들
	WorldMap::Point* Stage1 = nullptr;
	WorldMap::Point* Stage2 = nullptr;
	WorldMap::Point* Stage3 = nullptr;
	WorldMap::Point* Stage4 = nullptr;
	WorldMap::Point* Stage5 = nullptr;
};

