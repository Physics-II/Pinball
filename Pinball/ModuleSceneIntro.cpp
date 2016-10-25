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
	
	

	//fairy

	SDL_Rect fa = animation_fairy->GetCurrentFrame();
	if (fairy1t == true)
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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
	{
		App->physics->springy->body->ApplyForce({ 0,18 }, { 0, 0 }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
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
	p2SString title("Score: %i Global Score: %i",score, globalScore);

	App->window->SetTitle(title.GetString());
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
	if (App->player->lifes == 0)
	{
		App->player->createball = false;

		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			App->player->lifes = 3;
			App->player->createball = true;
		}
	}

	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	float x2, y2;

	
	
	App->audio->PlayFx(bonus_fx);

	
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
		if (bodyA->body == App->player->player->body && bodyB->body == frog1->body)
		{
			score += 50;
			b.categoryBits = OFF;
			b.maskBits = ON | OFF;
			frog1t=false;
		}
		if (bodyA->body == App->player->player->body && bodyB->body == frog2->body)
		{
			score += 50;
			b.categoryBits = OFF;
			b.maskBits = ON | OFF;
			frog2t = false;
		}
		if (bodyA->body == App->player->player->body && bodyB->body == frog3->body)
		{
			score += 50;
			b.categoryBits = OFF;
			b.maskBits = ON | OFF;
			frog3t = false;
		}
		if (bodyA->body == App->player->player->body && bodyB->body == fairy1->body)
		{
			score += 30;
			b.categoryBits = ON;
			b.maskBits = ON | OFF;
			fairy1t = false;
		}
		if (bodyA->body == App->player->player->body && bodyB->body == fairy2->body)
		{
			score += 30;
			b.categoryBits = ON;
			b.maskBits = ON | OFF;
			fairy2t = false;
		}
		if (bodyA->body == App->player->player->body && bodyB->body == fairy3->body)
		{
			score += 30;
			b.categoryBits = ON;
			b.maskBits = ON | OFF;
			fairy3t = false;
		}
	}
}
