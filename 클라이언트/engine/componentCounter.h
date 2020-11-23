#pragma once

struct ComponentCounter
{
	static size_t count;
};

size_t ComponentCounter::count = 0;

template <typename ComponentType>
struct ComponentManager
{
    static inline size_t getBit()
    {
        static size_t bit = ComponentCounter::count++;
        return bit;
    }
};

class Component
{
public:
    virtual ~Component()
    {
    }
};

template <typename ComponentType>
size_t GetComponentBit()
{
    return ComponentManager<ComponentType>::getBit();
}
