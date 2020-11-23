#pragma once

#include "core.h"
#include "particle.h"


/////////////////////////////////////




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

		// 스파크 생성
		for (int i = 0; i < 10; i++)
		{
			auto spark = entity->world->createEntity();
			spark->addComponent(new Particle(spark, "spark"));
			spark->addComponent(new Position(position->x, position->y));
			spark->addComponent(new Direction(direction->value));
			// 속력은 내부에서 줌
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

