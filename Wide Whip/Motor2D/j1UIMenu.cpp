#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1UIMenu.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"

UIMenu::UIMenu()
{}

bool UIMenu::Start()
{
	//CASE MENU

	//MYTODO: Audio
	App->audio->PlayMusic(App->scene->menu_music.GetString());

	App->gui->CreateUIImage(iPoint{ 0, 0 }, SDL_Rect{ 0, 2673, 1600, 1600 });
	App->gui->CreateUIButton(iPoint{ 100, 300 }, "NEW GAME", SDL_Rect{ 0,3404,200,38 }, UIButtonType::NEW_GAME, this, true);
	App->gui->CreateUIButton(iPoint{ 130, 375 }, "CONTINUE", SDL_Rect{ 0,3404,200,38 }, UIButtonType::CONTINUE, this, true);
	App->gui->CreateUIButton(iPoint{ 100, 450 }, "SETTINGS", SDL_Rect{ 0,3404,200,38 }, UIButtonType::SETTINGS, this, true);
	App->gui->CreateUIButton(iPoint{ 130, 525 }, "CREDITS", SDL_Rect{ 0,3404,200,38 }, UIButtonType::CREDITS, this, true);
	App->gui->CreateUIButton(iPoint{ 100, 600 }, "EXIT", SDL_Rect{ 0,3404,200,38 }, UIButtonType::EXIT, this, true);
	

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
	return true;
}

bool UIMenu::ButtonEvent(const UIButtonType type)
{
	switch (type)
	{
	case UIButtonType::NEW_GAME:
		LOG("Button NEW GAME pressed.");
		break;
	case UIButtonType::CONTINUE:
		LOG("CONTINUE game");
		break;
	case UIButtonType::SETTINGS:
		LOG("Go into SETTINGS");
		settingsParent = App->gui->CreateUIImage(iPoint{ 400, 300 }, SDL_Rect{ 29, 1140, 1292, 1013 }, true);
		App->gui->CreateUIText(iPoint{ 200, 20 }, "SETTINGS", true, settingsParent);
		App->gui->CreateUIText(iPoint{ 20, 100 }, "Music Volume", true, settingsParent);
		App->gui->CreateUIText(iPoint{ 20, 150 }, "SFX Volume", true, settingsParent);
		break;
	case UIButtonType::CREDITS:
		LOG("Our precious CREDITS");
		App->gui->CreateUIText(iPoint{ 700, 630 }, "Copyright:");
		App->gui->CreateUIText(iPoint{ 700, 650 }, "MIT LICENSE");
		App->gui->CreateUIText(iPoint{ 700, 670 }, "WIDE WHIP by Jorge Gemas and Nadine Gutiérrez");
		break;
	case UIButtonType::EXIT:
		LOG("EXIT game");
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
		break;
	case UIScrollBarType::SFX:
		LOG("SFX SCROLLBAR");
		break;
	default:
		break;
	}


	return true;
}