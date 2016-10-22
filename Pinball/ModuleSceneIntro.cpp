#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"



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

	App->renderer->camera.x = App->renderer->camera.y = 0;

	map = App->textures->Load("Game/Sprites/Empty_map_PNG.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	circle = App->textures->Load("Game/Sprites/Ball_PNG.png"); 

	basic_sprites = App->textures->Load("Game/Sprites/Basic_sprites_PNG.png");

	

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
	//DRAW ALL FROGS IN MAP
		//frog
	SDL_Rect fr = animation_frog->GetCurrentFrame();
	App->renderer->Blit(basic_sprites, 205, 199, &fr);
	App->renderer->Blit(basic_sprites, 292, 215, &fr);
	App->renderer->Blit(basic_sprites, 247, 251, &fr);
		//fairy
	SDL_Rect fa = animation_fairy->GetCurrentFrame();
	App->renderer->Blit(basic_sprites, 53, 209, &fa);
	App->renderer->Blit(basic_sprites, 98, 245, &fa);
	App->renderer->Blit(basic_sprites, 141, 193, &fa);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8));
		circles.getLast()->data->listener = this;
	}


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		//if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY())) //blits texture only if mouse inside shape
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = chains.getFirst();

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
