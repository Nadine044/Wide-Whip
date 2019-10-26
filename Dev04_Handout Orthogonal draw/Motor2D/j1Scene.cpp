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
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("mapping.tmx");
	
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
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;



	CheckLevelChange();

	


	//App->render->Blit(img, 0, 0);
	App->map->Draw(); // to map update

	////Draw player MYTODO
	//iPoint mouse_pos;
	//
	//App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	//mouse_pos.x -= App->render->camera.x;
	//mouse_pos.y -= App->render->camera.y;
	//iPoint tile_on_mouse = App->map->WorldToMap(mouse_pos);

	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:(%d,%d)",
	//				App->map->data.width, App->map->data.height,
	//				App->map->data.tile_width, App->map->data.tile_height,
	//				App->map->data.tilesets.count(),
	//				tile_on_mouse.x,
	//				tile_on_mouse.y);

	//App->win->SetTitle(title.GetString());
	return true;
}

void j1Scene::CheckLevelChange()
{
	
	// Check inputs to change the level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(change_to_level_1, 2.f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(change_to_level_2, 2.f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(start_this_level, 2.f);
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		App->fade_to_black->FadeToBlack(change_between_levels, 2.f);
	}


	// Change level if is set to change.
	if (change_to_level_1)
	{
		ChangeLevelTo("map1.tmx");
	}

	if (change_to_level_2)
	{
		ChangeLevelTo("mapping.tmx");
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
	if (App->map->GetMapNameLoaded() == "mapping.tmx")
	{
		ChangeLevelTo("mapping.tmx");
	}
	else
	{
		ChangeLevelTo("map1.tmx");
	}
}

void j1Scene::ChangeBetweenLevel()
{
	if (App->map->GetMapNameLoaded() == "mapping.tmx")
	{
		ChangeLevelTo("map1.tmx");
	}
	else
	{
		ChangeLevelTo("mapping.tmx");
	}
}

void j1Scene::ChangeLevelTo(const p2SString level)
{
	App->map->CleanUp();
	App->collisions->CleanUp();
	App->player->CleanUp();

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
