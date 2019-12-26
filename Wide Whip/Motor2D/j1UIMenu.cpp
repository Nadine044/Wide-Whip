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

	App->gui->CreateUIImage(iPoint{ 0, 0 }, SDL_Rect{ 0, 2673, 1600, 1600 });
	App->gui->CreateUIButton(iPoint{ 400, 150 }, "NEW GAME", SDL_Rect{ 0,3404,200,38 }, UIButtonType::NEW_GAME, this, true);
	App->gui->CreateUIButton(iPoint{ 400, 250 }, "CONTINUE", SDL_Rect{ 0,3404,200,38 }, UIButtonType::CONTINUE, this, true);
	App->gui->CreateUIButton(iPoint{ 400, 350 }, "SETTINGS", SDL_Rect{ 0,3404,200,38 }, UIButtonType::SETTINGS, this, true);
	App->gui->CreateUIButton(iPoint{ 400, 450 }, "CREDITS", SDL_Rect{ 0,3404,200,38 }, UIButtonType::CREDITS, this, true);
	App->gui->CreateUIButton(iPoint{ 400, 550 }, "EXIT", SDL_Rect{ 0,3404,200,38 }, UIButtonType::EXIT, this, true);

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