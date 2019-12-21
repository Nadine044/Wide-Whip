#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "MFonts.h"
#include "j1Input.h"
#include "MGui.h"
#include "UIImage.h"
#include "UIText.h"
#include "p2Point.h"


MGui::MGui() : j1Module()
{
	name.create("gui");
}

// Destructor
MGui::~MGui()
{}

// Called before render is available
bool MGui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool MGui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool MGui::PreUpdate()
{
	for (p2List_item<UIObject*>* iter = UI_objects.start; iter; iter = iter->next)
		iter->data->PreUpdate();

	return true;
}

// Called after all Updates
bool MGui::PostUpdate()
{
	for (p2List_item<UIObject*>* iter = UI_objects.start; iter; iter = iter->next)
		iter->data->PostUpdate(atlas);

	return true;
}

// Called before quitting
bool MGui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* MGui::GetAtlas() const
{
	return atlas;
}

UIObject* MGui::CreateUIObject(UIType type, iPoint pos, SDL_Rect rect_spritesheet)
{
	UIObject* ret = nullptr;
	switch (type)
	{
	case UIType::IMAGE:
		ret = new UIImage(type, pos, rect_spritesheet);
		break;
	case UIType::TEXT:
		ret = new UIText(type, pos, rect_spritesheet);
		break;
	case UIType::BUTTON:
		ret = new UIButton(type, pos, rect_spritesheet);
		break;
	default:
		break;
	}
	if (ret != nullptr)
		UI_objects.add(ret);

	else
		LOG("UI Object was not created correctly... Don't add to the list!");



	return ret;
}

UIText* MGui::CreateUIText(iPoint pos, p2String text)
{
	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	UIText* ret = (UIText*)CreateUIObject(UIType::TEXT, pos, texture_rect);

	ret->texture_text = texture_text;

	return ret;
}

UIButton* MGui::CreateUIButton(iPoint pos, p2String text, SDL_Rect image_rect)
{
	UIButton* ret = (UIButton*)CreateUIObject(UIType::BUTTON, pos, image_rect);

	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	ret->text = new UIText(UIType::TEXT, pos, texture_rect);
	ret->text->texture_text = texture_text;
	ret->background = new UIImage(UIType::BUTTON, pos, image_rect);
	ret->hover = new UIImage(UIType::BUTTON, pos, SDL_Rect{ 411,169,229,69 });
	ret->clicked = new UIImage(UIType::BUTTON, pos, SDL_Rect{ 642,169,229,69 });
	ret->current_image = ret->background;
	return ret;
}


// class Gui ---------------------------------------------------

