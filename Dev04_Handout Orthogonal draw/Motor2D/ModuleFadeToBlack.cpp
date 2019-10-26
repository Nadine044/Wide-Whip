#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "ModuleFadeToBlack.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Scene.h"

ModuleFadeToBlack::ModuleFadeToBlack()
{
	name.create("FadeToBlack");
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{}

// Load assets
bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	uint width = 0u;
	uint height = 0u;
	App->win->GetWindowSize(width, height);
	screen = { 0, 0,  (int)(width * App->win->GetScale()), (int)(height * App->win->GetScale()) };
	return true;
}

// Update: draw background
bool ModuleFadeToBlack::Update(float dt)
{
	if(current_step == fade_step::none)
		return true;

	now = SDL_GetTicks() - start_time;
	normalized = MIN(1.0f, (float)now / (float)total_time);

	switch(current_step)
	{
		case fade_step::fade_to_black:
		{
			if(now >= total_time)
			{
				//total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::changing_mode;
				*to_active = true;
			}
		} break;

		case fade_step::changing_mode:
		{
			*to_active = false;
			current_step = fade_step::fade_from_black;
		}
		case fade_step::fade_from_black:
		{


			normalized = 1.0f - normalized;

			if (now >= total_time)
			{
				current_step = fade_step::none;
				to_active = nullptr;
			}

		} break;
	}

	return true;
}

bool ModuleFadeToBlack::PostUpdate()
{
	if (current_step == fade_step::none)
		return true;


	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool ModuleFadeToBlack::Save(pugi::xml_node &save_file) 
{
	save_file.append_child("total_time").append_attribute("value") = total_time;
	save_file.append_child("now").append_attribute("value") = now;
	save_file.append_child("current_step").append_attribute("value") = current_step;


	if (App->player->GetReviveBoolAdress() == to_active)
	{
		save_file.append_child("test").append_attribute("value") = "revive";
	}
	else if (App->scene->GetChangeToLevel1BoolAdress() == to_active)
	{
		save_file.append_child("test").append_attribute("value") = "change_to_level_1";
	}

	else if (App->scene->GetChangeToLevel2BoolAdress() == to_active)
	{
		save_file.append_child("test").append_attribute("value") = "change_to_level_2";
	}

	else if (App->scene->GetStartThisLevelBoolAdress() == to_active)
	{
		save_file.append_child("test").append_attribute("value") = "start_this_level";
	}

	else if (App->scene->GetChangeBetweenLevelsBoolAdress() == to_active)
	{
		save_file.append_child("test").append_attribute("value") = "change_between_levels";
	}



	return true;
}

bool ModuleFadeToBlack::Load(pugi::xml_node &save_file)
{
	total_time = (Uint32)(save_file.child("total_time").attribute("value").as_uint());
	start_time = SDL_GetTicks(); //- now;
	current_step = (fade_step)(save_file.child("current_step").attribute("value").as_int());
	p2SString name = save_file.child("test").attribute("value").as_string();
	if (name == "revive")
	{
		to_active = App->player->GetReviveBoolAdress();
	}

	else if (name == "change_to_level_1")
	{
		to_active = App->scene->GetChangeToLevel1BoolAdress();
	}

	else if (name == "change_to_level_2")
	{
		to_active = App->scene->GetChangeToLevel2BoolAdress();
	}

	else if (name == "start_this_level")
	{
		to_active = App->scene->GetStartThisLevelBoolAdress();
	}

	else if (name == "change_between_levels")
	{
		to_active = App->scene->GetChangeBetweenLevelsBoolAdress();
	}
	return true;
}


// Fade to black. At mid point deactivate one module, then activate the other
bool ModuleFadeToBlack::FadeToBlack(bool &active, float time)
{
	bool ret = false;

	if(current_step == fade_step::none || current_step == fade_step::fade_from_black)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_active = &active;
		ret = true;
	}

	return ret;
}

bool ModuleFadeToBlack::IsFading() const
{
	return current_step != fade_step::none;
}