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
#include "UIInputText.h"


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
	SDL_StartTextInput();
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

void MGui::InputEvent(char* input)
{
	for (p2List_item<UIObject*>* iter = UI_objects.start; iter; iter = iter->next)
	{
		if (iter->data->type == UIType::INPUTTEXT)
		{
			UIInputText* iter_input_text = (UIInputText*)iter->data;
			iter_input_text->WriteInput(input);
		}
	}
}


UIObject* MGui::CreateUIObject(UIType type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent)
{
	UIObject* ret = nullptr;
	switch (type)
	{
	case UIType::IMAGE:
		ret = new UIImage(type, local_pos, rect_spritesheet_original, draggable, parent);
		break;
	case UIType::TEXT:
		ret = new UIText(type, local_pos, rect_spritesheet_original, draggable, parent);
		break;
	case UIType::BUTTON:
		ret = new UIButton(type, local_pos, rect_spritesheet_original, draggable, parent);
		break;
	case UIType::INPUTTEXT:
		ret = new UIInputText(type, local_pos, rect_spritesheet_original, draggable, parent);
		break;
	case UIType::SCROLLBAR:
		ret = new UIScrollBar(type, local_pos, rect_spritesheet_original, draggable, parent);
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

UIImage* MGui::CreateUIImage(iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent)
{
	return (UIImage*)CreateUIObject(UIType::IMAGE, local_pos, rect_spritesheet_original, draggable, parent);
}

UIText* MGui::CreateUIText(iPoint local_pos, p2String text, bool draggable, UIObject* parent)
{
	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	UIText* ret = (UIText*)CreateUIObject(UIType::TEXT, local_pos, texture_rect, draggable, parent);

	ret->texture_text = texture_text;

	return ret;
}

UIButton* MGui::CreateUIButton(iPoint local_pos, p2String text, SDL_Rect image_rect, UIButtonType type, j1Module* listener, bool draggable, UIObject* parent)
{
	UIButton* ret = (UIButton*)CreateUIObject(UIType::BUTTON, local_pos, image_rect, draggable, parent);

	SDL_Texture* texture_text = App->font->Print(text.GetString());

	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	ret->text = new UIText(UIType::TEXT, iPoint{50,25}, texture_rect, false, ret);
	ret->text->texture_text = texture_text;
	ret->background = new UIImage(UIType::IMAGE, iPoint{0,0}, image_rect, false, ret);
	ret->hover = new UIImage(UIType::IMAGE, iPoint{ 0,0 }, SDL_Rect{ 411,169,229,69 }, false, ret);
	ret->clicked = new UIImage(UIType::IMAGE, iPoint{ 0,0 }, SDL_Rect{ 642,169,229,69 }, false, ret);
	ret->current_image = ret->background;
	ret->button_type = type;
	ret->listener = listener;
	return ret;
}

UIInputText* MGui::CreateUIInputText(iPoint local_pos, p2String text, SDL_Rect image_rect, bool draggable, UIObject* parent)
{
	UIInputText* ret = (UIInputText*)CreateUIObject(UIType::INPUTTEXT, local_pos, image_rect, draggable, parent);

	SDL_Texture* texture_text = App->font->Print(text.GetString());
	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	ret->text = new UIText(UIType::TEXT, iPoint{ 10,25 }, texture_rect, false, ret);
	ret->text->texture_text = texture_text;
	ret->input = new UIText(UIType::TEXT, iPoint{ 10,25 }, texture_rect, false, ret);

	ret->cursor_original_pos.x = local_pos.x + 10;
	ret->cursor_original_pos.y = local_pos.y + 25;
	ret->cursor.x = ret->cursor_original_pos.x;
	ret->cursor.y = ret->cursor_original_pos.y;
	ret->cursor.w = 1;
	ret->cursor.h = texture_rect.h;

	ret->current_text = ret->input;
	ret->background = new UIImage(UIType::BUTTON, iPoint{ 0,0 }, image_rect, false, ret);

	return ret;
}

UIScrollBar* MGui::CreateUIScrollBar(iPoint local_pos, SDL_Rect image_rect, Orientation orientation, UIObject* parent)
{
	UIScrollBar* ret = (UIScrollBar*)CreateUIObject(UIType::SCROLLBAR, local_pos, image_rect, false, parent);

	ret->orientation = orientation;


	ret->background = new UIImage(UIType::IMAGE, iPoint{ 0,0 }, image_rect, false, ret);
	if(ret->orientation == Orientation::VERTICAL)
		ret->thumb = new UIImage(UIType::IMAGE, iPoint{ -3,0 }, SDL_Rect{843, 330, 15, 10}, true, ret);
	else if (ret->orientation == Orientation::HORIZONTAL)
		ret->thumb = new UIImage(UIType::IMAGE, iPoint{ 0,-3 }, SDL_Rect{ 803, 518, 10, 15 }, true, ret);

	return ret;
}


// class Gui ---------------------------------------------------

