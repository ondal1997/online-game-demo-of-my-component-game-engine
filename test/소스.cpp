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
//		// ���� �� �������̽� �ʱ�ȭ
//
//		timeForUnits = timeForInterfaces = std::chrono::steady_clock::now();
//	}
//
//	void idleCall()
//	{
//		auto now = std::chrono::steady_clock::now();
//		bool flag = true;
//
//		while (true) // ������ �ݺ����� ���� ���� ���� �Ǻ��� ��
//		{
//			auto unit = timeForUnits + termForUnits;
//			auto interface = timeForInterfaces + termForInterfaces;
//
//			if (unit <= now) // �ð� �񱳴� �ȵ�. �̰� �߸���
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
//		// ���� �ִϸ���Ʈ
//		// ���� �����ϱ� (�ݹ鰡��)
//	}
//
//	void updateForInterface()
//	{
//		// ���� ������ �����ϱ�
//		// ������ �����
//		// �Է� ��Ȱ��ȭ ��Ű��
//		// ī�޶� �̵���Ű��
//		// ī�޶� ���ִ� �ǵ�����
//		// ���� �ִϸ��̼� ����
//		// + ...
//
//		// ī�޶�� �������̽��� ���� ���� �����ϸ鼭 ������
//		// �ܹ��� �ִϸ��̼��� ����Ѵ�.
//		// ƽ�� ����. 20*150 �� 3�ʴ�.
//		// 150�� �Ǹ� input�� Ȱ��ȭ�Ѵ�.
//
//		// �������̽� �ִϸ���Ʈ
//	}
//
//	void render()
//	{
//		// �ʵ� �׸��� (unit����)
//
//		// ī�޶�� �߶���̱� (���� ����� �𸣰���)
//
//		// interface �׸���
//	}
//};
//
//
//int main()
//{
//	// ��ü ����
//
//	while (1/*����Ż�ⰴü�˻�*/)
//	{
//		if (1/*���Ӱ�ü������Ʈ*/)
//		{
//
//		}
//		else if (1/*�޽�����üó��*/)
//		{
//
//		}
//	}
//
//	// ��ü �ı�
//}
