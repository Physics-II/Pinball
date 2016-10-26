#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "Module.h"
#include <Windows.h>

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
	void ScoreNumber(int x);
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
	bool createfrog=true, createfairy= true, frog1t= true, frog2t = true , frog3t = true,fairy1t=true,fairy2t= true, fairy3t= true;
	bool nboss = false, n1 = false, n2 = false, n3 = false, n4 = false, n5 = false, n6 = false, n7 = false, n8 = false, n9 = false , n10 = false;

	Animation num1, num2, num3, num4, num5, num6, num7, num8, num9, num10, numboss;
	Animation *animation_num1 = &num1, *animation_num2 = &num2, *animation_num3 = &num3, *animation_num4 = &num4, *animation_num5 = &num5, *animation_num6 = &num6, *animation_num7 = &num7, *animation_num8 = &num8, *animation_num9 = &num9, *animation_num10 = &num10, *animation_numboss = &numboss;
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
	PhysBody* dead , *spaceship, *setspaceship,*fairy1,*fairy2, *fairy3 ,*frog1,*frog2,*frog3, *sensor_2 ;
	bool ret = true , changebody; 

	//sounds
	uint kicker_sound;
	uint game_loop_music;
	uint spaceship_sound;
	uint enemy_hit;
	uint enemy_kill;
	uint king_frog_spawn;
	uint king_frog_hit;
	uint triangle_sound;
	uint win_sound;
	
	int time, lastTime;
	
	//Score
	int score = 0, globalScore = 0;
	uint bosscount = 0;
	
};
