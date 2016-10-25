#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	bool ret = true;
	graphics = App->textures->Load("Game/Sprites/Ball_PNG.png");
	
	return ret;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(graphics);
	App->textures->Unload(life);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (createball == true)
	{
		player = App->physics->CreateCircle(354, 311, 8);
		player->listener = App->scene_intro;
		createball = false;
	}
	int playerPositionX, playerPositionY;
	player->GetPosition(playerPositionX,playerPositionY);
	App->renderer->Blit(graphics, playerPositionX, playerPositionY, NULL, 1.0f, player->GetRotation());
	for (uint i = 0, j = 0; i < lifes; i++, j+=22)
	{
		App->renderer->Blit(graphics,(390+j),220);
	}

	
	return UPDATE_CONTINUE;
}



