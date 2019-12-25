#include "UIInputText.h"
#include "j1App.h"
#include "j1Render.h"
#include "UIText.h"
#include "UIImage.h"
#include "MFonts.h"
#include "p2Log.h"
#include "j1Input.h"

bool UIInputText::PostUpdate(SDL_Texture* atlas)
{
	background->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	current_text->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	App->render->DrawQuad(cursor, 255, 255, 255, 255, true, false);
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
			SDL_Texture* texture_text_cuted = App->font->Print(string_cuted.GetString());

			SDL_Rect texture_rect;
			texture_rect.x = 0;
			texture_rect.y = 0;
			SDL_QueryTexture(texture_text_cuted, NULL, NULL, &texture_rect.w, &texture_rect.h);

			cursor.x = cursor_original_pos.x + texture_rect.w;

		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		if (cursor_int != 0)
		{
			++cursor_int;

			p2String string_cuted = text_string;
			string_cuted.Cut(cursor_int);
			SDL_Texture* texture_text_cuted = App->font->Print(string_cuted.GetString());

			SDL_Rect texture_rect;
			texture_rect.x = 0;
			texture_rect.y = 0;
			SDL_QueryTexture(texture_text_cuted, NULL, NULL, &texture_rect.w, &texture_rect.h);

			cursor.x = cursor_original_pos.x + texture_rect.w;

		}
	}
	return true;
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
	//text_string += input;
	AddCharacter(input);

	this->input->texture_text = App->font->Print(text_string.GetString());
	SDL_Rect texture_rect;
	texture_rect.x = 0;
	texture_rect.y = 0;
	SDL_QueryTexture(this->input->texture_text, NULL, NULL, &texture_rect.w, &texture_rect.h);

	this->input->rect_spritesheet_original.w = texture_rect.w;
	this->input->rect_spritesheet_original.h = texture_rect.h;

	this->input->rect_world.w = texture_rect.w;
	this->input->rect_world.h = texture_rect.h;

	this->input->rect_spritesheet_final = this->input->rect_spritesheet_original;

	cursor.x = cursor_original_pos.x + texture_rect.w;

}

void UIInputText::AddCharacter(const char* input)
{
	if (text_string.Length() != cursor_int)
	{
		p2String part1 = text_string;
		part1.Cut(cursor_int);
		p2String part2 = text_string;
		part2.Cut(0, cursor_int);

		text_string = part1.GetString();
		text_string += input;
		text_string += part2.GetString();
		cursor_int++;
	}
	else
	{
		text_string += input;
		cursor_int++;
	}
}
