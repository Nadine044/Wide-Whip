#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "InLevel.h"
#include "MGui.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "UIImage.h"
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
	ret = App->gui->CreateUIImage(iPoint{ 300, 100 }, SDL_Rect{ 485, 829, 328, 103 }, true);
	App->gui->CreateUIText(iPoint{ 300, 0 }, "Hello World", true, ret);
	App->gui->CreateUIButton(iPoint{ 50, 50 }, "Button", SDL_Rect{ 0,113, 229, 69 }, UIButtonType::TEST, this, true);
	App->gui->CreateUIInputText(iPoint{ 200, 300 }, "Your Name", SDL_Rect{ 488, 569, 344, 61 }, true, ret);
	App->gui->CreateUIScrollBar(iPoint{ 50, 300 }, SDL_Rect{ 974, 788, 9, 154 }, Orientation::VERTICAL, UIScrollBarType::TEST, this);
	App->gui->CreateUIScrollBar(iPoint{ 100, 375 }, SDL_Rect{ 557, 62, 154, 9 }, Orientation::HORIZONTAL, UIScrollBarType::TEST, this);


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