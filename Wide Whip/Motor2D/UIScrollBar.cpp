#include "UIScrollBar.h"
#include "UIImage.h"

#include "j1App.h"
#include "MGui.h"
#include "j1Render.h"
#include "j1Input.h"
bool UIScrollBar::PostUpdate(SDL_Texture* atlas)
{
	background->PostUpdate(atlas);
	thumb->PostUpdate(atlas);
	if (focus)
		App->render->DrawQuad(rect_world, 125, 125, 125, 125, true, false);
	return true;
}

bool UIScrollBar::Update(float dt)
{

	if (!thumb->dragging && thumb->MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		App->gui->SetDragging(this);
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

bool UIScrollBar::CleanUp()
{
	UIObject::CleanUp();

	background->CleanUp();
	RELEASE(background);
	thumb->CleanUp();
	RELEASE(thumb);

	listener = nullptr;

	return true;
}

void UIScrollBar::SetPosThumb(iPoint& mouse_move)
{
	if (orientation == Orientation::VERTICAL && mouse_move.y != 0)
	{
		MoveThumbAxisY(mouse_move.y);

		float pos_in_limit_y_max = background->rect_world.h - thumb->rect_world.h;
		float ratio = thumb->local_pos.y / pos_in_limit_y_max;

		listener->SliderEvent(scrollbar_type, ratio);
	}
	else if (orientation == Orientation::HORIZONTAL && mouse_move.x != 0)
	{
		MoveThumbAxisX(mouse_move.x);

		float pos_in_limit_y_max = background->rect_world.w - thumb->rect_world.w;
		float ratio = thumb->local_pos.x / pos_in_limit_y_max;

		listener->SliderEvent(scrollbar_type, ratio);

	}
}

void UIScrollBar::MoveThumbAxisX(uint mouse_move_x)
{
	if (thumb->world_pos_final.x + mouse_move_x < background->rect_world.x + background->rect_world.w - thumb->rect_world.w && thumb->world_pos_final.x + mouse_move_x > background->world_pos_final.x)
	{
		thumb->local_pos.x += mouse_move_x;
		thumb->world_pos_original.x += mouse_move_x;
		thumb->rect_world.x += mouse_move_x;
		thumb->world_pos_final.x += mouse_move_x;
	}
	else if (thumb->world_pos_final.x + mouse_move_x > background->rect_world.x + background->rect_world.w - thumb->rect_world.w)
	{
		uint pos_in_limit_x = background->rect_world.x + background->rect_world.w - thumb->rect_world.w;
		uint dif = pos_in_limit_x - thumb->world_pos_final.x;
		thumb->local_pos.x += dif;
		thumb->world_pos_original.x += dif;
		thumb->rect_world.x += dif;
		thumb->world_pos_final.x += dif;
	}
	else if (thumb->world_pos_final.x + mouse_move_x < background->world_pos_final.x)
	{
		uint dif = background->rect_world.x - thumb->world_pos_final.x;
		thumb->local_pos.x += dif;
		thumb->world_pos_original.x += dif;
		thumb->rect_world.x += dif;
		thumb->world_pos_final.x += dif;
	}
}

void UIScrollBar::MoveThumbAxisY(uint mouse_move_y)
{
	if (thumb->world_pos_final.y + mouse_move_y < background->rect_world.y + background->rect_world.h - thumb->rect_world.h && thumb->world_pos_final.y + mouse_move_y > background->world_pos_final.y)
	{
		thumb->local_pos.y += mouse_move_y;
		thumb->world_pos_original.y += mouse_move_y;
		thumb->rect_world.y += mouse_move_y;
		thumb->world_pos_final.y += mouse_move_y;
	}
	else if (thumb->world_pos_final.y + mouse_move_y > background->rect_world.y + background->rect_world.h - thumb->rect_world.h)
	{
		uint pos_in_limit_y = background->rect_world.y + background->rect_world.h - thumb->rect_world.h;
		uint dif = pos_in_limit_y - thumb->world_pos_final.y;
		thumb->local_pos.y += dif;
		thumb->world_pos_original.y += dif;
		thumb->rect_world.y += dif;
		thumb->world_pos_final.y += dif;
	}
	else if (thumb->world_pos_final.y + mouse_move_y < background->world_pos_final.y)
	{
		uint dif = background->rect_world.y - thumb->world_pos_final.y;
		thumb->local_pos.y += dif;
		thumb->world_pos_original.y += dif;
		thumb->rect_world.y += dif;
		thumb->world_pos_final.y += dif;
	}
}
