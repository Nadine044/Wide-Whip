#include "EnemyFly.h"
#include "j1App.h"
#include "ModuleEntityManager.h"
#include "ModuleCollision.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

EnemyFly::EnemyFly(SDL_Rect& rect) : Enemy(EntityType::FLYENEMY, rect) {};

void EnemyFly::GoToNextPoint()
{
	const p2DynArray<iPoint>* path = App->pathfinding_module->GetLastPath();
	if (path != nullptr)
	{
		const iPoint* next_point = path->At(path->Count()-3); // is Array. So Count() -1 are the last. Pick the next because the last is so close (collider enemy is bigger than 1 tile) 

		if (next_point != nullptr)
		{

			if (in_collision)
			{
				MoveInCollision(next_point);
			}
			else
			{
				MoveNormal(path, next_point);
			}
		}
	}
}

void EnemyFly::MoveNormal(const p2DynArray<iPoint> * path, const iPoint * next_point)
{
	// Normal movement (tiles)
	// All coordinates in map.

	const iPoint* current_point = path->At(path->Count() - 1); // path is array. Start from 0. Last is Count()-1. Current is the last cause the path start from the destination (player) to origin (enemy). 

	const iPoint* next_point_x = next_point; //next point in axis x.
	const iPoint* next_point_y = next_point; //next point in axis y.

	if (next_point->x == current_point->x) // If it's the same x, calcule the new point in x and assign it if exist. 
	{
		const iPoint* next = App->pathfinding_module->GetNextHorizontalPoint(current_point);
		if (next != nullptr)
			next_point_x = next;
	}
	if (next_point->y == current_point->y) // If it's the same y, calcule the new point in y and assign it if exist. 
	{
		const iPoint* next = App->pathfinding_module->GetNextVerticalPoint(current_point);
		if (next != nullptr)
			next_point_y = next;
	}

	//Enemy in map_pos
	iPoint enemy_central_pos = pos;
	enemy_central_pos.x += col->rect.w * 0.5f;
	enemy_central_pos.y += col->rect.h * 0.5f;

	iPoint pos_central_map = App->map->WorldToMap(enemy_central_pos);


	// ---------------MOVEMENT-------------------
	// Horizontal
	if (pos_central_map.x < next_point_x->x)			// Right
		pos.x += speed;
	else if (pos_central_map.x > next_point_x->x)		// Left
		pos.x -= speed;

	// Vertical
	if (pos_central_map.y < next_point_y->y)			// Down
		pos.y += speed;
	else if (pos_central_map.y > next_point_y->y)		// Up
		pos.y -= speed;

}

void EnemyFly::MoveInCollision(const iPoint * next_point)
{
	// The next declarations are in word coordinates (pixels)
	iPoint next_point_world = App->map->MapToWorld(*next_point);


	//next_pont pivots
	iPoint next_point_pivot_central = next_point_world;
	next_point_pivot_central.x += App->map->data.tile_width * 0.5f;
	next_point_pivot_central.y += App->map->data.tile_height * 0.5f;

	iPoint next_point_pivot_down_right = next_point_world;
	next_point_pivot_down_right.x += App->map->data.tile_width;
	next_point_pivot_down_right.y += App->map->data.tile_height;


	//enemy pivots
	iPoint enemy_central_pos = pos;
	enemy_central_pos.x += col->rect.w * 0.5f;
	enemy_central_pos.y += col->rect.h * 0.5f;

	iPoint enemy_down_right_pos = pos;
	enemy_down_right_pos.x += col->rect.w;
	enemy_down_right_pos.y += col->rect.h;


	// ---------------MOVEMENT-------------------
	// Horizontal----
	if (HorizontalDirection == HorizontalMovementDirection::NO_DIRECTION)
	{
		// If in the previous frame it did not move in x, calculate the new address from the central positions.
		if (next_point_pivot_central.x > enemy_central_pos.x)
			HorizontalDirection = HorizontalMovementDirection::RIGHT;

		else if (next_point_pivot_central.x < enemy_central_pos.x)
			HorizontalDirection = HorizontalMovementDirection::LEFT;
	}


	if (HorizontalDirection == HorizontalMovementDirection::LEFT)
	{
		// Left
		if (next_point_pivot_down_right.x < enemy_down_right_pos.x)
			pos.x -= speed;

		else
			HorizontalDirection = HorizontalMovementDirection::NO_DIRECTION;
	}
	else if (HorizontalDirection == HorizontalMovementDirection::RIGHT)
	{
		// Right
		if (next_point_world.x > pos.x)
			pos.x += speed;

		else
			HorizontalDirection = HorizontalMovementDirection::NO_DIRECTION;
	}


	//Vertical-----

	if (VerticalDirection == VerticalMovementDirection::NO_DIRECTION)
	{
		// If in the previous frame it did not move in y, calculate the new address from the central positions.
		if (next_point_pivot_central.y > enemy_central_pos.y)
			VerticalDirection = VerticalMovementDirection::DOWN;

		else if (next_point_pivot_central.y < enemy_central_pos.y)
			VerticalDirection = VerticalMovementDirection::UP;
	}



	if (VerticalDirection == VerticalMovementDirection::UP)
	{
		// Up
		if (next_point_pivot_down_right.y < enemy_down_right_pos.y + 10)
			pos.y -= speed;

		else
			VerticalDirection = VerticalMovementDirection::NO_DIRECTION;
	}
	else if (VerticalDirection == VerticalMovementDirection::DOWN)
	{
		// Down
		if (next_point_world.y > pos.y)
			pos.y += speed;

		// if 
		else
			VerticalDirection = VerticalMovementDirection::NO_DIRECTION;
	}
}
