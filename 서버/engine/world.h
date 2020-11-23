#pragma once

#include "worldBase.h"
#include "entity.h"
#include "system.h"
#include "componentMask.h"

#include <vector>
#include <map>

class World : public WorldBase
{
private:
	std::vector<Entity*> entities;
	std::map<Entity*, size_t> entityIndices;
	std::vector<System*> systems;

	void addEntityComponent(Entity* entity, size_t componentBit)
	{
		ComponentMask oldMask = entity->componentMask;
		entity->componentMask.addComponentBit(componentBit);
		updateEntityComponent(entity, oldMask);
	}

	void removeEntityComponent(Entity* entity, size_t componentBit)
	{
		ComponentMask oldMask = entity->componentMask;
		entity->componentMask.removeComponentBit(componentBit);
		updateEntityComponent(entity, oldMask);
	}

	void updateEntityComponent(Entity* entity, ComponentMask& oldMask)
	{
		for (auto system : systems)
		{
			if (entity->componentMask.isNewMatch(oldMask, system->componentMask))
			{
				system->addEntity(entity);
			}
			else if (entity->componentMask.isNoLongerMatched(oldMask, system->componentMask))
			{
				system->removeEntity(entity);
			}
		}
	}

public:
	~World()
	{
		for (auto entity : entities)
		{
			delete entity;
		}
	}

	Entity* createEntity()
	{
		auto entity = new Entity;
		entityIndices[entity] = entities.size();
		entities.push_back(entity);
		entity->world = this;
		return entity;
	}

	void destroyEntity(Entity* entity)
	{
		if (entities.back() == entity)
		{
			entities.pop_back();
			entityIndices.erase(entity);
		}
		else
		{
			entities[entityIndices[entity]] = entities.back();
			entityIndices[entities.back()] = entityIndices[entity];
			entities.pop_back();
			entityIndices.erase(entity);
		}

		for (auto system : systems)
		{
			system->removeEntity(entity);
		}

		// delete entity;
	}

	void addSystem(System* system)
	{
		systems.push_back(system);
		system->world = this;
	}
};
