#include "engine.h"

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

class Move : public Component
{
public:
	Move(float value = 0.0f) : value(value)
	{
	}

	float value;
};

class Jump : public Component
{
public:
	Jump(float value = 0.0f) : value(value)
	{
	}

	float value;
};

class Health : public Component
{
public:
	Health(float value = 0.0f, float max = 0.0f) : value(value), max(max)
	{
	}

	float value;
	float max;
};

class HitBox : public Component
{
public:
	HitBox(float width, float height) : width(width), height(height)
	{
	}

	float width;
	float height;
};

class Team : public Component
{
public:
	Team(int value) : value(value)
	{
	}

	int value;
};

class Image : public Component
{
public:
	Image() : key(nullptr)
	{
	}

	const char* key;
};

class Sound : public Component
{
public:
	Sound() : key(nullptr)
	{
	}

	const char* key;
};

#include <vector>

class Hit : public Component
{
public:
	Hit(size_t value, std::vector<Entity*> exception) : value(value), exception(exception)
	{
	}

	size_t value;
	std::vector<Entity*> exception;
};

class Fire : public Component
{
public:
	Fire()
	{
	}
};

class Human : public Component
{
public:
	Human(Entity* entity, const char* state = "idle", size_t tick = 0) : entity(entity), state(state), tick(tick), horizontal(0), vertical(0)
	{
	}

	Entity* entity;
	const char* state;
	size_t tick;
	int horizontal;
	int vertical;

	void ready()
	{
		auto health = *entity->getComponent<Health>();
		auto image = *entity->getComponent<Image>();
		auto position = *entity->getComponent<Position>();
		auto direction = *entity->getComponent<Direction>();

		if (state == "idle")
		{
			if (health.value == 0)
			{
				state = "die";
				tick = 0;
				ready();
				return;
			}

			if (position.y < 0)
			{
				state = "air";
				tick = 0;
				ready();
				return;
			}

			if (vertical == -1)
			{
				state = "jump";
				tick = 0;
				ready();
				return;
			}

			if (horizontal)
			{
				direction = horizontal;
				state = "move";
				tick = 0;
				ready();
				return;
			}

			if (tick < 20)
			{
				image.key = "human_idle0";
			}
			else if (tick < 40)
			{
				image.key = "human_idle1";
			}
		}
		else if (state == "move")
		{
			if (health.value == 0)
			{
				state = "die";
				tick = 0;
				ready();
				return;
			}

			if (position.y < 0)
			{
				state = "air";
				tick = 0;
				ready();
				return;
			}

			if (vertical == -1)
			{
				state = "jump";
				tick = 0;
				ready();
				return;
			}

			/*if (!horizontal)
			{
				state = "move";
				tick = 0;
				ready();
				return;
			}*/

			if (tick < 20)
			{
				image.key = "human_move0";
			}
			else if (tick < 40)
			{
				image.key = "human_move1";
			}
		}
		else if (state == "air")
		{
			if (health.value == 0)
			{
				state = "die";
				tick = 0;
				ready();
				return;
			}

			if (position.y == 0)
			{
				state = "idle";
				tick = 0;
				ready();
				return;
			}

			image.key = "human_air0";
		}
		else if (state == "jump")
		{
			if (health.value == 0)
			{
				state = "die";
				tick = 0;
				ready();
				return;
			}

			if (position.y < 0)
			{
				state = "air";
				tick = 0;
				ready();
				return;
			}

			if (tick < 20)
			{
				image.key = "human_jump0";
			}
			else if (tick < 40)
			{
				image.key = "human_jump1";
			}
		}
		else if (state == "die")
		{
			if (tick < 20)
			{
				image.key = "human_die0";
			}
			else if (tick < 40)
			{
				image.key = "human_die1";
			}
			else if (tick < 60)
			{
				image.key = "human_die2";
			}
			else if (tick < 80)
			{
				image.key = "human_die3";
			}
		}
		else if (state == "attack")
		{
			if (health.value == 0)
			{
				state = "die";
				tick = 0;
				ready();
				return;
			}

			if (tick == 60)
			{
				// ���̾���̺ꤻ ����, ready() �������ֱ��;
			}

			if (tick < 20)
			{
				image.key = "human_attack0";
			}
			else if (tick < 40)
			{
				image.key = "human_attack1";
			}
			else if (tick == 40) // case
			{
				image.key = "human_attack1";

				// y == 0 ? tick = 60; ready(); return;
			}
			else if (tick < 80)
			{
				image.key = "human_attack2";
			}
			else if (tick < 100)
			{
				image.key = "human_attack3";
			}
		}
	}

	void input(unsigned int key, bool isDown)
	{
		switch (key)
		{
		case 0:
		case 1:
			if (state == "idle")
			{
				if (horizontal)
				{
					auto direction = *entity->getComponent<Direction>();
					direction = horizontal;
					state = "move";
					tick = 0;
					ready();
					return;
				}
			}
			else if (state == "move")
			{
				if (horizontal)
				{
					auto direction = *entity->getComponent<Direction>();
					direction.value = horizontal;
				}
				else
				{
					state = "idle";
					tick = 0;
					ready();
					return;
				}
			}
			else if (state == "jump")
			{
				if (horizontal)
				{
					auto direction = *entity->getComponent<Direction>();
					direction.value = horizontal;
				}
			}
			break;
		case 2:
		case 3:
			break;
		case 4:
			if (isDown)
			{
				if (state == "idle" || state == "move" || state == "air")
				{
					state = "attack";
					tick = 0;
					ready();
					return;
				}
			}
			break;
		}
	}

	void action()
	{
		if (state == "idle")
		{
			tick++;
			if (tick == 40)
				tick = 0;
		}
		else if (state == "move")
		{
			tick++;
			if (tick == 40)
				tick = 0;

			auto deltaPosition = *entity->getComponent<DeltaPosition>();
			auto move = *entity->getComponent<Move>();
			auto direction = *entity->getComponent<Direction>();

			deltaPosition.dx += move.value * direction.value;
		}
		else if (state == "air")
		{
			auto deltaPosition = *entity->getComponent<DeltaPosition>();
			auto move = *entity->getComponent<Move>();

			deltaPosition.dx += move.value * horizontal;
		}
		else if (state == "jump")
		{
			tick++;
			if (tick == 40)
			{
				auto velocity = *entity->getComponent<Velocity>();
				auto jump = *entity->getComponent<Jump>();

				velocity.vy -= jump.value;

				state = "idle";
				tick = 0;
			}
		}
		else if (state == "die")
		{
			tick++;
			if (tick == 80)
				tick--; // ;)
		}
		else if (state == "attack")
		{
			tick++;
			if (tick == 41)
			{
				tick--;
			}
			else if (tick == 80)
			{
				// ���� �ۿ�
			}
			else if (tick == 100)
			{
				state = "idle";
				tick = 0;
			}
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Gravity : public System
{
public:
	Gravity(float value = 10.0f) : value(value)
	{
		addComponentType<Velocity>();
	}
	float value;

	void update()
	{
		for (auto entity : entities)
		{
			auto velocity = *entity->getComponent<Velocity>();

			velocity.vy += value;
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
			auto position = *entity->getComponent<Position>();
			auto velocity = *entity->getComponent<Velocity>();
			auto deltaPosition = *entity->getComponent<DeltaPosition>();

			deltaPosition.dx += velocity.vx;
			deltaPosition.dy += velocity.vy;

			position.x += deltaPosition.dx;
			position.y += deltaPosition.dy;

			deltaPosition.dx = 0.0f;
			deltaPosition.dy = 0.0f;
		}
	}
};

class LandInspection : public System
{
public:
	LandInspection(float value = 0.25f) : value(value)
	{
		addComponentType<Position>();
		addComponentType<Velocity>();
	}
	float value;

	void update(int dt)
	{
		for (auto entity : entities)
		{
			auto position = *entity->getComponent<Position>();
			auto velocity = *entity->getComponent<Velocity>();

			if (position.y >= 0.0f)
			{
				position.y = velocity.vy = 0.0f;
			}

			if (position.y == 0.0f)
				if (velocity.vx < 0.0f)
					if (velocity.vx >= -value)
						velocity.vx = 0.0f;
					else
						velocity.vx += value;
				else
					if (velocity.vx <= value)
						velocity.vx = 0.0f;
					else
						velocity.vx -= value;
		}
	}
};

#include <SDL.h>

#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2main")

#include <SDL_image.h>

#pragma comment(lib, "SDL2_image")

#include <map>

void stretchTextureEx(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Texture* texture, float angle, SDL_RendererFlip flip = SDL_FLIP_NONE)
{
	//SDL_FLIP_HORIZONTAL SDL_FLIP_VERTICAL

	SDL_Rect src, dst;
	SDL_Point center; src.x = src.y = 0;

	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

	dst.x = x; dst.y = y; dst.w = w; dst.h = h;

	center.x = w / 2;
	center.y = h / 2;

	SDL_RenderCopyEx(renderer, texture, &src, &dst, 0.0, &center, flip);
}

class ImageSystem : public System
{
public:
	ImageSystem(SDL_Window* window) : renderer(SDL_CreateRenderer(window, -1, 0))
	{
		addComponentType<Position>();
		addComponentType<Image>();
	}

	~ImageSystem()
	{
		SDL_DestroyRenderer(renderer);
	}

	SDL_Renderer* renderer;
	//std::map<const char*, Sprite> sprites;

	void update()
	{
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// �ΰ��� �ؽ��� (����, �������̽�)

		for (auto entity : entities)
		{
			if (entity->hasComponent<Direction>())
			{

			}

			auto position = *entity->getComponent<Position>();
			auto image = *entity->getComponent<Image>();

			// TODO

			IMG_LoadTexture(renderer, "path"); // texture*
			//stretchTextureEx();
			//SDL_DestroyTexture();
		}

		// �ΰ��� �ؽ��� �����ؼ� �׸��� (����, �������̽�)

		SDL_RenderPresent(renderer);
	}
};

#include <fmod.hpp>

#pragma comment(lib, "fmod64_vc.lib")

#include <map>

class SoundSystem : public System
{
public:
	FMOD::System* system;
	FMOD::Channel* channel;
	std::map<const char*, FMOD::Sound*> map;

	SoundSystem() : channel(nullptr)
	{
		FMOD::System_Create(&system);
		system->init(64, FMOD_INIT_NORMAL, 0);

		addComponentType<Sound>();
	}

	~SoundSystem()
	{
		for (auto it = map.begin(); it != map.end(); it++)
			it->second->release();

		system->close();
		system->release();
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

	void update()
	{
		for (auto entity : entities)
		{
			auto sound = *entity->getComponent<Sound>();
			playSound(sound.key);
			sound.key = nullptr;
		}
	}
};

// InGame
class HumanSystem : public System
{
public:
	HumanSystem()
	{
		addComponentType<Human>();
	}

	void ready()
	{
		for (auto entity : entities)
		{
			auto human = *entity->getComponent<Human>();

			human.ready();
		}
	}

	void action()
	{
		for (auto entity : entities)
		{
			auto human = *entity->getComponent<Human>();

			human.action();
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SDL.h>
#include <SDL_image.h>

#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2main")
#pragma comment(lib, "SDL2_image")

#include "engine.h"

#include <chrono>

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
	SDL_Event event;

	///////////////////////////////

	World world;

	HumanSystem humanSystem;
	Gravity gravity(9.8f);
	Translate translate;
	LandInspection landInspection(0.5f);
	ImageSystem imageSystem(window); // �̹��� ����
	SoundSystem soundSystem; // ���� ����

	world.addSystem(&humanSystem);
	world.addSystem(&gravity);
	world.addSystem(&translate);
	world.addSystem(&landInspection);
	world.addSystem(&imageSystem);
	world.addSystem(&soundSystem);

	auto human0 = world.createEntity();
	human0->addComponent(new Direction());
	human0->addComponent(new Position());
	human0->addComponent(new Velocity());
	human0->addComponent(new DeltaPosition());
	human0->addComponent(new Move(10.0f));
	human0->addComponent(new Jump(10.0f));
	human0->addComponent(new Health(5.0f, 5.0f));
	human0->addComponent(new Team(0));
	human0->addComponent(new Image());
	human0->addComponent(new Sound());
	human0->addComponent(new Human(human0));

	auto human1 = world.createEntity();
	human1->addComponent(new Direction(-1));
	human1->addComponent(new Position(100.0f, 0.0f));
	human1->addComponent(new Velocity());
	human1->addComponent(new DeltaPosition());
	human1->addComponent(new Move(10.0f));
	human1->addComponent(new Jump(10.0f));
	human1->addComponent(new Health(5.0f, 5.0f));
	human1->addComponent(new Team(1));
	human1->addComponent(new Image());
	human1->addComponent(new Sound());
	human1->addComponent(new Human(human1));

	std::chrono::steady_clock::time_point timeForGame;
	std::chrono::steady_clock::time_point timeForInterface;

	timeForGame = timeForInterface = std::chrono::steady_clock::now();

	std::chrono::milliseconds baseTerm(20);
	std::chrono::milliseconds termForGame(baseTerm);
	std::chrono::milliseconds termForInterface(baseTerm);
	std::chrono::milliseconds zeroSeconds(0);

	bool inputFlag = false;

	while (1)
	{
		auto now = std::chrono::steady_clock::now();

		auto waitingForInterface = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeForInterface) - termForInterface;
		auto waitingForGame = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeForGame) - termForGame;

		int value = waitingForInterface >= zeroSeconds ? waitingForGame > waitingForInterface ? 2 : 1 : waitingForGame >= zeroSeconds ? 2 : 0;

		switch (value)
		{
		case 1: // interface
			imageSystem;
			soundSystem;
			break;
		case 2: // game
			humanSystem;
			gravity;
			translate;
			landInspection;
			break;
		}

		if (value)
		{
			// if (count++ == 500) // 500�� ���� ����Ǿ��°�?
			// ���� �������� �����ϱ�.
			continue;
		}
		else
		{
			// count = 0;
		}

		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				if (event.key.repeat)
					break;
				unsigned int value = 0;
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					value = 0;
					break;
				case SDLK_RIGHT:
					value = 1;
					break;
				case SDLK_UP:
					value = 2;
					break;
				case SDLK_DOWN:
					value = 3;
					break;
				case SDLK_z:
					value = 4;
					break;
				}
				auto human = *human0->getComponent<Human>();
				human.input(value, event.key.state);
				break;
			}
			}
		}
		else
		{
			// ������
		}
	}


	///////////////////////////////

	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
