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
#include "ModuleEntityManager.h"
#include "MGui.h"
#include "UIButton.h"
#include "UIImage.h"
#include "j1UIMenu.h"
#include "InLevel.h"
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
	 
	menu = "Menu";

	menu_music = config.child_value("menu_music");
	start_game_sfx.path = config.child_value("start_game_sfx");
	start_game_sfx.id = App->audio->LoadFx(start_game_sfx.path.GetString());
	button_hover_sfx.path = config.child_value("button_hover_sfx");
	button_hover_sfx.id = App->audio->LoadFx(button_hover_sfx.path.GetString());
	button_click_sfx.path = config.child_value("button_click_sfx");
	button_click_sfx.id = App->audio->LoadFx(button_click_sfx.path.GetString());

	time_in_fade = config.child("time_in_fade").attribute("value").as_float();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	map_name_loaded = menu;
	App->menu->Start();
	debug_tex = App->tex->Load("maps/path2.png");

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
	CheckLevelChange();

	CheckSaveLoad();

	return true;
}

bool j1Scene::ButtonEvent(const UIButtonType type)
{
	switch (type)
	{
	case UIButtonType::TEST:
		LOG("Button pressed! Do something here!");
		ret->SetAllVisible(!ret->GetVisible());
		break;
	default:
		break;
	}

	return true;
}

bool j1Scene::SliderEvent(const UIScrollBarType type, const float ratio)
{
	switch (type)
	{
	case UIScrollBarType::TEST:
		LOG("SLIDER EVENT! Ratio: %f", ratio);
		break;
	default:
		break;
	}

	return true;
}

void j1Scene::CheckSaveLoad()
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();
}

void j1Scene::CheckLevelChange()
{
	// Check inputs to change the level
	if (!App->fade_to_black->IsFading())
	{
		
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			ChangeToLevel1Fade();
		}

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			ChangeToLevel2Fade();
		}

		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			StartThisLevelFade();
		}

		if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		{
			ChangeBetweenLevelFade();
		}

		if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			ChangeToMenuFade();
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

	if (change_to_menu)
	{
		ChangeLevelTo(menu);
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

void j1Scene::ChangeToLevel1Fade()
{
	App->fade_to_black->FadeToBlack(change_to_level_1, time_in_fade);
}

void j1Scene::ChangeToLevel2Fade()
{
	App->fade_to_black->FadeToBlack(change_to_level_2, time_in_fade);
}

void j1Scene::ChangeToMenuFade()
{
	App->in_level->lifes = 3;
	App->fade_to_black->FadeToBlack(change_to_menu, time_in_fade);
}

void j1Scene::StartThisLevelFade()
{
	App->fade_to_black->FadeToBlack(start_this_level, time_in_fade);
}

void j1Scene::ChangeBetweenLevelFade()
{
	App->fade_to_black->FadeToBlack(change_between_levels, time_in_fade);
}

void j1Scene::StartThisLevel()
{
	if (IsLevel2Loaded())
	{
		ChangeLevelTo(level2);
	}

	else if(IsLevel1Loaded())
	{
		ChangeLevelTo(level1);
	}

	else if (IsMenuLoaded())
	{
		ChangeLevelTo(menu);
	}
}

void j1Scene::ChangeBetweenLevel()
{
	if (IsLevel2Loaded())
	{
		ChangeLevelTo(menu);
	}
	else if(IsLevel1Loaded())
	{
		ChangeLevelTo(level2);
	}
	else if (IsMenuLoaded())
	{
		ChangeLevelTo(level1);
	}

}

void j1Scene::ChangeLevelTo(const p2String level)
{
	//Unload
	if (IsMenuLoaded())
	{
		App->menu->CleanUp();
		App->menu->active = false;

	}
	else
	{
		if (level == menu)
		{
			App->in_level->CleanUp();
		}
		App->in_level->active = false;
		App->map->CleanUp();
		App->collisions->CleanUp();
		App->module_entity_manager->CleanUp();
	}

	//Load
	if (level == menu)
	{
		App->menu->Start();
		App->menu->active = true;
	}
	else
	{

		if (IsMenuLoaded())
		{
			App->in_level->Start();
		}
		App->in_level->active = true;
		App->map->Load(level.GetString());
	}

	map_name_loaded = level;
}

bool j1Scene::IsMenuLoaded() const
{
	return GetMapNameLoaded() == menu;
}

bool j1Scene::IsLevel1Loaded() const
{
	return GetMapNameLoaded() == level1;
}

bool j1Scene::IsLevel2Loaded() const
{
	return GetMapNameLoaded() == level2;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;
	if(!IsMenuLoaded())
		App->map->Draw();

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && App->in_level->esc_menu == false)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

p2String j1Scene::GetMapNameLoaded() const
{
	return map_name_loaded;
}

bool j1Scene::Save(pugi::xml_node& save_file) const
{
	save_file.append_child("level_loaded").append_attribute("value") = GetMapNameLoaded().GetString();


	return true;
}

bool j1Scene::Load(pugi::xml_node& save_file)
{
	p2String level_saved = save_file.child("level_loaded").attribute("value").as_string();

	if (level_saved != GetMapNameLoaded())
	{
		ChangeLevelTo(level_saved);
	}
	return true;
}