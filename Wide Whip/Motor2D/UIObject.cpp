#include "UIObject.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "MGui.h"
#include "p2List.h"


UIObject::UIObject(iPoint local_pos, SDL_Rect rect_spritesheet, bool draggable, UIObject* parent) : local_pos(local_pos), rect_spritesheet(rect_spritesheet), draggable(draggable), parent(parent)
{
	if (parent != nullptr)
	{
		world_pos = local_pos + parent->local_pos;
		parent->childrens.add(this);
	}

	else
		world_pos = local_pos;
	
	rect_world.x = world_pos.x;
	rect_world.y = world_pos.y;
	rect_world.w = rect_spritesheet.w;
	rect_world.h = rect_spritesheet.h;
}

bool UIObject::PostUpdate(SDL_Texture* atlas)
{
	App->render->Blit(atlas, world_pos.x, world_pos.y, &rect_spritesheet, 1.0f, SDL_FLIP_NONE, false);

	return true;
}

bool UIObject::Update(float dt)
{
	if (draggable)
	{
		if (!dragging && MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			dragging = true;
			UIObject* current = this;
			while(current->parent)
			{
				current = current->parent;
				current->dragging = false;
			}
		}

		if (dragging)
		{
			iPoint mouse_move;
			App->input->GetMouseMotion(mouse_move.x, mouse_move.y);
			SetAllPos(mouse_move);
			


			if (App->input->GetMouseButtonDown(1) == KEY_UP)
			{
				dragging = false;
			}
		}
	}
	return true;
}

void UIObject::SetAllPos(iPoint &mouse_move)
{
	if (childrens.count() > 0)
	{
		p2List<UIObject*> all_childrens;
		all_childrens.add(this);

		while (all_childrens.count() != 0)
		{
			p2List_item<UIObject*>* current_ui_object = all_childrens.start;

			for (p2List_item<UIObject*>* item = current_ui_object->data->childrens.start; item; item = item->next)
			{
				all_childrens.add(item->data);
			}

			current_ui_object->data->SetPos(mouse_move);
			all_childrens.del(current_ui_object);
		}
	}
	else
		SetPos(mouse_move);
	
}

void UIObject::SetPos(iPoint & mouse_move)
{
	local_pos += mouse_move;
	world_pos += mouse_move;
	rect_world.x += mouse_move.x;
	rect_world.y += mouse_move.y;
}

const bool UIObject::MouseInRect() const
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	return !(mouse_pos.x >= (rect_world.x + rect_world.w) || mouse_pos.x <= rect_world.x || mouse_pos.y >= (rect_world.y + rect_world.h) || mouse_pos.y <= rect_world.y);
}