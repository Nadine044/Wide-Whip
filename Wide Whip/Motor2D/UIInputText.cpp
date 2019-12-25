#include "UIInputText.h"
#include "j1App.h"
#include "j1Render.h"
#include "UIText.h"
#include "UIImage.h"
#include "MFonts.h"
#include "p2Log.h"
#include "j1Input.h"


UIInputText::UIInputText(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent)
{ 
	type = _type; 
	timer_blink.Start(); 
}


bool UIInputText::PostUpdate(SDL_Texture* atlas)
{
	background->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	current_text->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	if (timer_blink.Read() >= time_to_blink * 2)
	{
		timer_blink.Start();
	}
	else if (timer_blink.Read() <= time_to_blink)
	{
		App->render->DrawQuad(cursor, 255, 255, 255, 255, true, false);
	}

	return true;
}

bool UIInputText::Update(float dt)
{
	UIObject::Update(dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		if (cursor_int != 0)
		{
			--cursor_int;

			p2String string_cuted = text_string;
			string_cuted.Cut(cursor_int);
			SetCursorPos(string_cuted);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		if (cursor_int != text_string.Length())
		{
			++cursor_int;

			p2String string_cuted = text_string;
			string_cuted.Cut(cursor_int);
			SetCursorPos(string_cuted);
		}
	}
	return true;
}

void UIInputText::SetCursorPos(p2String &string_cuted)
{
	int w, h;
	App->font->CalcSize(string_cuted.GetString(), w, h);

	cursor.x = cursor_original_pos.x + w;
}

void UIInputText::SetPos(iPoint & mouse_move)
{
	local_pos += mouse_move;
	world_pos_original += mouse_move;
	rect_world.x += mouse_move.x;
	rect_world.y += mouse_move.y;
	world_pos_final += mouse_move;
	cursor.x += mouse_move.x;
	cursor.y += mouse_move.y;
}

void UIInputText::GetInput(char* input)
{
	AddCharacter(input);

	this->input->texture_text = App->font->Print(text_string.GetString());
	int w, h;
	App->font->CalcSize(text_string.GetString(), w, h);

	this->input->rect_spritesheet_original.w = w;
	this->input->rect_spritesheet_original.h = h;

	this->input->rect_world.w = w;
	this->input->rect_world.h = h;

	this->input->rect_spritesheet_final = this->input->rect_spritesheet_original;
}

void UIInputText::AddCharacter(const char* input)
{
	if (text_string.Length() != cursor_int)
	{
		p2String part1 = text_string;
		part1.Cut(cursor_int);
		p2String part2 = text_string;
		if(cursor_int != 0)
		part2.Cut(0, cursor_int);

		text_string = part1.GetString();
		text_string += input;
		cursor_int++;
		SetCursorPos(text_string);

		text_string += part2.GetString();

	}
	else
	{
		text_string += input;
		cursor_int++;
		SetCursorPos(text_string);
	}
}
