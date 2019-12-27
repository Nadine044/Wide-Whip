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

	if (focus)
	{
		if (timer_blink.Read() >= time_to_blink * 2)
		{
			timer_blink.Start();
		}
		else if (timer_blink.Read() <= time_to_blink)
		{
			App->render->DrawQuad(cursor, 255, 255, 255, 255, true, false);
		}

		App->render->DrawQuad(rect_world, 125, 125, 125, 125, true, false);
	}
	return true;
}

bool UIInputText::Update(float dt)
{
	UIObject::Update(dt);

	if (focus)
	{
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

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{
			if (cursor_int != 0)
			{
				DeleteInput();
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
		{
			SuppressInput();
		}

		if (App->input->GetKey(SDL_SCANCODE_HOME) == KEY_DOWN)
		{
			HomeInput();
		}

		if (App->input->GetKey(SDL_SCANCODE_END) == KEY_DOWN)
		{
			EndInput();
		}
	}
	return true;
}
bool UIInputText::CleanUp()
{
	UIObject::CleanUp();

	text->CleanUp();
	RELEASE(text);
	input->CleanUp();
	RELEASE(input);

	current_text = nullptr;

	background->CleanUp();
	RELEASE(background);

	return true;
}

void UIInputText::EndInput()
{
	cursor_int = text_string.Length();
	SetCursorPos(text_string);
}

void UIInputText::HomeInput()
{
	cursor_int = 0;
	cursor.x = cursor_original_pos.x;
}

void UIInputText::SuppressInput()
{
	Suppress();
	RecalculateStringTexture();
}

void UIInputText::Suppress()
{
	if (text_string.Length() != cursor_int)
	{
		p2String part1 = text_string;
		part1.Cut(cursor_int);
		p2String part2 = text_string;
		if (cursor_int != 0)
			part2.Cut(0, cursor_int);

		text_string = part1.GetString();
		part2.Cut(0,1);
		text_string += part2.GetString();

	}

}

void UIInputText::DeleteInput()
{
	Delete();
	RecalculateStringTexture();
}

void UIInputText::Delete()
{
	if (text_string.Length() != cursor_int)
	{
		p2String part1 = text_string;
		part1.Cut(cursor_int);
		p2String part2 = text_string;
		if (cursor_int != 0)
			part2.Cut(0, cursor_int);

		text_string = part1.GetString();
		text_string.Cut(--cursor_int);
		SetCursorPos(text_string);

		text_string += part2.GetString();

	}
	else
	{
		text_string.Cut(--cursor_int);
		SetCursorPos(text_string);
	}
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
	cursor_original_pos += mouse_move;
}

void UIInputText::WriteInput(char* input)
{
	if (GetVisible() && focus)
	{
		AddCharacter(input);
		RecalculateStringTexture();
	}

}

void UIInputText::RecalculateStringTexture()
{
	if (this->input->local_pos.x + this->input->rect_world.w < background->rect_world.w - 15/*margin right*/)
	{

	}
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

void UIInputText::SetFocusThis(bool focus_value)
{
	focus = focus_value;

	if (!focus && text_string.Length() == 0)
		current_text = text;
	
	else if (focus && current_text == text)
		current_text = input;
}