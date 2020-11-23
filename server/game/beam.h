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



		deltaPosition->dx += direction->value * 5.0f;

		if (tick == 0)
		{
			entity->getComponent<Sound>()->key = "beam";
		}

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

		if (++time == 180)
			entity->addComponent(new DeleteComponent);
	}
};

