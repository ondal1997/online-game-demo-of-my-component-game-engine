#pragma once

#include "core.h"

#include "particle.h"
#include "beam.h"

class Human : public Component
{
public:
	Entity* entity;
	const char* state;
	int tick;
	int left;
	int right;
	int up;
	int horizon;
	int charge;

	Human(Entity* entity, const char* state) : entity(entity)
	{
		left = 0;
		right = 0;
		up = 0;
		horizon = 0;
		charge = 0;

		init(state);
	}

	void init(const char* state)
	{
		this->state = state;

		if (state == "idle")
		{
			if (up)
			{
				init("jump");
				return;
			}
			else if (horizon)
			{
				init("move");
				return;
			}

			tick = 0;
		}
		else if (state == "move")
		{
			if (up)
			{
				init("jump");
				return;
			}
			else if (!horizon)
			{
				init("idle");
				return;
			}

			tick = 0;
			auto direction = entity->getComponent<Direction>();
			direction->value = horizon;
		}
		else if (state == "jump")
		{
			auto velocity = entity->getComponent<Velocity>();
			velocity->vy -= 30.0f;
			init("air");
		}
		else if (state == "air")
		{
			if (!horizon)
				return;

			auto direction = entity->getComponent<Direction>();
			direction->value = horizon;
		}
		else if (state == "attack")
		{
			tick = 0;
		}
		else if (state == "skill")
		{
			tick = 0;
		}
		else if (state == "charge")
		{
			tick = 0;
			charge = 1;
		}
		else if (state == "die")
		{
			tick = 0;
		}
	}

	void update()
	{
		auto graphic = entity->getComponent<Graphic>();
		auto deltaPosition = entity->getComponent<DeltaPosition>();
		auto direction = entity->getComponent<Direction>();
		auto velocity = entity->getComponent<Velocity>();

		if (state == "idle")
		{
			if (tick < 20)
			{
				graphic->key = "humanIdle0";
			}
			else if (tick < 40)
			{
				graphic->key = "humanIdle1";
			}

			if (++tick == 40)
				tick = 0;
		}
		else if (state == "move")
		{
			velocity->vx += 0.6f * direction->value;

			if (tick < 20)
			{
				graphic->key = "humanMove0";
			}
			else if (tick < 40)
			{
				graphic->key = "humanMove1";
			}

			if (++tick == 40)
				tick = 0;
		}
		else if (state == "air")
		{
			graphic->key = "humanAir0";

			if (!horizon)
				return;

			velocity->vx += 0.6f * direction->value;
		}
		else if (state == "attack")
		{
			if (tick < 2)
			{
				graphic->key = "humanAttack0";
			}
			else if (tick < 4)
			{
				graphic->key = "humanAttack1";
			}
			else if (tick < 20)
			{
				graphic->key = "humanAttack2";
			}
			else if (tick < 23)
			{
				graphic->key = "humanAttack3";
			}
			else if (tick < 26)
			{
				graphic->key = "humanAttack4";
			}
			else if (tick < 29)
			{
				graphic->key = "humanAttack5";
			}
			else if (tick < 35)
			{
				graphic->key = "humanAttack6";
			}

			if (++tick == 35)
			{
				init("idle");
			}
		}
		else if (state == "skill")
		{
			if (tick == 20)
			{
				entity->getComponent<Sound>()->key = "skill";
				auto position = entity->getComponent<Position>();
				auto deltaPosition = entity->getComponent<DeltaPosition>();
				auto direction = entity->getComponent<Direction>();

				auto beam = entity->world->createEntity();
				beam->addComponent(new Beam(beam, entity));
				beam->addComponent(new Position(position->x, position->y));
				beam->addComponent(new Direction(direction->value));
				beam->addComponent(new DeltaPosition);
				beam->addComponent(new Velocity);
				beam->addComponent(new GravityComponent);
				beam->addComponent(new Graphic);
				beam->addComponent(new Sound);
			}

			if (tick < 10)
			{
				graphic->key = "humanSkill0";
			}
			else if (tick < 20)
			{
				graphic->key = "humanSkill1";
			}
			else if (tick < 25)
			{
				graphic->key = "humanSkill2";
			}
			else if (tick < 30)
			{
				graphic->key = "humanSkill3";
			}
			else if (tick < 35)
			{
				graphic->key = "humanSkill4";
			}
			else if (tick < 45)
			{
				graphic->key = "humanSkill5";
			}

			if (++tick == 45)
			{
				init("idle");
			}
		}
		else if (state == "charge")
		{
			if (tick < 5)
			{
				graphic->key = "humanCharge0";
			}
			else if (tick < 10)
			{
				graphic->key = "humanCharge1";
			}
			else if (tick < 15)
			{
				graphic->key = "humanCharge2";
			}
			else if (tick < 17)
			{
				graphic->key = "humanCharge3";
			}
			else if (tick < 20)
			{
				graphic->key = "humanCharge4";
			}
			else if (tick < 23)
			{
				graphic->key = "humanCharge5";
			}
			else if (tick < 33)
			{
				graphic->key = "humanCharge6";
			}
			else if (tick < 43)
			{
				graphic->key = "humanCharge7";
			}

			if (charge)
			{
				if (tick == 23)
				{
					entity->getComponent<Sound>()->key = "gathering";
				}

				if (tick >= 23)
				{
					auto mp = entity->getComponent<MpComponent>();
					mp->delta += 0.02f;
				}

				if (++tick == 43)
				{
					tick = 23;
				}
			}
			else
			{
				if (tick >= 23)
				{
					tick = 22;
				}
				else if (tick == 0)
				{
					init("idle");
				}
				else
				{
					tick--;
				}
			}
		}
		else if (state == "die")
		{
			if (tick < 20)
			{
				graphic->key = "humanDie0";
			}
			else if (tick < 25)
			{
				graphic->key = "humanDie1";
			}
			else if (tick < 30)
			{
				graphic->key = "humanDie2";
			}
			else if (tick < 35)
			{
				graphic->key = "humanDie3";
			}
			else
			{
				graphic->key = "humanDie4";
			}

			if (++tick > 160)
			{
				tick = 160;
			}
		}
	}

	void input(char data)
	{
		if (data == 0)
		{
			left = 1;
			horizon = -1;
		}
		else if (data == 1)
		{
			right = 1;
			horizon = 1;
		}
		else if (data == 0 + 7)
		{
			left = 0;
			horizon = right ? 1 : 0;
		}
		else if (data == 1 + 7)
		{
			right = 0;
			horizon = left ? -1 : 0;
		}
		else if (data == 2)
		{
			up = 1;
		}
		else if (data == 2 + 7)
		{
			up = 0;
		}

		if (state == "idle")
		{
			switch (data)
			{
			case 0:
			case 1:
				init("move");
				break;
			case 2:
				init("jump");
				break;
			case 4:
				init("attack");
				break;
			case 5:
				init("charge");
				break;
			case 6:
			{
				auto mp = entity->getComponent<MpComponent>();
				if (mp->value >= 1.0f)
				{
					mp->delta = -1.0f;
					init("skill");
				}
				break;
			}
			}
		}
		else if (state == "move")
		{
			switch (data)
			{
			case 0:
			case 1:
			case 0 + 7:
			case 1 + 7:
			{
				if (horizon)
				{
					auto direction = entity->getComponent<Direction>();
					direction->value = horizon;
				}
				else
				{
					init("idle");
				}
				break;
			}
			case 2:
				init("jump");
				break;
			case 4:
				init("attack");
				break;
			case 5:
				init("charge");
				break;
			case 6:
			{
				auto mp = entity->getComponent<MpComponent>();
				if (mp->value >= 1.0f)
				{
					mp->delta = -1.0f;
					init("skill");
				}
				break;
			}
			}
		}
		else if (state == "air")
		{
			switch (data)
			{
			case 0:
			case 1:
			case 0 + 7:
			case 1 + 7:
			{
				if (horizon)
				{
					auto direction = entity->getComponent<Direction>();
					direction->value = horizon;
				}
				break;
			}
			case 4:
				init("attack");
				break;
			case 6:
			{
				auto mp = entity->getComponent<MpComponent>();
				if (mp->value >= 1.0f)
				{
					mp->delta = -1.0f;
					init("skill");
				}
				break;
			}
			}
		}
		else if (state == "charge")
		{
			switch (data)
			{
			case 5 + 7:
				charge = 0;
				break;
			}
		}
	}
};

class HumanSystem : public System
{
public:
	HumanSystem()
	{
		addComponentType<Human>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto human = entity->getComponent<Human>();

			if (human->state == "attack" && human->tick == 20)
			{
				entity->getComponent<Sound>()->key = "swing";
				auto pos = entity->getComponent<Position>();

				for (auto target : entities)
				{
					if (entity == target)
						continue;

					auto targetPos = target->getComponent<Position>();

					auto direction = entity->getComponent<Direction>();

					if (targetPos->x < pos->x - 40 - 140 * (direction->value == -1))
						continue;
					if (targetPos->x > pos->x + 40 + 140 * (direction->value == 1))
						continue;
					if (targetPos->y < pos->y - 100)
						continue;
					if (targetPos->y > pos->y + 100)
						continue;

					target->getComponent<Sound>()->key = "pak";

					//���� ���
					auto hp = target->getComponent<HpComponent>();
					auto velocity = target->getComponent<Velocity>();

					hp->delta -= 1.0f;
					velocity->vx += direction->value * 10.0f;
					velocity->vy -= 10.0f;

					// �ݴ� ����
					auto particle = entity->world->createEntity();
					particle->addComponent(new Particle(particle, "hitAttack"));
					particle->addComponent(new Position(targetPos->x, targetPos->y - 100.0f));
					particle->addComponent(new Direction(direction->value));
					particle->addComponent(new Graphic);

					// �� ����
					for (int i = 0; i < 10; i++)
					{
						auto blood = entity->world->createEntity();
						blood->addComponent(new Particle(blood, "blood"));
						blood->addComponent(new Position(targetPos->x, targetPos->y - 100.0f));
						blood->addComponent(new Direction(direction->value));
						// �ӷ��� ���ο��� ��
						blood->addComponent(new DeltaPosition);
						blood->addComponent(new GravityComponent);
						blood->addComponent(new Graphic);
					}
				}
			}

			human->update();
		}
	}
};
