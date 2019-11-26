#include "j1App.h"
#include "j1Render.h"
#include "ModuleCollision.h"
#include "j1Input.h" //temporally
#include "j1Player.h" // wil be replace by object
#include "Enemy.h"
//#include "p2Log.h"
//--------------------COLLIDER---------------------------

Collider::Collider(iPoint pos, int w, int h, TAG tag, Color color, Entity* parent, bool dynamic) : tag(tag), dynamic(dynamic), color(color), object(parent)
{
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = w;
	rect.h = h;
	enable = true;
}

bool Collider::CheckColision(const Collider* coll) const
{
	return !(coll->rect.x >= (rect.x + rect.w) || (coll->rect.x + coll->rect.w) <= rect.x || coll->rect.y >= (rect.y + rect.h) || (coll->rect.y + coll->rect.h) <= rect.y);
}

bool Collider::CheckColisionBottom(const Collider* coll) const
{
	return ((coll->rect.y < (rect.y + rect.h)) && !(coll->rect.x >= (rect.x + rect.w)) && !((coll->rect.x + coll->rect.w) <= rect.x));
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

Color Collider::GetColor() const
{
	return color;
}

bool Collider::IsEnabled() const
{
	return enable;
}

void Collider::Enable()
{
	enable = true;
}

void Collider::Disable()
{
	enable = false;
}
//--------------------MODULE COLLISION---------------------------

ModuleCollision::ModuleCollision()
{
	name.create("collision");
}

bool ModuleCollision::Awake(pugi::xml_node& config)
{
	alpha_debug = (Uint8)(config.child("alpha_debug").attribute("value").as_uint());

	return true;
}

Collider* ModuleCollision::AddCollider(iPoint pos, int w, int h, TAG tag, Color color, Entity* parent, bool dynamic)
{
	Collider * ret = new Collider(pos, w, h, tag, color, parent, dynamic);
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
	for(uint i = 0u; i < (uint)TAG::MAX; ++i)
	{
		for (uint j = 0u; j < (uint)TAG::MAX; ++j)
		{
			physics_matrix[i][j] = false;
			trigger_matrix[i][j] = false;
		}
	}

	//TRIGGER MATRIX to OnTrigger.
		//The collider dynamic first in the matrxi. If the two colliders are dynamic, it's necessary to set the matrix true with the 2 cases: [dynamic1][dynamic2] = true and [dynamic2][dynamic1] = true. With dynamic vs static only on time with the dynamic first.
	trigger_matrix[(uint)TAG::PLAYER][(uint)TAG::WALL] = true;
	trigger_matrix[(uint)TAG::PLAYER][(uint)TAG::PLATFORM] = true;
	trigger_matrix[(uint)TAG::PLAYER][(uint)TAG::WATER] = true;
	trigger_matrix[(uint)TAG::PLAYER][(uint)TAG::CHANGE_LEVEL] = true;
	trigger_matrix[(uint)TAG::PLAYER][(uint)TAG::ENEMY] = true;

	//ENEMY TAG
	trigger_matrix[(uint)TAG::ENEMY][(uint)TAG::WALL] = true;
	trigger_matrix[(uint)TAG::ENEMY][(uint)TAG::PLATFORM] = true;
	trigger_matrix[(uint)TAG::ENEMY][(uint)TAG::WATER] = true;
	trigger_matrix[(uint)TAG::ENEMY][(uint)TAG::CHANGE_LEVEL] = true;
	trigger_matrix[(uint)TAG::ENEMY][(uint)TAG::PLAYER] = true;
	

	//PHYSICS MATRIX to overlap.
		// Functions OnTrigger will be called only in the first collider. If want to call the function OnTrigger in the two colliders, set the marix bool with the invers too([TAG1][TAG2] = true and [TAG2][TAG1] = true).
	physics_matrix[(uint)TAG::PLAYER][(uint)TAG::WALL] = true;
	physics_matrix[(uint)TAG::PLAYER][(uint)TAG::PLATFORM] = true;
	physics_matrix[(uint)TAG::PLAYER][(uint)TAG::CHANGE_LEVEL] = true;

	//PHYSICS MATRIX to overlap WALKABLE ENEMY
	physics_matrix[(uint)TAG::ENEMY][(uint)TAG::WALL] = true;
	physics_matrix[(uint)TAG::ENEMY][(uint)TAG::PLATFORM] = true;
	physics_matrix[(uint)TAG::ENEMY][(uint)TAG::CHANGE_LEVEL] = true;
	
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

	//Dynamic colliders
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

	//Statics colliders
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



	// check and do OnTrigger and Overlap:
	// this don't check dynamics with dynamics (statics with statics no sense). Only dynamics with statics in this order.
	if (colliders_dynamic_list.count() != 0)
	{
		p2List_item<Collider*>* collider1 = nullptr;
		p2List_item<Collider*>* collider2 = nullptr;

		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next)
		{
			collider1->data->last_colision_direction = DISTANCE_DIR::NONE;
			if (collider1->data->IsEnabled())
			{
				for (collider2 = colliders_static_list.start; collider2; collider2 = collider2->next)
				{
					if (collider2->data->IsEnabled())
					{
						if (collider1->data->CheckColision(collider2->data))
						{

							//don't do and don't check OnTrigger in the invers order because this is Dynamic vs Static but in Dynamics vs Dynamics  it's necessary check and do in the invers order too before the overlap.
							//(it's not necessary do before the overlap because OnTrigger and overlap is doing when a collision is detected. if the collision is detected two times, to OnTrigger and to Overlap separated, it will be important the order. First all OnTrigger in two orders and then the overlap).
							if (physics_matrix[(uint)collider1->data->tag][(uint)collider2->data->tag])
							{
								if (collider2->data->tag == TAG::PLATFORM)
								{
									collider1->data->last_colision_direction = OverlapPlatform(collider1->data, collider2->data);
								}
								else
								{
									collider1->data->last_colision_direction = OverlapDS(collider1->data, collider2->data);
								}
								collider2->data->first_time_collision = false;
							}

							if (trigger_matrix[(uint)collider1->data->tag][(uint)collider2->data->tag])
								collider1->data->object->OnTrigger(collider2->data); //mytodo


						}
						else
						{
							collider2->data->first_time_collision = true;
						}
					}
				}
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;
	
	
	return true;
}

bool ModuleCollision::PostUpdate()
{
	if (debug)
	{
		p2List_item<Collider*>* collider1 = nullptr;

		//Draw statics colliders
		for (collider1 = colliders_static_list.start; collider1; collider1 = collider1->next)
		{
			Color col_color;
			collider1->data->IsEnabled() ? (col_color = collider1->data->GetColor()) : (col_color = Grey);

			App->render->DrawQuad(collider1->data->rect, col_color.r, col_color.g, col_color.b, alpha_debug);
			
		}

		//Draw Dynamic colliders
		for (collider1 = colliders_dynamic_list.start; collider1; collider1 = collider1->next)
		{
			Color col_color;
			collider1->data->IsEnabled() ? (col_color = collider1->data->GetColor()) : (col_color = Grey);

			App->render->DrawQuad(collider1->data->rect, col_color.r, col_color.g, col_color.b, alpha_debug);
		}
	}
	return true;
}

bool ModuleCollision::CleanUp()
{
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

DISTANCE_DIR ModuleCollision::OverlapDS(Collider* c_dynamic, Collider* c_static)
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
	if (player->object->type == EntityType::PLAYER)
	{
		j1Player* _player = (j1Player*)player->object;
		_player->UpdateCameraPos();
	}

	return (DISTANCE_DIR)overlap_dir;
}

DISTANCE_DIR ModuleCollision::OverlapPlatform(Collider* c_dynamic, Collider* c_static)
{
	int overlap_dir = 0;
	if (c_static->first_time_collision)
	{
		//border widths of the collision:
		int distances[(int)DISTANCE_DIR::MAX];
		distances[(int)DISTANCE_DIR::LEFT] = c_dynamic->rect.x + c_dynamic->rect.w - c_static->rect.x;
		distances[(int)DISTANCE_DIR::RIGHT] = c_static->rect.x + c_static->rect.w - c_dynamic->rect.x;
		distances[(int)DISTANCE_DIR::UP] = c_dynamic->rect.y + c_dynamic->rect.h - c_static->rect.y;
		distances[(int)DISTANCE_DIR::DOWN] = c_static->rect.y + c_static->rect.h - c_dynamic->rect.y;

		


		for (int i = 0; i < (int)DISTANCE_DIR::MAX; ++i)
		{
			if (distances[i] < distances[(int)overlap_dir])
			{
				overlap_dir = i;  //the final dir is the dir with minor distance
			}
		}
		c_static->last_colision_direction = (DISTANCE_DIR)overlap_dir;
	}
	else
	{
		overlap_dir = (int)c_static->last_colision_direction;
	}

	if ((DISTANCE_DIR)overlap_dir == DISTANCE_DIR::UP && player->object->GetVelocity() <= 0.0f)
	{
		float l = player->object->GetVelocity();

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

		if (player->object->type == EntityType::PLAYER)
		{
			j1Player* _player = (j1Player*)player->object;
			_player->UpdateCameraPos();
		}
		
	}
	return (DISTANCE_DIR)overlap_dir;
}

Uint8 ModuleCollision::GetAlphaDebug() const
{
	return alpha_debug;
}