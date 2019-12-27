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

	music = config.child_value("music");
	menu_music = config.child_value("menu_music");

	time_in_fade = config.child("time_in_fade").attribute("value").as_float();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	/*if (App->map->Load(level1.GetString()) == true)
	{
		map_name_loaded = level1;
		App->audio->PlayMusic(music.GetString());
	}

	debug_tex = App->tex->Load("maps/path2.png");
	ret = App->gui->CreateUIImage(iPoint{ 300, 100 }, SDL_Rect{ 485, 829, 328, 103 }, true);
	App->gui->CreateUIText(iPoint{ 300, 0 }, "Hello World", true, ret);
	App->gui->CreateUIButton(iPoint{ 50, 50 }, "Button", SDL_Rect{ 0,113, 229, 69 }, UIButtonType::TEST, this, true);
	App->gui->CreateUIInputText(iPoint{ 200, 300 }, "Your Name", SDL_Rect{ 488, 569, 344, 61 }, true, ret);
	App->gui->CreateUIScrollBar(iPoint{ 50, 300 }, SDL_Rect{ 974, 788, 9, 154 }, Orientation::VERTICAL, UIScrollBarType::TEST, this);
	App->gui->CreateUIScrollBar(iPoint{ 100, 375 }, SDL_Rect{ 557, 62, 154, 9 }, Orientation::HORIZONTAL, UIScrollBarType::TEST, this);

	*/
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

void j1Scene::ChangeToLevel1Fade()
{
	App->fade_to_black->FadeToBlack(change_to_level_1, time_in_fade);
}

void j1Scene::ChangeToLevel2Fade()
{
	App->fade_to_black->FadeToBlack(change_to_level_2, time_in_fade);
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
}

void j1Scene::ChangeBetweenLevel()
{
	if (IsLevel2Loaded())
	{
		ChangeLevelTo(level1);
	}
	else
	{
		ChangeLevelTo(level2);
	}
}

void j1Scene::ChangeLevelTo(const p2String level)
{
	//Unload
	App->map->CleanUp();
	App->collisions->CleanUp();
	App->module_entity_manager->CleanUp();

	//Load
	App->map->Load(level.GetString());
	map_name_loaded = level;
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