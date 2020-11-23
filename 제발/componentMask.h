#pragma once

#include "componentCounter.h"

struct ComponentMask
{
	unsigned int mask[32];

	template <typename ComponentType>
	void addComponentBit()
	{
		addComponentBit(GetComponentBit<ComponentType>());
	}

	template <typename ComponentType>
	void removeComponentBit()
	{
		removeComponentBit(GetComponentBit<ComponentType>());
	}

	void addComponentBit(size_t bit)
	{
		mask[bit / 32] |= (1 << bit % 32);
	}

	void removeComponentBit(size_t bit)
	{
		mask[bit / 32] &= ~(1 << bit % 32);
	}

	bool matches(ComponentMask& systemMask)
	{
		for (int i = 0; i < 32; i++)
		{
			if ((mask[i] & systemMask.mask[i]) != systemMask.mask[i])
				return false;
		}
		return true;
	}

	bool isNewMatch(ComponentMask& oldMask, ComponentMask& systemMask)
	{
		return matches(systemMask) && !oldMask.matches(systemMask);
	}

	bool isNoLongerMatched(ComponentMask& oldMask, ComponentMask& systemMask)
	{
		return oldMask.matches(systemMask) && !matches(systemMask);
	}
};
