#include "UIInputText.h"
#include "j1App.h"
#include "j1Render.h"
#include "UIText.h"
#include "UIImage.h"

bool UIInputText::PostUpdate(SDL_Texture* atlas)
{
	background->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	current_text->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	App->render->DrawQuad(cursor, 255, 255, 255, 255, true, false);
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