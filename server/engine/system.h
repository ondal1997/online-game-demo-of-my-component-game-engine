#pragma once

#include "worldBase.h"
#include "entity.h"
#include "componentMask.h"

#include <vector>
#include <map>

class World;
class System
{
	friend World;

private:
	std::map<Entity*, size_t> entityIndices;
	ComponentMask componentMask;

	void addEntity(Entity* entity)
	{
		entityIndices[entity] = entities.size();
		entities.push_back(entity);
	}

	void removeEntity(Entity* entity)
	{
		auto it = entityIndices.find(entity);
		if (it == entityIndices.end())
			return;

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
	}

public:
	WorldBase* world = nullptr;
	std::vector<Entity*> entities;

	template <typename ComponentType>
	void addComponentType()
	{
		componentMask.addComponentBit<ComponentType>();
	}
	
	virtual void update()
	{
	}
};
