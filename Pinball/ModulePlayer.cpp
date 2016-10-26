#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

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
	ball_spawn_sound = App->audio->LoadFx("Game/Sounds/ball_spawn.wav");
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
		App->audio->PlayFx(ball_spawn_sound);
		player = App->physics->CreateCircle(354, 311, 8);
		player->listener = App->scene_intro;
		b2Filter b;
		b.categoryBits = ON;
		b.maskBits = ON|OFF;
		player->body->GetFixtureList()->SetFilterData(b);
		createball = false;
	}

	player->body->SetBullet(true);

	if ((player->body->GetPosition().x > 364) && (player->body->GetPosition().y > 280)) //sets restitution only if ball has departed
	{
		player->body->GetFixtureList()->SetRestitution(0.3);
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



