#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "InLevel.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "UIImage.h"
#include "ModuleEntityManager.h"

InLevel::InLevel()
{
	name.create("in_level");
}

bool InLevel::Awake(pugi::xml_node& config)
{
	bool ret = true;

	music = config.child_value("music");
	active = false;
	return ret;
}

bool InLevel::Start()
{
	App->audio->PlayMusic(music.GetString());

	/*UI Elements in game*/
	
	App->gui->CreateUIImage(iPoint{ 10, 10 }, SDL_Rect{ 761, 0, 122, 130 });
	App->gui->CreateUIImage(iPoint{ 137, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	App->gui->CreateUIImage(iPoint{ 210, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	App->gui->CreateUIImage(iPoint{ 283, 20 }, SDL_Rect{ 888, 27, 69, 72 });

	App->gui->CreateUIImage(iPoint{ 820, 10 }, SDL_Rect{ 760, 283, 67, 77 });
	App->gui->CreateUIText(iPoint{ 850, 10 }, coins_count);

	return true;
}

bool InLevel::PreUpdate()
{
	return true;
}

bool InLevel::Update(float dt)
{
	return true;
}

bool InLevel::PostUpdate()
{
	return true;
}

bool InLevel::CleanUp()
{
	App->gui->CleanUp();
	return true;
}

bool InLevel::ButtonEvent(const UIButtonType type)
{
	return true;
}

bool InLevel::SliderEvent(const UIScrollBarType type, const float ratio)
{
	return true;
}