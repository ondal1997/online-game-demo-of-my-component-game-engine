#pragma once

#include "engine.h"

// Position
// Image

class HpBarController : public Component // ���ֿ��� �ο��ϸ� ������ HpBar ��ƼƼ�� ������ �� �ִ�.
{
public:
	Entity* entity;
};

class MpBarController : public Component // ���ֿ��� �ο��ϸ� ������ MpBar ��ƼƼ�� ������ �� �ִ�.
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
		�ؽ�ó�� hp_back�� �׸���;
		target = value / max;
		effect += (target - effect) / 2.0f;
		�ؽ�ó�� hp_effect�� effect ��ŭ �׸���;
		�ؽ�ó�� hp_value�� target ��ŭ �׸���;
	}
};

class MpBarSystem : public System
{
public:
	void update()
	{
		for (int i = 0; i < 5; i++)
		�ؽ�ó�� mp_back�� i��ġ�� �׸���;

		for (int i = 0; i < 5; i++)
		{
			if (value - i < 1.0f)
			{
				�ؽ�ó�� mp_value�� (value - i / 1.0f) ��ŭ i ��ġ�� �׸���;
				break;
			}
			�ؽ�ó�� mp_effect�� i��ġ�� �׸���;
		}
	}
};
