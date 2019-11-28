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


			if (in_collision)
			{
				// All coordinates in world (pixels)
				//horizontal----
				if (HorizontalDirection == HorizontalMovementDirection::NO_DIRECTION)
				{
					// If in the previous frame it did not move in x, calculate the new address from the central positions.
					if (next_point_pivot_central.x > enemy_central_pos.x)
						HorizontalDirection = HorizontalMovementDirection::RIGHT;

					else if(next_point_pivot_central.x < enemy_central_pos.x)
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
			else
			{
				// Normal movement (tiles)
				// All coordinates in map.

				iPoint pos_central_map = App->map->WorldToMap(enemy_central_pos);

				// Horizontal
				if (pos_central_map.x < next_point->x)			// Right
					pos.x += speed;
				else if (pos_central_map.x > next_point->x)		// Left
					pos.x -= speed;

				// Vertical
				if (pos_central_map.y < next_point->y)			// Down
					pos.y += speed;
				else if (pos_central_map.y > next_point->y)		// Up
					pos.y -= speed;
			}
		}
	}
}