#pragma once

#include "worldBase.h"
#include "componentMask.h"

class World;
class Entity
{
	friend World;

private:
	Component* components[1024];
	ComponentMask componentMask;

public:
	WorldBase* world = nullptr;

	~Entity()
	{
		for (size_t i = 0; i < ComponentCounter::count; i++)
		{
			delete components[i];
		}
	}

	template <typename ComponentType>
	bool hasComponent()
	{
		ComponentMask mask;
		mask.addComponentBit<ComponentType>();
		return componentMask.matches(mask);
	}

	template <typename ComponentType>
	void addComponent(ComponentType* component)
	{
		auto componentBit = GetComponentBit<ComponentType>();
		components[componentBit] = static_cast<Component*>(component);
		world->addEntityComponent(this, componentBit);
	}

	template <typename ComponentType>
	ComponentType* getComponent()
	{
		return static_cast<ComponentType*>(components[GetComponentBit<ComponentType>()]);
	}

	template <typename ComponentType>
	void removeComponent()
	{
		auto componentBit = GetComponentBit<ComponentType>();
		delete static_cast<ComponentType*>(components[componentBit]);
		components[componentBit] = nullptr;
		world->removeEntityComponent(this, componentBit);
	}
};
