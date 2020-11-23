#pragma once

#include "engine.h"

class DeleteComponent : public Component
{
};

class DeleteSystem : public System
{
public:
	DeleteSystem()
	{
		addComponentType<DeleteComponent>();
	}

	void update()
	{
		while (!entities.empty())
		{
			world->destroyEntity(entities.back());
		}
	}
};

class Direction : public Component
{
public:
	Direction(int value = 1) : value(value)
	{
	}

	int value;
};

class Position : public Component
{
public:
	Position(float x = 0.0f, float y = 0.0f) : x(x), y(y)
	{
	}

	float x;
	float y;
};

class Velocity : public Component
{
public:
	Velocity(float vx = 0.0f, float vy = 0.0f) : vx(vx), vy(vy)
	{
	}

	float vx;
	float vy;
};

class DeltaPosition : public Component
{
public:
	DeltaPosition(float dx = 0.0f, float dy = 0.0f) : dx(dx), dy(dy)
	{
	}

	float dx;
	float dy;
};

class GravityComponent : public Component
{
};

class GravitySystem : public System
{
public:
	GravitySystem(float value = 10.0f) : value(value)
	{
		addComponentType<Velocity>();
		addComponentType<GravityComponent>();
	}
	float value;

	void update()
	{
		for (auto entity : entities)
		{
			auto velocity = entity->getComponent<Velocity>();

			velocity->vy += value;
		}
	}
};

class Translate : public System
{
public:
	Translate()
	{
		addComponentType<Position>();
		addComponentType<Velocity>();
		addComponentType<DeltaPosition>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto position = entity->getComponent<Position>();
			auto velocity = entity->getComponent<Velocity>();
			auto deltaPosition = entity->getComponent<DeltaPosition>();

			deltaPosition->dx += velocity->vx;
			deltaPosition->dy += velocity->vy;

			position->x += deltaPosition->dx;
			position->y += deltaPosition->dy;

			deltaPosition->dx = 0.0f;
			deltaPosition->dy = 0.0f;
		}
	}
};

class LandSystem : public System
{
public:
	LandSystem(float value = 0.25f) : value(value)
	{
		addComponentType<Position>();
		addComponentType<Velocity>();
	}
	float value;

	void update()
	{
		for (auto entity : entities)
		{
			auto position = entity->getComponent<Position>();
			auto velocity = entity->getComponent<Velocity>();

			if (position->y >= 0.0f)
			{
				position->y = velocity->vy = 0.0f;

				if (entity->hasComponent<Human>())
				{
					auto human = entity->getComponent<Human>();
					if (human->state == "air")
						human->init("idle");
				}
			}
			else if (entity->hasComponent<Human>())
			{
				auto human = entity->getComponent<Human>();
				if (human->state == "idle" || human->state == "move")
					human->init("air");
			}

			if (position->y == 0.0f)
				if (velocity->vx < 0.0f)
					if (velocity->vx >= -value)
						velocity->vx = 0.0f;
					else
						velocity->vx += value;
				else
					if (velocity->vx <= value)
						velocity->vx = 0.0f;
					else
						velocity->vx -= value;
		}
	}
};

class WallSystem : public System
{
public:
	WallSystem(int fieldWidth) : fieldWidth(fieldWidth)
	{
		addComponentType<Human>();
	}
	int fieldWidth;

	void update()
	{
		for (auto entity : entities)
		{
			auto position = entity->getComponent<Position>();
			auto velocity = entity->getComponent<Velocity>();

			if (position->x < 0.0f)
			{
				position->x = 0.0f;

				if (velocity->vx < 0.0f)
					velocity->vx *= -1;
			}
			else if (position->x > fieldWidth)
			{
				position->x = fieldWidth;
				
				if (velocity->vx > 0.0f)
					velocity->vx *= -1;
			}
		}
	}
};

class HpComponent : public Component
{
public:
	HpComponent(float value) : value(value), max(value), delta(0)
	{
	}

	float value;
	float max;
	float delta;
};

class MpComponent : public Component
{
public:
	MpComponent(float value) : value(value), max(value), delta(0)
	{
	}

	float value;
	float max;
	float delta;
};

class HpSystem : public System
{
public:
	HpSystem()
	{
		addComponentType<HpComponent>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto hpComponent = entity->getComponent<HpComponent>();

			hpComponent->value += hpComponent->delta;

			hpComponent->delta = 0.0f;

			if (hpComponent->value < 0.0f)
				hpComponent->value = 0.0f;
			else if (hpComponent->value > hpComponent->max)
				hpComponent->value = hpComponent->max;

			if (hpComponent->value == 0.0f && entity->hasComponent<Human>())
			{
				auto human = entity->getComponent<Human>();
				human->init("die");
			}
		}
	}
};

class MpSystem : public System
{
public:
	MpSystem()
	{
		addComponentType<MpComponent>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto mpComponent = entity->getComponent<MpComponent>();

			mpComponent->value += mpComponent->delta;

			mpComponent->delta = 0.0f;

			if (mpComponent->value < 0)
				mpComponent->value = 0;
			else if (mpComponent->value > mpComponent->max)
				mpComponent->value = mpComponent->max;
		}
	}
};

class CameraComponent : public Component
{
};

class CameraSystem : public System
{
public:
	CameraSystem(int padding, int fieldWidth, int fieldHeight, int renderWidth, int renderHeight, float x, float y, float width, float height) : padding(padding), fieldWidth(fieldWidth), fieldHeight(fieldHeight), renderWidth(renderWidth), renderHeight(renderHeight), x(x), y(y), width(width), height(height)
	{
		addComponentType<Position>();
		addComponentType<CameraComponent>();

		targetX = x;
		targetY = y;
		targetWidth = width;
		targetHeight = height;
	}
	int padding;

	int fieldWidth;
	int fieldHeight;

	int renderWidth;
	int renderHeight;

	float targetX;
	float targetY;
	float targetWidth;
	float targetHeight;

	float x;
	float y;
	float width;
	float height;

	void update()
	{
		if (entities.empty())
			return;

		auto position = entities[0]->getComponent<Position>();

		float minx = position->x;
		float maxx = position->x;
		float miny = position->y;
		float maxy = position->y;

		for (size_t i = 1; i < entities.size(); i++)
		{
			auto position = entities[i]->getComponent<Position>();

			if (minx > position->x)
				minx = position->x;

			if (maxx < position->x)
				maxx = position->x;

			if (miny > position->y)
				miny = position->y;

			if (maxy < position->x)
				maxy = position->x;
		}

		float left = minx - padding;
		float right = maxx + padding;

		if (left < 0.0f)
			left = 0.0f;
		if (right > fieldWidth)
			right = fieldWidth;

		targetX = left;
		targetWidth = right - left;

		float top = miny - padding;
		float bottom = maxy + padding;

		if (top < 0.0f)
			top = 0.0f;
		if (bottom > fieldHeight)
			bottom = fieldHeight;

		targetY = top;
		targetHeight = bottom - top;

		float yBaseWidth = (height / renderHeight * renderWidth);

		if (targetWidth >= yBaseWidth)
		{
			float xBaseHeight = (targetWidth / renderWidth * renderHeight);
			targetHeight = xBaseHeight;
			targetY = top + (bottom - top) / 2 - xBaseHeight / 2;
		}
		else
		{
			targetWidth = yBaseWidth;
			targetX = left + (right - left) / 2 - yBaseWidth / 2;
		}

		x += (targetX - x) / 2.0f;
		width += (targetWidth - width) / 2.0f;
		y += (targetY - y) / 2.0f;
		height += (targetHeight - height) / 2.0f;
	}
};

class HpBar : public Component
{
public:
	HpBar(Entity* target, float effect, float value) : target(target), effect(effect), value(value)
	{
	}
	Entity* target;
	float effect;
	float value;
};

class MpBar : public Component
{
public:
	MpBar(Entity* target, float value) : target(target), value(value)
	{
	}
	Entity* target;
	float value;
};

class HpBarSystem : public System
{
public:
	HpBarSystem()
	{
		addComponentType<HpBar>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto hpBar = entity->getComponent<HpBar>();
			auto hp = hpBar->target->getComponent<HpComponent>();

			hpBar->value = hp->value / hp->max;
			hpBar->effect += (hpBar->value - hpBar->effect) / 20.0f;
		}
	}
};

class MpBarSystem : public System
{
public:
	MpBarSystem()
	{
		addComponentType<MpBar>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			auto mpBar = entity->getComponent<MpBar>();
			auto mp = mpBar->target->getComponent<MpComponent>();

			mpBar->value = mp->value / mp->max;
		}
	}
};

class Graphic : public Component
{
public:
	Graphic(const char* key = nullptr, bool bInterface = false) : key(key), bInterface(bInterface)
	{
	}

	const char* key;
	bool bInterface;
};

class Render : public System
{
public:
	Render(CameraSystem* camera) : camera(camera)
	{
		addComponentType<Position>();
		addComponentType<Graphic>();
	}
	CameraSystem* camera;

	void update()
	{
		// �ΰ��� ��Ʈ�� (����, �������̽�)

		for (auto entity : entities)
		{
			if (entity->hasComponent<Direction>())
			{

			}

			auto position = entity->getComponent<Position>();
			auto graphic = entity->getComponent<Graphic>();

			// has Hpbar? ���� �׷� ����;;; ���̽��� �׸���, ����Ʈ�� �׸���, ������ �׸���.
			// has Mpbar? ���� �׷� ����;;; ���̽��� �׸���, ���� ���� ������ ����Ʈ�� �׸���.
			// �ƴϸ� �׷���->key�� �����ϴ� ��Ʈ���� �����ͼ� �׸���.

			// �׷���->�������̽� ? �������̽��� �׸��� : ���ӿ� �׸���.

			// ���� �׷����� �ı��Ѵ�.
		}

		// ī�޶�� ������ �ڸ��� ���δ�.
		// �������̽��� ���δ�.
	}
};

/////////////////////////////////////

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
		init(state);

		left = 0;
		right = 0;
		up = 0;
		horizon = 0;
		charge = 0;
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
			velocity->vy -= 10.0f;
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
			deltaPosition->dx += 5.0f * direction->value;

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
			if (!horizon)
				return;

			deltaPosition->dx += 5.0f * direction->value;
		}
		else if (state == "attack")
		{
			if (tick < 5)
			{
				graphic->key = "humanAttack0";
			}
			else if (tick < 10)
			{
				graphic->key = "humanAttack1";
			}
			else if (tick < 30)
			{
				graphic->key = "humanAttack2";
			}
			else if (tick < 35)
			{
				graphic->key = "humanAttack3";
			}
			else if (tick < 40)
			{
				graphic->key = "humanAttack4";
			}
			else if (tick < 45)
			{
				graphic->key = "humanAttack5";
			}
			else if (tick < 80)
			{
				graphic->key = "humanAttack6";
			}

			if (++tick == 80)
			{
				init("idle");
			}
		}
		else if (state == "skill")
		{
			if (tick == 30)
			{
				auto position = entity->getComponent<Position>();
				auto deltaPosition = entity->getComponent<DeltaPosition>();
				auto direction = entity->getComponent<Direction>();

				auto beam = entity->world->createEntity();
				beam->addComponent(new Beam(beam, entity));
				beam->addComponent(new Position(position->x, position->y));
				beam->addComponent(new Direction(direction->value));
				beam->addComponent(new DeltaPosition);
				beam->addComponent(new GravityComponent);
				beam->addComponent(new Graphic);
			}

			if (tick < 15)
			{
				graphic->key = "humanSkill0";
			}
			else if (tick < 30)
			{
				graphic->key = "humanSkill1";
			}
			else if (tick < 35)
			{
				graphic->key = "humanSkill2";
			}
			else if (tick < 40)
			{
				graphic->key = "humanSkill3";
			}
			else if (tick < 45)
			{
				graphic->key = "humanSkill4";
			}
			else if (tick < 60)
			{
				graphic->key = "humanSkill5";
			}

			if (++tick == 60)
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
				if (tick >= 23)
				{
					auto mp = entity->getComponent<MpComponent>();
					mp->delta += 0.02f;
				}

				if (++tick == 43)
				{
					tick = 5;
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

		if (++tick > 40)
		{
			tick = 40;
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
			horizon = left ? 1 : 0;
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
			
			if (human->state == "attack" && human->tick == 35)
			{
				auto pos = entity->getComponent<Position>();

				for (auto target : entities)
				{
					if (entity == target)
						continue;

					auto targetPos = target->getComponent<Position>();

					if (targetPos->x < pos->x)
						continue;
					if (targetPos->x > pos->x + 100)
						continue;
					if (targetPos->y < pos->y - 100)
						continue;
					if (targetPos->y > pos->y + 100)
						continue;

					//���� ���
					auto direction = entity->getComponent<Direction>();

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

#include <set>

class Beam : public Component
{
public:
	Entity* entity;
	std::set<Entity*> friends;
	int tick;
	int time;

	Beam(Entity* entity, Entity* owner) : entity(entity)
	{
		friends.insert(owner);
		tick = 0;
		time = 0;
	}

	void update()
	{
		auto graphic = entity->getComponent<Graphic>();
		auto position = entity->getComponent<Position>();
		auto deltaPosition = entity->getComponent<DeltaPosition>();
		auto direction = entity->getComponent<Direction>();

		// ����ũ ����
		for (int i = 0; i < 10; i++)
		{
			auto spark = entity->world->createEntity();
			spark->addComponent(new Particle(spark, "spark"));
			spark->addComponent(new Position(position->x, position->y));
			spark->addComponent(new Direction(direction->value));
			// �ӷ��� ���ο��� ��
			spark->addComponent(new DeltaPosition);
			spark->addComponent(new GravityComponent);
			spark->addComponent(new Graphic);
		}

		deltaPosition->dx += direction->value * 7.0f;

		if (tick < 20)
		{
			graphic->key = "beam0";
		}
		else if (tick < 40)
		{
			graphic->key = "beam1";
		}

		if (++tick == 40)
			tick = 0;

		if (++time == 800)
			entity->addComponent(new DeleteComponent);
	}
};

class Particle : public Component
{
public:
	Entity* entity;
	const char* type;
	int tick;
	int max;

	Particle(Entity* entity, const char* type) : entity(entity), type(type)
	{
		if (type == "blood" || type == "spark")
		{
			entity->addComponent(new Velocity(0.0f, -5.0f));

			tick = 0; // ����
			max = 400; // ����
		}
		else
		{
			tick = 0;
		}
	}

	void update()
	{
		auto graphic = entity->getComponent<Graphic>();

		if (type == "blood" || type == "spark")
		{
			
			if (tick < 200)
			{
				if (type == "blood")
				{
					graphic->key = "blood0";
				}
				else
				{
					graphic->key = "spark0";
				}
			}
			else
			{
				if (type == "blood")
				{
					graphic->key = "blood1";
				}
				else
				{
					graphic->key = "spark1";
				}
			}

			if (++tick == max)
				entity->addComponent(new DeleteComponent);
		}
		else
		{
			if (tick < 30)
			{
				if (type == "hitAttack")
				{
					graphic->key = "hitAttack0";
				}
				else
				{
					graphic->key = "hitBeam0";
				}
			}
			else if (tick < 60)
			{
				if (type == "hitAttack")
				{
					graphic->key = "hitAttack1";
				}
				else
				{
					graphic->key = "hitBeam1";
				}
			}
			else if (tick < 90)
			{
				if (type == "hitAttack")
				{
					graphic->key = "hitAttack2";
				}
				else
				{
					graphic->key = "hitBeam2";
				}
			}

			if (++tick == 90)
				entity->addComponent(new DeleteComponent);
		}
	}
};

class BeamSystem : public System
{
	BeamSystem(HumanSystem* humanSystem) : humanSystem(humanSystem)
	{
		addComponentType<Beam>();
	}
	HumanSystem* humanSystem;

	void update()
	{
		for (auto entity : entities)
		{
			auto beam = entity->getComponent<Beam>();
			beam->update();

			auto pos = entity->getComponent<Position>();

			for (auto target : humanSystem->entities)
			{
				if (beam->friends.find(target) != beam->friends.end())
					continue;

				auto targetPos = target->getComponent<Position>();

				if (targetPos->x < pos->x - 100)
					continue;
				if (targetPos->x > pos->x + 100)
					continue;
				if (targetPos->y < pos->y - 100)
					continue;
				if (targetPos->y > pos->y + 100)
					continue;

				//���� ���
				beam->friends.insert(target);

				auto direction = entity->getComponent<Direction>();

				auto hp = target->getComponent<HpComponent>();
				auto velocity = target->getComponent<Velocity>();
				
				hp->delta -= 1.0f;
				velocity->vx += direction->value * 10.0f;
				velocity->vy -= 10.0f;

				// �ݴ� ����
				auto particle = entity->world->createEntity();
				particle->addComponent(new Particle(particle, "hitBeam"));
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
	}
};
class ParticleSystem : public System
{
	ParticleSystem()
	{
		addComponentType<Particle>();
	}

	void update()
	{
		for (auto entity : entities)
		{
			entity->getComponent<Particle>()->update();
		}
	}
};

/////////////////////////////////////

class EndSystem : public System
{
public:
	Entity* left;
	Entity* right;
	int* flag;

	EndSystem(Entity* left, Entity* right, int* flag) : left(left), right(right), flag(flag)
	{
	}

	void update()
	{
		auto leftHuman = left->getComponent<Human>();
		auto rightHuman = right->getComponent<Human>();

		if (leftHuman->state == "die" && leftHuman->tick == 40)
		{
			if (rightHuman->state == "die" && rightHuman->tick == 40)
			{
				*flag = 2;
			}
			else
			{
				*flag = 1;
			}
		}
		else if (rightHuman->state == "die" && rightHuman->tick == 40)
		{
			*flag = 0;
		}
	}
};
