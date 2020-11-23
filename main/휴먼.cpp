struct Human // ������Ʈ�� �� ������ �ִ� ������ ����ϱ� �� ����
{
public:
	const char* state;
	int tick;

	bool left;
	bool right;
	bool up;
	int face;

	bool ��������;

	Human(const char* state) : state(state)
	{
		enter();
	}

	void input()
	{
		if (����Ű�ٿ�)
			up = true;
		else if (����Ű��)
			up = false;
		else if (��Ű�ٿ�)
		{
			left = true;
			face = -1;
		}
		else if (��Ű�ٿ�)
		{
			right = true;
			face = 1;
		}
		else if (��Ű��)
		{
			left = false;
			face = right ? 1 : 0;
		}
		else if (��Ű��)
		{
			right = false;
			face = left ? -1 : 0;
		}

		if (state == "idle")
		{
			if (����Ű�ٿ�)
			{
				state = "jump";
				enter();
			}
			else if (��Ű�ٿ�)
			{
				state = "move";
				enter();
			}
			else if (��Ű�ٿ�)
			{
				state = "move";
				enter();
			}
			else if (����Ű�ٿ�)
			{
				state = "attack";
				enter();
			}
			else if (����Ű�ٿ�)
			{
				state = "charge";
				enter();
			}
		}

		if (state == "move")
		{
			if (����Ű�ٿ�)
			{
				state = "jump";
				enter();
			}
			else if (��Ű || ��Ű)
			{
				Direction.value���� �Ǵ� ���� �Ǵ� state = "idle", enter();
			}
			else if (����Ű�ٿ�)
			{
				state = "attack";
				enter();
			}
		}

		if (state == "air")
		{
			if (��Ű || ��Ű)
			{
				Direction.value���� �Ǵ� ����
			}
			else if (����Ű�ٿ�)
			{
				state = "attack";
				enter();
			}
		}

		if (state == "charge")
		{
			if (����Ű��)
			{
				�������� = false;

				if (tick == 0)
				{
					��� �̵� ���� �������� ����.
				}
				else if ������ƽ�̸� ��������ƽ���� ����;
				else ƽ�� �Ѵܰ� ���ҽ�Ų��.;
			}
		}
	}

	void enter()
	{
		if (state == "idle")
		{
			tick = 0;
		}
		if (state == "move")
		{
			tick = 0;
			Direction.value = face;
		}
		if (state == "jump")
		{
			Velocity.vy -= Jump.value;
			state = "air";
			enter();
		}
		if (state == "air")
		{
			Direction.value = face;
		}
		if (state == "attack")
		{
			tick = 0;
		}
		if (state == "charge")
		{
			tick = 0;
		}
	}

	void update()
	{
		if (state == "idle")
		{
			tick�� ���� ���;
			tick++;
			tick�� ������ ����� tick = 0;
		}

		if (state == "move")
		{
			����, ������, �̵������� �̵�
			tick�� ���� ���;
			tick++;
			tick�� ������ ����� tick = 0;
		}

		if (state == "air")
		{
			���;
			Ű, ����, ������, �̵������� �̵�
		}

		if (state == "attack")
		{
			tick�� ���� ���, �׼�;
			tick++;
			tick�� ������ ����� : ��� �Ǵ� �̵� �Ǵ� ���� �Ǵ� ���� enter();
		}

		if (state == "charge")
		{
			tick�� ���� ���, �׼�;
			�������߿� ���� tick ����;
			tick�� ������ ����� : ����ƽ;
			tick�� ������ ����� : ��� �Ǵ� �̵� �Ǵ� ���� �Ǵ� ���� enter();
		}
	}
};

class HumanSystem
{
public:
	HumanSystem()
	{
		�ñ״�ó += Human;
	}

	void update()
	{
		��ƼƼ��.Human.update();
	}
}

int main()
{
	Entity a;
	a.addComponent(Human("idle"));
}
