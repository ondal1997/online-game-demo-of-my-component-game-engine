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

			if (position->y >= 1000.0f)
			{
				position->y = 1000.0f;
				velocity->vy = 0.0f;

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
			else if (hpComponent->value > hpComponent->maxValue)
				hpComponent->value = hpComponent->maxValue;

			if (hpComponent->value == 0.0f && entity->hasComponent<Human>())
			{
				auto human = entity->getComponent<Human>();
				if (human->state != "die")
				{
					human->init("die");
				}
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
			else if (mpComponent->value > mpComponent->maxValue)
				mpComponent->value = mpComponent->maxValue;
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
	MpBar(Entity* target, float effect = 0.0f, float value = 0.0f) : target(target), effect(effect), value(value)
	{
	}
	Entity* target;
	float effect;
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

			hpBar->value = hp->value / hp->maxValue;
			hpBar->effect += (hpBar->value - hpBar->effect) / 80.0f;
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

			mpBar->value = mp->value;
			mpBar->effect += (mp->value - mpBar->effect) / 3.0f;
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

		if (leftHuman->state == "die" && leftHuman->tick == 160)
		{
			if (rightHuman->state == "die" && rightHuman->tick == 160)
			{
				*flag = 2;
			}
			else
			{
				*flag = 1;
			}
		}
		else if (rightHuman->state == "die" && rightHuman->tick == 160)
		{
			*flag = 0;
		}
	}
};
