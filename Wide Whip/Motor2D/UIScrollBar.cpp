#include "UIScrollBar.h"
#include "UIImage.h"

#include "j1App.h"
#include "j1Input.h"
bool UIScrollBar::PostUpdate(SDL_Texture* atlas)
{
	background->PostUpdate(atlas);
	thumb->PostUpdate(atlas);
	return true;
}

bool UIScrollBar::Update(float dt)
{

	if (!thumb->dragging && thumb->MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		thumb->dragging = true;
	}
	if (thumb->dragging)
	{
		iPoint mouse_move;
		App->input->GetMouseMotion(mouse_move.x, mouse_move.y);
		SetPosThumb(mouse_move);

		if (App->input->GetMouseButtonDown(1) == KEY_UP)
		{
			thumb->dragging = false;
		}
	}
	return true;
}

void UIScrollBar::SetPosThumb(iPoint& mouse_move)
{
	if (orientation == Orientation::VERTICAL)
	{
		if (thumb->world_pos_final.y + mouse_move.y < background->rect_world.y + background->rect_world.h - thumb->rect_world.h && thumb->world_pos_final.y + mouse_move.y > background->world_pos_final.y)
		{
			thumb->local_pos.y += mouse_move.y;
			thumb->world_pos_original.y += mouse_move.y;
			thumb->rect_world.y += mouse_move.y;
			thumb->world_pos_final.y += mouse_move.y;
		}
		else if(thumb->world_pos_final.y + mouse_move.y > background->rect_world.y + background->rect_world.h - thumb->rect_world.h)
		{
			uint pos_in_limit_y = background->rect_world.y + background->rect_world.h - thumb->rect_world.h;
			uint dif = pos_in_limit_y - thumb->world_pos_final.y;
			thumb->local_pos.y += dif;
			thumb->world_pos_original.y += dif;
			thumb->rect_world.y += dif;
			thumb->world_pos_final.y += dif;
		}
		else if (thumb->world_pos_final.y + mouse_move.y < background->world_pos_final.y)
		{
			uint dif = background->rect_world.y - thumb->world_pos_final.y;
			thumb->local_pos.y += dif;
			thumb->world_pos_original.y += dif;
			thumb->rect_world.y += dif;
			thumb->world_pos_final.y += dif;
		}
	}
}