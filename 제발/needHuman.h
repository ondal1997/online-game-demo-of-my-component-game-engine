#pragma once

#include "core.h"
#include "human.h"

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

class HpBar : public Component
{
public:
	HpBar(Entity* target, float effect = 0.0f, float value = 0.0f) : target(target), effect(effect), value(value)
	{
	}
	Entity* target;
	float effect;
	float value;
};

class MpBar : public Component
{
public:
	MpBar(Entity* target, float value = 0.0f) : target(target), value(value)
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
