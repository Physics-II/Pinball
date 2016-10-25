#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "Module.h"

struct SDL_Texture;
class PhysBody;



class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;

	p2List<PhysBody*> chains;

	PhysBody* sensor;
	bool sensed;
	SDL_Texture* springle_tex;
	SDL_Texture *lKiker, *rKiker;
	SDL_Texture* map;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* basic_sprites;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
	bool victory;
	bool createfrog=true, createfairy= true;
	//For animation the frog
	Animation frog;
	Animation* animation_frog = &frog;
	SDL_Rect frogRect;
	//For the fairy
	Animation fairy;
	Animation* animation_fairy = &fairy;
	SDL_Rect fairyRect;
	//Ball LIFES
	SDL_Rect ballLife;
	//BALL (PLAYER)
	PhysBody* dead , *spaceship ,*fairy1,*fairy2, *fairy3 ,*frog1,*frog2,*frog3;


	//Score
	int score = 0, globalScore = 0;

	
};
