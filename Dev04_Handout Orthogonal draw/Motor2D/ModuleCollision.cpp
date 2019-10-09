#include "j1App.h"
#include "j1Render.h"
#include "ModuleCollision.h"
#include "j1Input.h" // temporally
#include "j1Player.h" // wil be replace by object
//#include "p2Log.h"
//--------------------COLLIDER---------------------------

Collider::Collider(iPoint pos, int w, int h, TAG tag, bool dynamic) : tag(tag), dynamic(dynamic)
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

void Collider::UpdatePos(const iPoint pos)
{
	rect.x = pos.x;
	rect.y = pos.y;
}

const bool Collider::IsToDelete() const
{
	return to_delete;
}

void Collider::Remove()
{
	to_delete = true;
}
//--------------------MODULE COLLISION---------------------------

Collider* ModuleCollision::AddCollider(iPoint pos, int w, int h, TAG tag, bool dynamic)
{
	Collider * ret = new Collider(pos, w, h, tag, dynamic);
	if(!dynamic)
		colliders_static_list.add(ret);

	else
	{
		colliders_dynamic_list.add(ret);
	}

	return ret;
}

bool ModuleCollision::Start()
{
	
	return true;
}

bool ModuleCollision::PreUpdate()
{

	//check if a collider to_remove is true, delete it.


	//check for all.
	DeleteCollidersToRemove();


	return true;
}

void ModuleCollision::DeleteCollidersToRemove()
{
	// delete colliders to delete of the list. only for the preupdate and cleanUp.
	if (colliders_dynamic_list.count() != 0)
	{
		p2List_item<Collider*>* collider1 = nullptr;

		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next) //start from the end, where the dynamics are.
		{
			if (collider1->data->IsToDelete())
			{
				colliders_dynamic_list.del(collider1);
				RELEASE(collider1->data);	
			}
		}
	}

	if (colliders_static_list.count() != 0)
	{
		p2List_item<Collider*>* collider1 = nullptr;

		for (collider1 = colliders_static_list.start; collider1; collider1 = collider1->next) //start from the end, where the dynamics are.
		{
			if (collider1->data->IsToDelete())
			{
				colliders_static_list.del(collider1);
				RELEASE(collider1->data);
			}
		}
	}
}


bool ModuleCollision::Update(float dt)
{

	//the input of the movement must be before this


	// this don't check dynamics with dynamics (statics with statics no sense).
	if (colliders_dynamic_list.count() != 0)//TODO test
	{
		p2List_item<Collider*>* collider1 = nullptr;
		p2List_item<Collider*>* collider2 = nullptr;

		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next)
		{
			for (collider2 = colliders_static_list.start; collider2; collider2 = collider2->next)
			{
				if (collider1->data->CheckColision(collider2->data))
				{
					OverlapDS(collider1->data, collider2->data);
				}
			}
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
			colliders_static_list.start->data->Remove(); // only for testing
	
	return true;
}

bool ModuleCollision::PostUpdate()
{
	if (debug)
	{
		p2List_item<Collider*>* collider1 = nullptr;
		p2List_item<Collider*>* collider2 = nullptr;

		//Draw statics colliders
		for (collider1 = colliders_static_list.start; collider1; collider1 = collider1->next)
		{
			App->render->DrawQuad(collider1->data->rect, 255, 0, 0, 100);
		}

		//Draw Dynamic colliders
		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next)
		{
			App->render->DrawQuad(collider1->data->rect, 255, 0, 0, 100);
		}

		
	}
	return true;
}

bool ModuleCollision::CleanUp()
{
	//TODO nad delete
	SetAllCollidersToDelete();
	DeleteCollidersToRemove();

	return true;
}

void ModuleCollision::SetAllCollidersToDelete()
{
	if (colliders_dynamic_list.count() != 0)
	{
		p2List_item<Collider*>* collider1 = nullptr;

		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next) //start from the end, where the dynamics are.
		{
			collider1->data->Remove();
		}
	}

	if (colliders_static_list.count() != 0)
	{
		p2List_item<Collider*>* collider1 = nullptr;

		for (collider1 = colliders_static_list.start; collider1; collider1 = collider1->next) //start from the end, where the dynamics are.
		{
			collider1->data->Remove();
		}
	}
}

void ModuleCollision::OverlapDS(Collider* c_dynamic, Collider* c_static)
{
	//border widths of the collision:
	int distances[(int)DISTANCE_DIR::MAX];
	distances[(int)DISTANCE_DIR::LEFT] = c_dynamic->rect.x + c_dynamic->rect.w - c_static->rect.x;
	distances[(int)DISTANCE_DIR::RIGHT] = c_static->rect.x + c_static->rect.w - c_dynamic->rect.x;
	distances[(int)DISTANCE_DIR::UP] = c_dynamic->rect.y + c_dynamic->rect.h - c_static->rect.y;
	distances[(int)DISTANCE_DIR::DOWN] = c_static->rect.y + c_static->rect.h - c_dynamic->rect.y;

	int overlap_dir = 0;

	for (int i = 0; i < (int)DISTANCE_DIR::MAX; ++i)
	{
		if (distances[i] < distances[(int)overlap_dir])
		{
			overlap_dir = i;  //the final dir is the dir with minor distance
		}
	}

	switch ((DISTANCE_DIR)overlap_dir)
	{
	case DISTANCE_DIR::LEFT:
		c_dynamic->object->pos.x = c_static->rect.x - c_dynamic->rect.w;
		break;
	case DISTANCE_DIR::RIGHT:
		c_dynamic->object->pos.x = c_static->rect.x + c_static->rect.w;
		break;
	case DISTANCE_DIR::UP:
		c_dynamic->object->pos.y = c_static->rect.y - c_dynamic->rect.h;
		break;
	case DISTANCE_DIR::DOWN:
		c_dynamic->object->pos.y = c_static->rect.y + c_static->rect.h;
		break;
	}

	c_dynamic->UpdatePos(c_dynamic->object->pos);
}
