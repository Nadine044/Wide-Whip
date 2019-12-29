#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "InLevel.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "UIImage.h"
#include "j1Player.h"
#include "ModuleEntityManager.h"
#include "j1Render.h"
#include "j1Timer.h"

InLevel::InLevel()
{
	name.create("in_level");
}

bool InLevel::Awake(pugi::xml_node& config)
{
	bool ret = true;

	music = config.child_value("music");
	pause_open_sfx.path = config.child_value("pause_open");
	pause_open_sfx.id = App->audio->LoadFx(pause_open_sfx.path.GetString());
	pause_close_sfx.path = config.child_value("pause_close");
	pause_close_sfx.id = App->audio->LoadFx(pause_close_sfx.path.GetString());

	active = false;
	return ret;
}

bool InLevel::Start()
{
	App->audio->PlayMusic(music.GetString());
	if (App->IsPaused())
		App->PauseResumeGame();
	/*UI Elements in game*/
	
	App->gui->CreateUIImage(iPoint{ 10, 10 }, SDL_Rect{ 761, 0, 122, 130 });
	first_life = App->gui->CreateUIImage(iPoint{ 137, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	second_life = App->gui->CreateUIImage(iPoint{ 210, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	third_life = App->gui->CreateUIImage(iPoint{ 283, 20 }, SDL_Rect{ 888, 27, 69, 72 });
	player_name_text = App->gui->CreateUIText(iPoint{ 150, 100 }, player_name, SDL_Color{ 255, 255, 255, 255 });

	App->gui->CreateUIImage(iPoint{ 820, 10 }, SDL_Rect{ 760, 283, 67, 77 });
	App->gui->CreateUIImage(iPoint{ 890, 15 }, SDL_Rect{ 760, 1139, 174, 59 });
	coins_text = App->gui->CreateUIText(iPoint{ 916, 37 }, p2String(std::to_string(coins_count).c_str()));
	counter_text = App->gui->CreateUIText(iPoint{ 916, 100 }, p2String(std::to_string(counter_secs).c_str()) += " Seconds", SDL_Color{ 255, 255, 255, 255 });
	App->gui->CreateUIText(iPoint{ 875, 100 }, "Timer:", SDL_Color{255, 255, 255, 255});
	counter_secs = 0;
	timer_counter.Start();
	//App->gui->CreateUIImage(iPoint{ 4, 690 }, SDL_Rect{ 581, 1140, 164, 70 });
	//App->gui->CreateUIImage(iPoint{ 200, 700 }, SDL_Rect{ 760, 1139, 174, 59 }, true);

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
	if (timer_counter.ReadSec() >= 1)
	{
		if (!App->IsPaused())
		{
			timer_counter.Start();
			++counter_secs;
			counter_text->ChangeText(p2String(p2String(std::to_string(counter_secs).c_str()) += " Seconds"));
		}
	}
	return true;
}

bool InLevel::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (pause->GetVisible() == true)
			App->audio->PlayFx(pause_close_sfx.id);
		else
			App->audio->PlayFx(pause_open_sfx.id);

		pause->SetAllVisible(!pause->GetVisible());
		App->PauseResumeGame();
	}
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
		App->PauseResumeGame();
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

bool InLevel::Save(pugi::xml_node& save_file) const
{
	save_file.append_child("coins_count").append_attribute("value") = coins_count;
	save_file.append_child("counter_secs").append_attribute("value") = counter_secs;
	save_file.append_child("lifes").append_attribute("value") = lifes;
	save_file.append_child("player_name").append_attribute("value") = player_name.GetString();

	return true;
}

bool InLevel::Load(pugi::xml_node& config)
{
	if (pause)
		pause->SetAllVisible(false);

	coins_count = config.child("coins_count").attribute("value").as_int();
	if (coins_text)
		coins_text->ChangeText(p2String(std::to_string(coins_count).c_str()));
	counter_secs = config.child("counter_secs").attribute("value").as_int();
	lifes = config.child("lifes").attribute("value").as_int();
	player_name = config.child("player_name").attribute("value").as_string();
	if (player_name_text)
		player_name_text->ChangeText(player_name);

	return true;
}