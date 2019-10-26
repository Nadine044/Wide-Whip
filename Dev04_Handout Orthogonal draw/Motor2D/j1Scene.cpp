#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	pugi::xml_node levels = config.child("levels");

	level1 = levels.child_value("level1");
	level2 = levels.child_value("level2");

	time_in_fade = config.child("time_in_fade").attribute("value").as_float();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(level2.GetString());
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();
	   	  

	CheckLevelChange();

	return true;
}

void j1Scene::CheckLevelChange()
{
	// Check inputs to change the level
	if (!App->fade_to_black->IsFading())
	{
		
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			App->fade_to_black->FadeToBlack(change_to_level_1, time_in_fade);
		}

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			App->fade_to_black->FadeToBlack(change_to_level_2, time_in_fade);
		}

		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			App->fade_to_black->FadeToBlack(start_this_level, time_in_fade);
		}

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			App->fade_to_black->FadeToBlack(change_between_levels, time_in_fade);
		}
	}

	// Change level if is set to change.
	if (change_to_level_1)
	{
		ChangeLevelTo(level1);
	}

	if (change_to_level_2)
	{
		ChangeLevelTo(level2);
	}

	if (start_this_level)
	{
		StartThisLevel();
	}

	if (change_between_levels)
	{
		ChangeBetweenLevel();
	}
}

void j1Scene::StartThisLevel()
{
	if (App->map->GetMapNameLoaded() == level2)
	{
		ChangeLevelTo(level2);
	}
	else
	{
		ChangeLevelTo(level1);
	}
}

void j1Scene::ChangeBetweenLevel()
{
	if (App->map->GetMapNameLoaded() == level2)
	{
		ChangeLevelTo(level1);
	}
	else
	{
		ChangeLevelTo(level2);
	}
}

void j1Scene::ChangeLevelTo(const p2SString level)
{
	//Unload
	App->map->CleanUp();
	App->collisions->CleanUp();
	App->player->CleanUp();

	//Load
	App->map->Load(level.GetString());
	App->player->Start();
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
