#include "EnemyWalk.h"
#include "p2DynArray.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "ModuleCollision.h"
#include "p2SString.h"

EnemyWalk::EnemyWalk(SDL_Rect& rect) : Enemy(EntityType::WALKENEMY, rect) 
{};

bool EnemyWalk::Awake(const pugi::xml_node& walker)
{
	LOG("Loading Walker Parser");
	bool ret = true;

	pugi::xml_node enemy_node = walker.child("enemy");

	Enemy::Awake(walker);
	text_path = enemy_node.child_value("texture_walker");

	idle.LoadAnimation(enemy_node.child("animations").child("idle"));
	walk.LoadAnimation(enemy_node.child("animations").child("walk"));
	
	slime_death.path = enemy_node.child("audios").child_value("slime_death");
	slime_death.id = App->audio->LoadFx(slime_death.path.GetString());

	return ret;
}

void EnemyWalk::GoToNextPoint(float dt)
{

	const iPoint* next_point = path.At(path.Count() - 3); // is Array. So Count() -1 are the last. Pick the next because the last is so close (collider enemy is bigger than 1 tile) 

	if (next_point != nullptr)
	{
		int final_speed = speed * dt;

		const iPoint* current_point = path.At(path.Count() - 1); // path is array. Start from 0. Last is Count()-1. Current is the last cause the path start from the destination (player) to origin (enemy). 

		const iPoint* next_point_x = next_point; //next point in axis x.
		//const iPoint* next_point_y = next_point; //next point in axis y.

		if (next_point->x == current_point->x) // If it's the same x, calcule the new point in x and assign it if exist. 
		{
			const iPoint* next = App->pathfinding_module->GetNextHorizontalPoint(path, current_point);
			if (next != nullptr)
				next_point_x = next;
		}

		// ---------------MOVEMENT-------------------
		// Horizontal
		iPoint enemy_pivot_central_down = pos;
		enemy_pivot_central_down.x += col->rect.w * 0.5f;
		enemy_pivot_central_down.y += col->rect.h;

		iPoint enemy_pivot_central_down_map = App->map->WorldToMap(enemy_pivot_central_down);

		current_animation = &walk;

		if (enemy_pivot_central_down_map.x < next_point_x->x)
		{
			// Right
			if (ground != nullptr)
			{
				if (ground->rect.x + ground->rect.w > pos.x + col->rect.w)
				{
					flip = SDL_FLIP_HORIZONTAL;
					pos.x += final_speed;
				}
			}
			else
			{
				flip = SDL_FLIP_HORIZONTAL;
				pos.x += final_speed;
			}
		}
		else if (enemy_pivot_central_down_map.x > next_point_x->x)		
		{
			// Left
			if (ground != nullptr)
			{
				if (ground->rect.x < pos.x)
					pos.x -= final_speed;
				if(flip == SDL_FLIP_HORIZONTAL)
					flip = SDL_FLIP_NONE;
			}
			else
			{
				if (flip == SDL_FLIP_HORIZONTAL)
					flip = SDL_FLIP_NONE;
				pos.x -= final_speed;

			}
				
		}
	}
	
}

void EnemyWalk::OnTrigger(Collider* col2)
{
	Enemy::OnTrigger(col2);
	if (col2->tag == TAG::WALL && col->last_colision_direction == DISTANCE_DIR::UP)
	{
		velocity = 0.f;
		ground = col2;
	}
	if (col2->tag == TAG::PLAYER)
	{
		LOG("Walker Enemy attacking Player");
	}
}

bool EnemyWalk::Update(float dt)
{
	Enemy::Update(dt);

	Gravity();

	return true;
}

void EnemyWalk::Gravity()
{
	velocity -= gravity;
	pos.y += -velocity;
}
