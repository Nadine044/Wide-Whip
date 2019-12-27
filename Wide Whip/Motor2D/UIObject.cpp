#include "UIObject.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "MGui.h"
#include "p2List.h"


UIObject::UIObject(iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : local_pos(local_pos), rect_spritesheet_original(rect_spritesheet_original), rect_spritesheet_final(rect_spritesheet_original), draggable(draggable), parent(parent)
{
	if (parent != nullptr)
	{
		world_pos_original = local_pos + parent->world_pos_final;
		parent->childrens.add(this);
	}

	else
		world_pos_original = local_pos;
	
	world_pos_final = world_pos_original;
	rect_world.x = world_pos_original.x;
	rect_world.y = world_pos_original.y;
	rect_world.w = rect_spritesheet_original.w;
	rect_world.h = rect_spritesheet_original.h;

}
bool UIObject::PreUpdate()
{
	if (MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
		App->gui->SetFocus(this);

	return true;
}
bool UIObject::PostUpdate(SDL_Texture* atlas)
{
	//App->render->DrawQuad(rect_world, 255, 0, 0, 200, true, false);
	App->render->Blit(atlas, world_pos_final.x, world_pos_final.y, &rect_spritesheet_final, 1.0f, SDL_FLIP_NONE, false);

	return true;
}

bool UIObject::CleanUp()
{
	//childrens.clear();
	parent = nullptr;
	return true;
}

bool UIObject::Update(float dt)
{
	if (draggable)
	{
		if (!dragging && MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			App->gui->SetDragging(this);
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

		if (parent)
		{
			if (world_pos_original.x < parent->world_pos_original.x)
			{
				rect_spritesheet_final.x = rect_spritesheet_original.x + (parent->world_pos_original.x - world_pos_original.x);
				rect_spritesheet_final.w = rect_spritesheet_original.w - (parent->world_pos_original.x - world_pos_original.x);
				world_pos_final.x = world_pos_original.x + (parent->world_pos_original.x - world_pos_original.x);
			}
			else if (world_pos_original.x + rect_spritesheet_original.w > parent->world_pos_original.x + parent->rect_spritesheet_final.w)
			{
				rect_spritesheet_final.w = rect_spritesheet_original.w - (world_pos_original.x + rect_spritesheet_original.w - (parent->world_pos_original.x + parent->rect_spritesheet_final.w));
			}
			else
			{
				world_pos_final.x = world_pos_original.x;
				rect_spritesheet_final.x = rect_spritesheet_original.x;
				rect_spritesheet_final.w = rect_spritesheet_original.w;
			}

			if (world_pos_original.y < parent->world_pos_original.y)
			{
				rect_spritesheet_final.y = rect_spritesheet_original.y + (parent->world_pos_original.y - world_pos_original.y);
				rect_spritesheet_final.h = rect_spritesheet_original.h - (parent->world_pos_original.y - world_pos_original.y);
				world_pos_final.y = world_pos_original.y + (parent->world_pos_original.y - world_pos_original.y);
			}
			else if (world_pos_original.y + rect_spritesheet_original.h > parent->world_pos_original.y + parent->rect_spritesheet_final.h)
			{
				rect_spritesheet_final.h = rect_spritesheet_original.h - (world_pos_original.y + rect_spritesheet_original.h - (parent->world_pos_original.y + parent->rect_spritesheet_final.h));
			}
			else
			{
				world_pos_final.y = world_pos_original.y;
				rect_spritesheet_final.y = rect_spritesheet_original.y;
				rect_spritesheet_final.h = rect_spritesheet_original.h;
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
	world_pos_original += mouse_move;
	rect_world.x += mouse_move.x;
	rect_world.y += mouse_move.y;
	world_pos_final += mouse_move;
}

const bool UIObject::MouseInRect() const
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	return !(mouse_pos.x >= (rect_world.x + rect_world.w) || mouse_pos.x <= rect_world.x || mouse_pos.y >= (rect_world.y + rect_world.h) || mouse_pos.y <= rect_world.y);
}

const bool UIObject::GetVisible() const
{
	return visible;
}

void UIObject::SetAllVisible(const bool visible)
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

		current_ui_object->data->SetVisible(visible);
		all_childrens.del(current_ui_object);
	}
}

void UIObject::SetVisible(const bool visible)
{
	this->visible = visible;
}

void UIObject::SetFocusThis(bool focus_value)
{
	focus = focus_value;
}