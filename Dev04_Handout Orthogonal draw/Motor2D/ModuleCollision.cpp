#include "j1App.h"
#include "j1Render.h"
#include "ModuleCollision.h"
#include "j1Input.h" // temporally
#include "p2Log.h"
//--------------------COLLIDER---------------------------

Collider::Collider(fPoint pos, float w, float h, TAG tag) : tag(tag)
{
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = w;
	rect.h = h;
}

bool Collider::CheckColision(const Collider* coll) const
{
	return !(coll->rect.x >= (rect.x + rect.w) || (coll->rect.x + coll->rect.w) <= rect.x || coll->rect.y >= (rect.y + rect.h) || (coll->rect.y + coll->rect.h) <= rect.y);
}
//--------------------MODULE COLLISION---------------------------

Collider* ModuleCollision::AddCollider(fPoint pos, float w, float h, TAG tag)
{
	Collider * ret = new Collider(pos, w, h, tag);
	colliders_list.add(ret);

	return ret;
}

bool ModuleCollision::Update(float dt)
{

	p2List_item<Collider*>* collider1;
	p2List_item<Collider*>* collider2;

	for (collider1 = colliders_list.start; collider1; collider1 = collider1->next)
	{
		for (collider2 = collider1->next; collider2; collider2 = collider2->next)
		{
			if (collider1->data->CheckColision(collider2->data))
			{
				LOG("COLISIOON");
			}
		}
	}
	

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		player->rect.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		player->rect.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		player->rect.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		player->rect.x += 1;

	if (debug)
	{
		for (collider1 = colliders_list.start; collider1; collider1 = collider1->next)
		{
			App->render->DrawQuad(collider1->data->rect,255, 0, 0);
		}	
	}

	return true;
}