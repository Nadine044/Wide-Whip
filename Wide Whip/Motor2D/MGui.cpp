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

bool MGui::Update(float dt)
{
	for (p2List_item<UIObject*>* iter = UI_objects.start; iter; iter = iter->next)
		iter->data->Update(dt);

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

UIObject* MGui::CreateUIObject(UIType type, iPoint pos, SDL_Rect rect_spritesheet, bool draggable)
{
	UIObject* ret = nullptr;
	switch (type)
	{
	case UIType::IMAGE:
		ret = new UIImage(type, pos, rect_spritesheet, draggable);
		break;
	case UIType::TEXT:
		ret = new UIText(type, pos, rect_spritesheet, draggable);
		break;
	case UIType::BUTTON:
		ret = new UIButton(type, pos, rect_spritesheet, draggable);
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

UIImage* MGui::CreateUIImage(iPoint pos, SDL_Rect rect_spritesheet, bool draggable)
{
	return (UIImage*)CreateUIObject(UIType::IMAGE, pos, rect_spritesheet, draggable);
}

UIText* MGui::CreateUIText(iPoint pos, p2String text, bool draggable)
{
	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	UIText* ret = (UIText*)CreateUIObject(UIType::TEXT, pos, texture_rect, draggable);

	ret->texture_text = texture_text;

	return ret;
}

UIButton* MGui::CreateUIButton(iPoint pos, p2String text, SDL_Rect image_rect, UIButtonType type, j1Module* listener, bool draggable)
{
	UIButton* ret = (UIButton*)CreateUIObject(UIType::BUTTON, pos, image_rect, draggable);

	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	ret->text = new UIText(UIType::TEXT, pos, texture_rect, draggable);
	ret->text->texture_text = texture_text;
	ret->background = new UIImage(UIType::BUTTON, pos, image_rect, draggable);
	ret->hover = new UIImage(UIType::BUTTON, pos, SDL_Rect{ 0,3445,200,38 }, draggable);
	ret->clicked = new UIImage(UIType::BUTTON, pos, SDL_Rect{ 0,3487,200,38 }, draggable);
	ret->current_image = ret->background;
	ret->button_type = type;
	ret->listener = listener;
	return ret;
}


// class Gui ---------------------------------------------------

