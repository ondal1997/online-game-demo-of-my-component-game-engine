#pragma once

#include "../engine/engine.h"

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

class FrictionSystem : public System
{
public:
	FrictionSystem(float value = 0.0625f) : value(value)
	{
		addComponentType<Velocity>();
	}
	float value;

	void update()
	{
		for (auto entity : entities)
		{
			auto velocity = entity->getComponent<Velocity>();

			velocity->vx *= (1.0f - value);
			velocity->vy *= (1.0f - value);
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

class HpComponent : public Component
{
public:
	HpComponent(float value, float maxValue) : value(value), maxValue(maxValue), delta(0)
	{
	}

	float value;
	float maxValue;
	float delta;
};

class MpComponent : public Component
{
public:
	MpComponent(float value, float maxValue) : value(value), maxValue(maxValue), delta(0)
	{
	}

	float value;
	float maxValue;
	float delta;
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

			if (maxy < position->y)
				maxy = position->y;
		}

		float left = minx - padding;
		float right = maxx + padding;

		targetX = left;
		targetWidth = right - left;

		float top = miny - padding;
		float bottom = maxy + padding;
		//float yBaseWidth = (height / renderHeight * renderWidth);

		//if (targetWidth >= yBaseWidth)
		//{
		float xBaseHeight = (targetWidth / renderWidth * renderHeight);
		targetHeight = xBaseHeight;
		targetY = (top + (bottom - top) / 2) - (xBaseHeight * 0.75);
		//}
		//else
		//{
		//	targetWidth = yBaseWidth;
		//	targetX = left + (right - left) / 2 - yBaseWidth / 2;
		//}

		x += (targetX - x) / 20.0f;
		width += (targetWidth - width) / 20.0f;
		y += (targetY - y) / 20.0f;
		height += (targetHeight - height) / 20.0f;
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

class Sound : public Component
{
public:
	Sound(const char* key = nullptr) : key(key)
	{
	}

	const char* key;
};
