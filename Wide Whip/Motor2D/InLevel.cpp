#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "InLevel.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "UIImage.h"
#include "j1Player.h"
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
	first_life = App->gui->CreateUIImage(iPoint{ 137, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	second_life = App->gui->CreateUIImage(iPoint{ 210, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	third_life = App->gui->CreateUIImage(iPoint{ 283, 20 }, SDL_Rect{ 888, 27, 69, 72 });

	App->gui->CreateUIImage(iPoint{ 820, 10 }, SDL_Rect{ 760, 283, 67, 77 });
	App->gui->CreateUIText(iPoint{ 850, 10 }, coins_count);

	pause = App->gui->CreateUIImage(iPoint{ 250, 200 }, SDL_Rect{ 947, 417, 519, 414 });
	App->gui->CreateUIButton(iPoint{ 170, 70 }, "  RESUME", SDL_Rect{ 0, 3447, 200, 38 }, UIButtonType::RESUME, this, false, pause);
	App->gui->CreateUIButton(iPoint{ 170, 140 }, "     SAVE", SDL_Rect{ 0, 3447, 200, 38 }, UIButtonType::SAVE, this, false, pause);
	App->gui->CreateUIButton(iPoint{ 170, 210 }, " SETTINGS", SDL_Rect{ 0, 3447, 200, 38 }, UIButtonType::SETTINGS, this, false, pause);
	App->gui->CreateUIButton(iPoint{ 170, 280 }, "      EXIT", SDL_Rect{ 0, 3447, 200, 38 }, UIButtonType::IN_GAME_EXIT, this, false, pause);
	pause->SetAllVisible(false);

	esc_menu = true;

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
	switch (type)
	{
	case UIButtonType::RESUME:
		LOG("RESUME game");
		pause->SetAllVisible(false);
		break;
	case UIButtonType::SAVE:
		LOG("SAVE the current game");
		App->SaveGame();
		break;
	case UIButtonType::IN_GAME_EXIT:
		LOG("EXIT to menu WITHOUT SAVE THE GAME");
		App->scene->ChangeToMenuFade();
	default:
		break;
	}

	return true;
}

bool InLevel::SliderEvent(const UIScrollBarType type, const float ratio)
{
	return true;
}