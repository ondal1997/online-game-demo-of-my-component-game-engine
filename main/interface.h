#pragma once

#include "engine.h"

// Position
// Image

class HpBarController : public Component // 유닛에게 부여하면 유닛이 HpBar 엔티티에 접근할 수 있다.
{
public:
	Entity* entity;
};

class MpBarController : public Component // 유닛에게 부여하면 유닛이 MpBar 엔티티에 접근할 수 있다.
{
public:
	Entity* entity;
};

class HpBar : public Component
{
public:
	float value;
	float max;

	float effect;
};

class MpBar : public Component
{
public:
	float value;
};

class HpBarSystem : public System
{
public:
	void update()
	{
		텍스처에 hp_back을 그린다;
		target = value / max;
		effect += (target - effect) / 2.0f;
		텍스처에 hp_effect를 effect 만큼 그린다;
		텍스처에 hp_value를 target 만큼 그린다;
	}
};

class MpBarSystem : public System
{
public:
	void update()
	{
		for (int i = 0; i < 5; i++)
		텍스처에 mp_back을 i위치에 그린다;

		for (int i = 0; i < 5; i++)
		{
			if (value - i < 1.0f)
			{
				텍스처에 mp_value를 (value - i / 1.0f) 만큼 i 위치에 그린다;
				break;
			}
			텍스처에 mp_effect를 i위치에 그린다;
		}
	}
};
