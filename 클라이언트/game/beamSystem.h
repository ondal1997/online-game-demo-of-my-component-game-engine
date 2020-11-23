#pragma once

#include "beam.h"
#include "human.h"

class BeamSystem : public System
{
public:
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

				if (targetPos->x < pos->x - 50)
					continue;
				if (targetPos->x > pos->x + 50)
					continue;
				if (targetPos->y < pos->y - 50)
					continue;
				if (targetPos->y > pos->y + 50)
					continue;

				target->getComponent<Sound>()->key = "pak";

				//공격 대상
				beam->friends.insert(target);

				auto direction = entity->getComponent<Direction>();

				auto hp = target->getComponent<HpComponent>();
				auto velocity = target->getComponent<Velocity>();

				hp->delta -= 1.0f;
				velocity->vx += direction->value * 10.0f;
				velocity->vy -= 10.0f;

				// 반달 생성
				auto particle = entity->world->createEntity();
				particle->addComponent(new Particle(particle, "hitBeam"));
				particle->addComponent(new Position(targetPos->x, targetPos->y - 100.0f));
				particle->addComponent(new Direction(direction->value));
				particle->addComponent(new Graphic);

				// 피 생성
				for (int i = 0; i < 10; i++)
				{
					auto blood = entity->world->createEntity();
					blood->addComponent(new Particle(blood, "blood"));
					blood->addComponent(new Position(targetPos->x, targetPos->y - 100.0f));
					blood->addComponent(new Direction(direction->value));
					// 속력은 내부에서 줌
					blood->addComponent(new DeltaPosition);
					blood->addComponent(new GravityComponent);
					blood->addComponent(new Graphic);
				}

				//스파크 생성
				for (int i = 0; i < 10; i++)
				{
					auto spark = entity->world->createEntity();
					spark->addComponent(new Particle(spark, "spark"));
					spark->addComponent(new Position(targetPos->x, targetPos->y));
					spark->addComponent(new Direction(direction->value));
					// 속력은 내부에서 줌
					spark->addComponent(new DeltaPosition);
					spark->addComponent(new GravityComponent);
					spark->addComponent(new Graphic);
				}
			}
		}
	}
};
