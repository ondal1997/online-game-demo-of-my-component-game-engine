#pragma once

class Entity;
class WorldBase
{
	friend Entity;

private:
	virtual void addEntityComponent(Entity* entity, size_t componentBit) = 0;
	virtual void removeEntityComponent(Entity* entity, size_t componentBit) = 0;

public:
	virtual Entity* createEntity() = 0;
	virtual void destroyEntity(Entity* entity) = 0;
};
