#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1UIMenu.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "ModuleFadeToBlack.h"
#include "InLevel.h"
#include "SDL_mixer/include/SDL_mixer.h"

UIMenu::UIMenu()
{
	name.create("ui_menu");
}

bool UIMenu::Awake(pugi::xml_node& config)
{
	bool ret = true;

	active = false;
	return ret;
}

bool UIMenu::Start()
{
	App->audio->PlayMusic(App->scene->menu_music.GetString());

	App->gui->CreateUIImage(iPoint{ 0, 0 }, SDL_Rect{ 0, 2673, 1600, 1600 });
	App->gui->CreateUIButton(iPoint{ 100, 300 }, "NEW GAME", SDL_Rect{ 0,3447,200,38 }, UIButtonType::NEW_GAME, this);
	UIButton* continue_but = App->gui->CreateUIButton(iPoint{ 130, 375 }, "CONTINUE", SDL_Rect{ 0,3447,200,38 }, UIButtonType::CONTINUE, this);
	if (!App->SaveGameFileExist())
		continue_but->enabled = false;
	App->gui->CreateUIButton(iPoint{ 100, 450 }, " SETTINGS", SDL_Rect{ 0,3447,200,38 }, UIButtonType::SETTINGS, this);
	App->gui->CreateUIButton(iPoint{ 130, 525 }, "  CREDITS", SDL_Rect{ 0,3447,200,38 }, UIButtonType::CREDITS, this);
	App->gui->CreateUIButton(iPoint{ 100, 600 }, "      EXIT", SDL_Rect{ 0,3447,200,38 }, UIButtonType::EXIT, this);
	
	newGameParent = App->gui->CreateUIImage(iPoint{ 360, 280 }, SDL_Rect{ 761, 0, 122, 130 }, true);
	App->gui->CreateUIInputText(iPoint{ 140, 20 }, "Name: ", SDL_Rect{ 967, 25, 463, 69 }, false, newGameParent);
	App->gui->CreateUIButton(iPoint{ 400, 320 }, "     START", SDL_Rect{ 0, 3447, 200, 38 }, UIButtonType::START, this, false, newGameParent);
	newGameParent->SetAllVisible(false);

	settingsParent = App->gui->CreateUIImage(iPoint{ 400, 300 }, SDL_Rect{ 33, 1140, 487, 384 }, false);
	App->gui->CreateUIText(iPoint{ 210, 20 }, "SETTINGS", SDL_Color{0, 0, 0, 0}, false, settingsParent);
	App->gui->CreateUIText(iPoint{ 25, 105 }, "Music Volume", SDL_Color{ 0, 0, 0, 0 }, false, settingsParent);
	App->gui->CreateUIScrollBar(iPoint{ 120, 105 }, SDL_Rect{ 383, 574, 322, 36 }, Orientation::HORIZONTAL, UIScrollBarType::MUSIC, this, settingsParent);
	App->gui->CreateUIText(iPoint{ 25, 175 }, "SFX Volume", SDL_Color{ 0, 0, 0, 0 }, false, settingsParent);
	App->gui->CreateUIScrollBar(iPoint{ 120, 175 }, SDL_Rect{ 383, 574, 322, 36 }, Orientation::HORIZONTAL, UIScrollBarType::SFX, this, settingsParent);
	settingsParent->SetAllVisible(false);

	creditsParent = App->gui->CreateUIImage(iPoint{ 0, 0 }, SDL_Rect{ 0, 0, 0, 0 });
	App->gui->CreateUIText(iPoint{ 700, 630 }, "WIDE WHIP", SDL_Color{ 0, 0, 0, 0 }, false, creditsParent);
	App->gui->CreateUIText(iPoint{ 700, 650 }, "MIT LICENSE", SDL_Color{ 0, 0, 0, 0 }, false, creditsParent);
	App->gui->CreateUIText(iPoint{ 700, 670 }, "Copyright (c) [2019] [Nadine Gutiérrez & Jorge Gemas]", SDL_Color{ 0, 0, 0, 0 }, false, creditsParent);
	creditsParent->SetAllVisible(false);

	return true;
}

bool UIMenu::PreUpdate()
{
	return true;
}

bool UIMenu::Update(float dt)
{
	return true;
}

bool UIMenu::PostUpdate()
{
	

	return true;
}

bool UIMenu::CleanUp()
{
	App->gui->CleanUp();
	return true;
}

bool UIMenu::ButtonEvent(const UIButtonType type)
{
	switch (type)
	{
	case UIButtonType::NEW_GAME:
		LOG("Button NEW GAME pressed.");
		newGameParent->SetAllVisible(!newGameParent->GetVisible());		
		if (settingsParent->GetVisible())
			settingsParent->SetAllVisible(false);
		break;
	case UIButtonType::START:
		LOG("START into the game");
		App->in_level->coins_count = 0;
		App->audio->PlayFx(App->scene->start_game_sfx.id);
		App->scene->ChangeToLevel1Fade();
		break;
	case UIButtonType::CONTINUE:
		LOG("CONTINUE game");
		App->scene->ChangeToSaveGameStateFade();
		break;
	case UIButtonType::SETTINGS:
		LOG("Go into SETTINGS");
		settingsParent->SetAllVisible(!settingsParent->GetVisible());
		if (newGameParent->GetVisible())
			newGameParent->SetAllVisible(false);
		break;
	case UIButtonType::CREDITS:
		LOG("Our precious CREDITS");
		creditsParent->SetAllVisible(!creditsParent->GetVisible());
		break;
	case UIButtonType::EXIT:
		LOG("EXIT game");
		return false;
		break;
	default:
		break;
	}

	return true;
}

bool UIMenu::SliderEvent(const UIScrollBarType type, const float ratio)
{
	switch (type)
	{
	case UIScrollBarType::TEST:
		LOG("TEST SCROLLBAR");
		break;
	case UIScrollBarType::MUSIC:
		LOG("Music SCROLLBAR");
		Mix_VolumeMusic(MAX_VOLUME*ratio);
		break;
	case UIScrollBarType::SFX:
		LOG("SFX SCROLLBAR");
		App->audio->SetAllChunkVolume(MAX_VOLUME*ratio);
		break;
	default:
		break;
	}


	return true;
}