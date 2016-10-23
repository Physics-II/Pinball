//#include "Globals.h"
//#include "Application.h"
//#include "ModuleRender.h"
//#include "ModuleScore.h"
//#include "ModuleInput.h"
//#include "ModuleTextures.h"
//#include "ModuleAudio.h"
//#include "ModulePhysics.h"
//
//
//
//ModuleScore::ModuleScore(Application* app, bool start_enabled = true) : Module(app, start_enabled)
//{
//	n0 = { 63,408,7,12 };
//	n1 = { 38,408,7,12 };
//	n3 = { 75,408,7,12 };
//	n4 = { 98,408,7,12 };
//	n8 = { 51,408,7,12 };
//
//}
//ModuleScore::~ModuleScore()
//{
//
//
//}
//
//bool ModuleScore::Start()
//{
//	bool ret = true;
//	
//	graphics = App->textures->Load("Game/Sprites/Basic_sprites_PNG.png");
//	maxPuntuation = 0;
//	number_pos1 = 0;
//	number_pos2 = 0;
//	number_pos3 = 0;
//	number_pos4 = 0;
//	return ret;
//}
//bool ModuleScore::CleanUp()
//{
//	App->textures->Unload(graphics);
//
//	return true;
//}
//
//update_status ModuleScore::Update()
//{
//	if (puntuation >= maxPuntuation)
//	{
//		maxPuntuation = puntuation;
//	}
//	
//	if (puntuation == 0)
//	{
//		App->renderer->Blit(graphics, 394,255, &n0);
//	}
//	if (puntuation <= 90)
//	{
//		for (uint i = 0;i <= puntuation; i += 10)
//		{
//			if (i == puntuation)
//			{
//
//			}
//		}
//	}
//
//}