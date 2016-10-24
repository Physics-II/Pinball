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

	springle_tex = App->textures->Load("Game/Sprites/springle.png");

	

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
		App->renderer->Blit(basic_sprites, 205, 199, &fr);
		App->renderer->Blit(basic_sprites, 292, 215, &fr);
		App->renderer->Blit(basic_sprites, 247, 251, &fr);
		
		//fairy
	
		SDL_Rect fa = animation_fairy->GetCurrentFrame();
		App->renderer->Blit(basic_sprites, 53, 209, &fa);
		App->renderer->Blit(basic_sprites, 98, 245, &fa);
		App->renderer->Blit(basic_sprites, 141, 193, &fa);
		createfairy = false;

		//springy
		int sPositionX, sPositionY;
		App->physics->springy->GetPosition(sPositionX, sPositionY);
		App->renderer->Blit(springle_tex, sPositionX, sPositionY, NULL, 1.0f);

	App->physics->springy->body->ApplyForce({ 0,-10}, { 0, 0 }, true);
	
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
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
		App->physics->springy->body->ApplyForce({ 0,-200}, { 0, 0 }, true);
	}
	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	

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
	//print score
	//p2DynArray title("Score: %i Global Score: %i",score, globalScore);
	//App->window->SetTitle(title.GetString());
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
