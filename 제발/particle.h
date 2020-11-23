#pragma once

#include "core.h"


/////////////////////////////////////

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

			tick = 0; // ·£´ý
			max = 400; // ·£´ý
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

class ParticleSystem : public System
{
public:
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

