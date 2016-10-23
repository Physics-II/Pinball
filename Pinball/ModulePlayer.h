#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Application.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	PhysBody* player;

	int lifes = 3;
	SDL_Texture* graphics = nullptr;

	SDL_Texture* life;

	int speed;
	int bar_movement;
	bool dead = true;
	bool createball = true;

public:

	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	


};