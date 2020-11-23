//#include <chrono>
//
//class Unit
//{
//public:
//	float x, y;
//
//	int tick;
//	int state;
//};
//
//class InterfaceForUnit
//{
//public:
//	Unit* target;
//};
//
//class GameContext
//{
//public:
//	Unit a, b;
//	InterfaceForUnit A, B;
//
//	std::chrono::milliseconds baseTerm;
//
//	std::chrono::milliseconds termForUnits;
//	std::chrono::steady_clock::time_point timeForUnits;
//	std::chrono::milliseconds termForInterfaces;
//	std::chrono::steady_clock::time_point timeForInterfaces;
//
//	bool renderFlag;
//	bool inputFlag;
//
//	GameContext() : baseTerm(20), termForUnits(baseTerm), termForInterfaces(baseTerm), renderFlag(true), inputFlag(false)
//	{
//		// 유닛 및 인터페이스 초기화
//
//		timeForUnits = timeForInterfaces = std::chrono::steady_clock::now();
//	}
//
//	void idleCall()
//	{
//		auto now = std::chrono::steady_clock::now();
//		bool flag = true;
//
//		while (true) // 무작정 반복하지 말고 종료 조건 판별할 것
//		{
//			auto unit = timeForUnits + termForUnits;
//			auto interface = timeForInterfaces + termForInterfaces;
//
//			if (unit <= now) // 시각 비교는 안됨. 이거 잘못됨
//			{
//				flag = false;
//				renderFlag = true;
//
//				if (interface <= now)
//				{
//					if (unit <= interface)
//					{
//						updateForUnitWithTime();
//					}
//					else
//					{
//						updateForInterfaceWithTime();
//					}
//				}
//				else
//				{
//					updateForUnitWithTime();
//				}
//			}
//			else if (interface <= now)
//			{
//				flag = false;
//				renderFlag = true;
//
//				updateForInterfaceWithTime();
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		if (flag && renderFlag)
//		{
//			render();
//
//			renderFlag = false;
//		}
//	}
//
//	void updateForUnitWithTime()
//	{
//		timeForUnits += termForUnits;
//		if (termForUnits > baseTerm)
//		{
//			auto temp = (baseTerm - termForUnits).count();
//			auto operand = temp / 2;
//			if (temp & 1)
//			{
//				if (temp > 0)
//				{
//					operand++;
//				}
//				else
//				{
//					operand--;
//				}
//			}
//			termForUnits += std::chrono::milliseconds(operand);
//		}
//		updateForUnit();
//	}
//
//	void updateForInterfaceWithTime()
//	{
//		timeForInterfaces += termForInterfaces;
//		if (termForInterfaces > baseTerm)
//		{
//			auto temp = (baseTerm - termForInterfaces).count();
//			auto operand = temp / 2;
//			if (temp & 1)
//			{
//				if (temp > 0)
//				{
//					operand++;
//				}
//				else
//				{
//					operand--;
//				}
//			}
//			termForInterfaces += std::chrono::milliseconds(operand);
//		}
//		updateForInterface();
//	}
//
//	void updateForUnit()
//	{
//		// 유닛 애니메이트
//		// 판정 적용하기 (콜백가능)
//	}
//
//	void updateForInterface()
//	{
//		// 판정 변수로 판정하기
//		// 느리게 만들기
//		// 입력 비활성화 시키기
//		// 카메라 이동시키기
//		// 카메라 좀있다 되돌리기
//		// 판정 애니메이션 띄우기
//		// + ...
//
//		// 카메라는 인터페이스임 ㅎㅎ 유닛 참조하면서 ㄱㄱ씽
//		// 단방향 애니메이션을 재생한다.
//		// 틱을 센다. 20*150 이 3초다.
//		// 150이 되면 input을 활성화한다.
//
//		// 인터페이스 애니메이트
//	}
//
//	void render()
//	{
//		// 필드 그리기 (unit으로)
//
//		// 카메라로 잘라붙이기 (아직 방법을 모르겠음)
//
//		// interface 그리기
//	}
//};
//
//
//int main()
//{
//	// 객체 생성
//
//	while (1/*루프탈출객체검사*/)
//	{
//		if (1/*게임객체업데이트*/)
//		{
//
//		}
//		else if (1/*메시지객체처리*/)
//		{
//
//		}
//	}
//
//	// 객체 파괴
//}
