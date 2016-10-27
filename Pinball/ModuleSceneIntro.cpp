#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "p2DynArray.h"
#include "ModuleWindow.h"
#include "ModulePlayer.h"
#include "p2String.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	ray_on = false;
	sensed = false;

	frog.PushBack({25,181,49,23});
	frog.PushBack({82,181,49,23});
	frog.PushBack({139,181,49,23});
	frog.speed = 0.07;
	frogRect = { 25,181,49,23 };
	
	fairy.PushBack({39,234,13,39});
	fairy.PushBack({80,234,17,40});
	fairy.PushBack({119,234,25,40});
	fairy.speed = 0.05;
	fairyRect = { 39,234,13,39 };

	num1.PushBack({459,25,26,22});
	num1.PushBack({ 459,181,26,22 });
	num1.PushBack({ 458,337,26,22 });
	num1.speed = 0.05;

	num2.PushBack({ 499,35,26,22 });
	num2.PushBack({ 499,280,26,22 });
	num2.PushBack({ 498,436,26,22 });
	num2.speed = 0.05;

	num3.PushBack({ 526,63,26,22 });
	num3.PushBack({ 526,219,26,22 });
	num3.PushBack({ 525,375,26,22 });
	num3.speed = 0.05;

	num4.PushBack({ 526,96,26,22 });
	num4.PushBack({ 526,252,26,22 });
	num4.PushBack({ 525,408,26,22 });
	num4.speed = 0.05;

	num5.PushBack({ 499,124,26,22 });
	num5.PushBack({ 499,280,26,22 });
	num5.PushBack({ 498,436,26,22 });
	num5.speed = 0.05;

	num6.PushBack({ 459,132,26,22 });
	num6.PushBack({ 459,288,26,22 });
	num6.PushBack({ 458,444,26,22 });
	num6.speed = 0.05;

	num7.PushBack({ 417,124,26,22 });
	num7.PushBack({ 417,280,26,22 });
	num7.PushBack({ 416,436,26,22 });
	num7.speed = 0.05;

	num8.PushBack({ 393,96,26,22 });
	num8.PushBack({ 393,252,26,22 });
	num8.PushBack({ 392,408,26,22 });
	num8.speed = 0.05;

	num9.PushBack({ 393,63,26,22 });
	num9.PushBack({ 393,219,26,22 });
	num9.PushBack({ 392,375,26,22 });
	num9.speed = 0.05;

	num10.PushBack({ 417,35,26,22 });
	num10.PushBack({ 417,191,26,22 });
	num10.PushBack({ 416,347,26,22 });
	num10.speed = 0.05;

	numboss.PushBack({ 232,161,105,43});
	numboss.PushBack({ 231,220,105,43});
	numboss.PushBack({ 231,282,105,43 });
	numboss.speed = 0.05;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	changebody = false;
	App->renderer->camera.x = App->renderer->camera.y = 0;

	map = App->textures->Load("Game/Sprites/Empty_map_PNG.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	circle = App->textures->Load("Game/Sprites/Ball_PNG.png"); 

	basic_sprites = App->textures->Load("Game/Sprites/Basic_sprites_PNG.png");

	springle_tex = App->textures->Load("Game/Sprites/springle.png");

	lKiker = App->textures->Load("Game/Sprites/left_kicker.png");

	rKiker = App->textures->Load("Game/Sprites/right_kicker.png");

	//sound load
	game_loop_music = App->audio->PlayMusic("Game/Sounds/main_music.ogg");
	
	kicker_sound = App->audio->LoadFx("Game/Sounds/flipper_sound.wav");
	spaceship_sound = App->audio->LoadFx("Game/Sounds/ovni_sound.wav");
	enemy_hit = App->audio->LoadFx("Game/Sounds/enemy_hit.wav");
	enemy_kill = App->audio->LoadFx("Game/Sounds/enemy_kill.wav");
	king_frog_spawn = App->audio->LoadFx("Game/Sounds/king_frog.wav");
	king_frog_hit = App->audio->LoadFx("Game/Sounds/boss_hit.wav");
	triangle_sound = App->audio->LoadFx("Game/Sounds/triangle_sound.wav");
	win_sound = App->audio->LoadFx("Game/Sounds/win_sound.wav");

	dead = App->physics->CreateRectangleSensor(186, 580, 135, 15);
	dead->listener = this;

	 time = SDL_GetTicks();

	b2Filter b;
	// create filter and spaceship sensor
	//spaceship = App->physics->CreateRectangleSensor(189, 145, 50, 11);
	b.categoryBits = ON;
	b.maskBits = ON;
	spaceship = App->physics->CreateRectangleSensor(189, 145, 50, 11);
	spaceship->listener = this;
	spaceship->body->GetFixtureList()->SetFilterData(b);


	setspaceship = App->physics->CreateRectangleSensor(189, 185, 100, 11);
	setspaceship->listener = this;
	b.categoryBits = OFF;
	b.maskBits = OFF;
	setspaceship->body->GetFixtureList()->SetFilterData(b);

	b.categoryBits = ON;
	b.maskBits = ON;
	//sensor 2
	sensor_2 = App->physics->CreateRectangleSensor(229, 210, 49, 23);
	sensor_2->listener = this;
	sensor_2->body->GetFixtureList()->SetFilterData(b);
	//frog 1
	frog1 = App->physics->CreateRectangleSensor(229, 210, 49, 23);
	frog1->listener = this;
	frog1->body->GetFixtureList()->SetFilterData(b);
	//frog2
	frog2 = App->physics->CreateRectangleSensor(316, 226, 49, 23);
	frog2->listener = this;
	frog2->body->GetFixtureList()->SetFilterData(b);
	//frog3
	frog3 = App->physics->CreateRectangleSensor(271, 262, 49, 23);
	frog3->listener = this;
	frog3->body->GetFixtureList()->SetFilterData(b);
	//fairy1
	fairy1 = App->physics->CreateRectangleSensor(65, 227, 25, 40);
	fairy1->listener = this;
	fairy1->body->GetFixtureList()->SetFilterData(b);
	//fairy2
	fairy2 = App->physics->CreateRectangleSensor(110, 264, 25, 40);
	fairy2->listener = this;
	fairy2->body->GetFixtureList()->SetFilterData(b);
	//fairy3
	fairy3 = App->physics->CreateRectangleSensor(154, 212, 25, 40);
	fairy3->listener = this;
	frog3->body->GetFixtureList()->SetFilterData(b);
	//boss
	b.categoryBits = DISABLE;
	b.maskBits = DISABLE;
	KingFrog = App->physics->CreateRectangleSensor(189, 192, 107, 47);
	KingFrog->listener = this;
	KingFrog->body->GetFixtureList()->SetFilterData(b);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(basic_sprites);
	App->textures->Unload(circle);
	App->textures->Unload(map);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	App->renderer->Blit(map, 0, 0);
	//DRAW ALL IN MAP
	// animations in number
	SDL_Rect number;
	if (n1 == true)
	{
		time += 30;
		number = animation_num1->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 171, 282, &number);
	}
	if (n2 == true)
	{
		number = animation_num2->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 211, 292, &number);
	}
	
	if (n3 == true)
	{
		number = animation_num3->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 238, 320, &number);
	}
	if (n4 == true)
	{
		number = animation_num4->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 238, 353, &number);
	}
	if (n5 == true)
	{
		number = animation_num5->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 211, 381, &number);
	}
	if (n6 == true)
	{
		number = animation_num6->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 171, 389, &number);
	}
	if (n7 == true)
	{
		number = animation_num7->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 129, 381, &number);
	}
	if (n8 == true)
	{
		number = animation_num8->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 105, 353, &number);
	}
	if (n9 == true)
	{
		number = animation_num9->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 105, 321, &number);
	}
	if (n10 == true)
	{
		number = animation_num10->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 129, 292, &number);
	}

		//frog
	
	SDL_Rect fr = animation_frog->GetCurrentFrame();
	if (frog1t == true)
	{
		App->renderer->Blit(basic_sprites, 205, 199, &fr);
	}
	if(frog2t == true)
	{
		App->renderer->Blit(basic_sprites, 292, 215, &fr);
	}
	if (frog3t == true)
	{
		App->renderer->Blit(basic_sprites, 247, 251, &fr);
	}
	
	//boss
	if (nboss == true)
	{
		SDL_Rect bss = animation_numboss->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 138, 167, &bss);
	}

	//fairy
	SDL_Rect fa = animation_fairy->GetCurrentFrame();
	if (fairy1t == true) //if false, they don't blit
	{
		App->renderer->Blit(basic_sprites, 53, 209, &fa);
	}
	if (fairy2t == true)
	{
		App->renderer->Blit(basic_sprites, 98, 245, &fa);
	}
	if (fairy3t == true)
	{
		App->renderer->Blit(basic_sprites, 141, 193, &fa);
	}
	
	createfairy = false;

	//springy
	int sPositionX, sPositionY;
	App->physics->springy->GetPosition(sPositionX, sPositionY);
	App->renderer->Blit(springle_tex, sPositionX, sPositionY, NULL, 1.0f);

	//kickers
	App->renderer->Blit(lKiker, 100, 447, NULL, 1.0f, App->physics->l_kicker->GetRotation(), 10, 10); //I finally fixed it, NO TOUCHY! >:c
	App->renderer->Blit(rKiker, 200, 447, NULL, 1.0f, App->physics->r_kicker->GetRotation(), 62, 9); //Same, NO TOUCHY! >:c

	App->physics->springy->body->ApplyForce({ 0,-10 }, { 0, 0 }, true);


	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->physics->springy->body->ApplyForce({ 0,18 }, { 0, 0 }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		App->physics->springy->body->ApplyForce({ 0,-180 }, { 0, 0 }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->physics->r_kicker->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			App->physics->r_kicker->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->l_kicker->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			App->physics->l_kicker->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN))
	{
		App->audio->PlayFx(kicker_sound);
	}


	//in case of death
	if ((App->player->lifes == 3 || App->player->lifes == 2 || App->player->lifes == 1 || App->player->lifes == 0) || (nboss ==true || n1 == true))
	{
		if (App->player->lifes == 0)
		{
			App->player->createball = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			App->player->lifes = 3;
			App->player->createball = true;
			frog1t = true, frog2t = true, frog3t = true;
			fairy1t = true, fairy2t = true, fairy3t = true;
			n1 = false, n2 = false, n3 = false, n4 = false, n5 = false, n6 = false, n7 = false, n8 = false, n9 = false, n10 = false, nboss = false;
			b2Filter b;
			b.categoryBits = ON;
			b.maskBits = ON | OFF;
			frog1->body->GetFixtureList()->SetFilterData(b);
			frog2->body->GetFixtureList()->SetFilterData(b);
			frog3->body->GetFixtureList()->SetFilterData(b);
			fairy1->body->GetFixtureList()->SetFilterData(b);
			fairy2->body->GetFixtureList()->SetFilterData(b);
			fairy3->body->GetFixtureList()->SetFilterData(b);

			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			KingFrog->body->GetFixtureList()->SetFilterData(b);
			victory = false;
			score = 0;
		}

		
	}
	ScoreNumber(score);

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();


	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY())) //blits texture only if mouse inside shape
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}
	//print score
	
	if (victory == true && n10==true)
	{
		
		App->audio->PlayFx(win_sound);
		p2SString title("YOU WIN!!! Press B to play again~ , Score: %i Global Score: %i", score, globalScore);
		App->window->SetTitle(title.GetString());
	}

	if( n10 == true && victory == false) //mision10 (last)
	{
		p2SString title("Score: %i Global Score: %i Mission: die, and kill the boss when it appears", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n9 == true && n10 == false) //mision9
	{
		p2SString title("Score: %i Global Score: %i Mission: Use the ovni", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n8 == true && n9 == false) //mision9
	{
		p2SString title("Score: %i Global Score: %i Mission: Score > 3000", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n7 == true && n8 == false) //mision8
	{
		p2SString title("Score: %i Global Score: %i Mission: kill 2 fairies", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n6 == true && n7 == false )// mision 7
	{
		p2SString title("Score: %i Global Score: %i Mission: kill the rightmost fairy and frog", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n5 == true  && n6 == false )// mision6
	{
		p2SString title("Score: %i Global Score: %i Mision: kill the middle fairy and frog" , score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n4 == true && n5 == false)// mision5
	{
		p2SString title("Score: %i Global Score: %i Mission: score > 1000", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n3 == true && n4 == false)//mision4
	{
		p2SString title("Score: %i Global Score: %i Mission: kill the leftmost fairy and frog", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n2 == true && n3 == false)//mision3
	{
		p2SString title("Score: %i Global Score: %i Mission: score > 500", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n1 == true && n2 == false)//mision2
	{
		p2SString title("Score: %i Global Score: %i Mission: destroy all fairies", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	else if (n1 == false)//mision1
	{
		p2SString title("Score: %i Global Score: %i Mission: destroy all frogs", score, globalScore);
		App->window->SetTitle(title.GetString());
	}
	

	if (score >= globalScore)
	{
		globalScore = score;
	}
	if (changebody == true)
	{
		App->player->player->body->SetType(b2_staticBody);
		changebody = false;

	}
	time = SDL_GetTicks();
	if ((time > lastTime + 3000) && App->player->player->body->GetType() == b2_staticBody)
	{
		App->player->player->body->SetType(b2_dynamicBody);
		App->player->player->body->ApplyForceToCenter({-50,-50}, true);
	}
	
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
	if (bodyA != nullptr && bodyB != nullptr)
	{
		b2Filter b;
		b.categoryBits = ON;
		b.maskBits = ON;
		if (bodyA->body == App->player->player->body && bodyB->body == spaceship->body)
		{
			b.categoryBits = OFF;
			b.maskBits = OFF |ON;
			LOG("COLLIDE");
			App->player->player->body->GetFixtureList()->SetFilterData(b);

			lastTime = SDL_GetTicks();
			changebody = true;
			
			App->audio->PlayFx(spaceship_sound);
		}
		if (bodyA->body == App->player->player->body && bodyB->body == setspaceship->body)
		{
			b.categoryBits = ON;
			b.maskBits = ON | OFF;
			LOG("COLLIDE 2");
			App->player->player->body->GetFixtureList()->SetFilterData(b);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == dead->body)
		{
			
			App->player->createball = true;
			App->player->lifes -= 1;

		}

		if (bodyA->body == App->player->player->body && bodyB->body == frog1->body) //right frog hit
		{
			if (b.categoryBits == ON) score += 50;
			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			frog1->body->GetFixtureList()->SetFilterData(b);
			frog1t=false;

			

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == frog2->body) //middle frog hit
		{
			if (b.categoryBits == ON) score += 50;
			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			frog2->body->GetFixtureList()->SetFilterData(b);
			frog2t = false;

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
			
		}

		if (bodyA->body == App->player->player->body && bodyB->body == frog3->body) //left frog hit
		{
			if (b.categoryBits == ON) score += 50;
			b.categoryBits = DISABLE;
			
			b.maskBits = DISABLE;
			frog3->body->GetFixtureList()->SetFilterData(b);
			frog3t = false;

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == fairy1->body)
		{
			if (b.categoryBits == ON) score += 30;
			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			fairy1->body->GetFixtureList()->SetFilterData(b);
			fairy1t = false;

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == fairy2->body)
		{
			if (b.categoryBits == ON) score += 30;
			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			fairy2->body->GetFixtureList()->SetFilterData(b);
			fairy2t = false;

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == fairy3->body)
		{
			if (b.categoryBits == ON) score += 30;
			b.categoryBits = DISABLE;
			b.maskBits = DISABLE;
			fairy3->body->GetFixtureList()->SetFilterData(b);
			fairy3t = false;

			App->audio->PlayFx(enemy_hit);
			App->audio->PlayFx(enemy_kill);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == App->physics->triangle_left->body)
		{
			App->player->player->body->ApplyForceToCenter({ 20, -100 }, true);
			App->audio->PlayFx(triangle_sound);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == App->physics->triangle_right->body)
		{
			App->player->player->body->ApplyForceToCenter({ 20, -100 }, true);
			App->audio->PlayFx(triangle_sound);
		}

		if (bodyA->body == App->player->player->body && bodyB->body == KingFrog->body)
		{
			frog1t = false, frog2t = false, frog3t = false;
			fairy1t = false, fairy2t = false, fairy3t = false;

			boss_hits++;
			if (b.categoryBits == ON) score += 100;
			App->player->player->body->ApplyForceToCenter({ 20, 40 }, true);
			if (boss_hits == 3)
			{
				b.categoryBits = DISABLE;
				b.maskBits = DISABLE;
				KingFrog->body->GetFixtureList()->SetFilterData(b);
				nboss = false, victory =true;
				//n1 = false, n2 = false, n3 = false, n4 = false, n5 = false, n6 = false, n7 = false, n8 = false, n9 = false, n10 = false;
			}

			App->audio->PlayFx(king_frog_hit);
		}

	}
}
void ModuleSceneIntro::ScoreNumber(int score)
{
	b2Filter b;
	if (frog1t == false && frog2t == false && frog3t == false)
	{
		n1 = true;
		score += 50;
		frog1t = true, frog2t = true, frog3t = true;
		b.categoryBits = ON;
		b.maskBits = ON | OFF;
		frog1->body->GetFixtureList()->SetFilterData(b);
		frog2->body->GetFixtureList()->SetFilterData(b);
		frog3->body->GetFixtureList()->SetFilterData(b);
	}

	if (fairy1t == false && fairy2t == false && fairy3t == false && n1 == true)
	{
		n2 = true;
		fairy1t = true, fairy2t = true, fairy3t = true;
		bosscount++;
		b.categoryBits = ON;
		b.maskBits = ON | OFF;
		fairy1->body->GetFixtureList()->SetFilterData(b);
		fairy2->body->GetFixtureList()->SetFilterData(b);
		fairy3->body->GetFixtureList()->SetFilterData(b);
	}

	if (score > 500 && n2 == true )
	{
		n3 = true;
	}

	if (fairy1t == false && frog1t == false && n3 == true)
	{
		n4 = true;
	}
	
	if (score > 1000 && n4 == true)
	{
		n5 = true;
	}
	if (fairy2t == false && frog2t == false && n5 == true)
	{
		n6 = true;
	}

	if (fairy3t == false && frog3t == false && n6 == true)
	{
		n7 = true;
	}
	
	if ( (fairy1t == false && fairy2t == false  || fairy2t == false && fairy3t == false || fairy1t == false && fairy3t == false) && n7 == true)
	{
		n8 = true;
	}

	if (n8 == true && score > 3000)
	{
		n9 = true;
	}
	if (changebody == true && n9 == true)
	{
		n10 = true;
	
	}
	if (n10 == true && App->player->createball == true)
	{
		nboss = true;
		b2Filter b;
		b.categoryBits = ON;
		b.maskBits = ON | OFF;
		KingFrog->body->GetFixtureList()->SetFilterData(b);
		App->audio->PlayFx(king_frog_spawn);
	}
	if (nboss == true && bosst == false)
	{
		end == true;
	}
}