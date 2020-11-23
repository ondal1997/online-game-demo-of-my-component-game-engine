#pragma once

#include "core.h"

#include <fmod.hpp>

#pragma comment(lib, "fmod64_vc.lib")

#include <map>

extern HWND hWnd;

class Audio : public System
{
public:
	FMOD::System* system;
	FMOD::Channel* channel;
	std::map<const char*, FMOD::Sound*> map;

	Audio() : channel(nullptr)
	{
		addComponentType<Sound>();

		FMOD::System_Create(&system);
		system->init(8, FMOD_INIT_NORMAL, 0);
	}

	void loadSound(const char* path, bool isLoop, const char* key)
	{
		if (map.find(key) == map.end())
			system->createSound(path, isLoop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT, 0, &map[key]);
	}

	void playSound(const char* key, float pitch = 1.0f)
	{
		system->playSound(map[key], 0, false, &channel);
		channel->setPitch(pitch);
	}

	~Audio()
	{
		for (auto it = map.begin(); it != map.end(); it++)
		{
			it->second->release();
		}

		system->close();
		system->release();
	}

	void update()
	{
		static int count = 1;
		system->update();

		for (auto entity : entities)
		{
			auto sound = entity->getComponent<Sound>();

			if (sound->key == nullptr)
				continue;

			playSound(sound->key);
			sound->key = nullptr;

			char buf[10] = {};
			count++;
			itoa(count, buf, 10);
			SetWindowTextA(hWnd, buf);
		}
	}
};
