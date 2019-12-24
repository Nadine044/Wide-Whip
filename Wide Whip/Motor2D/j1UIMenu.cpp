#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1UIMenu.h"
#include "MGui.h"

UIMenu::UIMenu()
{}

bool UIMenu::Start()
{
	//CASE MENU

	App->gui->CreateUIButton(iPoint{ 410, 100 }, "NEW GAME", SDL_Rect{ 0, 113, 229, 69 }, UIButtonType::NEW_GAME, this, true);
	App->gui->CreateUIButton(iPoint{ 410, 200 }, "CONTINUE", SDL_Rect{ 0, 113, 229, 69 }, UIButtonType::CONTINUE, this, true);
	App->gui->CreateUIButton(iPoint{ 410, 300 }, "SETTINGS", SDL_Rect{ 0, 113, 229, 69 }, UIButtonType::SETTINGS, this, true);
	App->gui->CreateUIButton(iPoint{ 410, 400 }, "CREDITS", SDL_Rect{ 0, 113, 229, 69 }, UIButtonType::CREDITS, this, true);
	App->gui->CreateUIButton(iPoint{ 410, 500 }, "EXIT", SDL_Rect{ 0, 113, 229, 69 }, UIButtonType::EXIT, this, true);

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
		break;
	case UIButtonType::CREDITS:
		LOG("Our precious CREDITS");
		break;
	case UIButtonType::EXIT:
		LOG("EXIT game");
		break;
	default:
		break;
	}


	return true;
}