#include "UIObject.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "MGui.h"


UIObject::UIObject(iPoint pos, SDL_Rect rect_spritesheet, bool draggable) : pos(pos), rect_spritesheet(rect_spritesheet), draggable(draggable)
{
	rect_world.x = pos.x;
	rect_world.y = pos.y;
	rect_world.w = rect_spritesheet.w;
	rect_world.h = rect_spritesheet.h;
}

bool UIObject::PostUpdate(SDL_Texture* atlas)
{
	App->render->Blit(atlas, pos.x, pos.y, &rect_spritesheet, 1.0f, SDL_FLIP_NONE, false);

	return true;
}

bool UIObject::Update(float dt)
{
	if (draggable)
	{
		if (!dragging && MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			dragging = true;
		}

		if (dragging)
		{
			iPoint mouse_move;
			App->input->GetMouseMotion(mouse_move.x, mouse_move.y);
			pos += mouse_move;
			rect_world.x += mouse_move.x;
			rect_world.y += mouse_move.y;

			if (App->input->GetMouseButtonDown(1) == KEY_UP)
			{
				dragging = false;
			}
		}
	}
	return true;
}

const bool UIObject::MouseInRect() const
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	return !(mouse_pos.x >= (rect_world.x + rect_world.w) || mouse_pos.x <= rect_world.x || mouse_pos.y >= (rect_world.y + rect_world.h) || mouse_pos.y <= rect_world.y);
}