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
		const iPoint* next_point = path->At(path->Count()-3);

		//if (next_point != nullptr)
		//{
		//	iPoint next_point_pivot_down_central = App->map->MapToWorld(*next_point);
		//	next_point_pivot_down_central.x += App->map->data.tile_width * 0.5f;
		//	next_point_pivot_down_central.y += App->map->data.tile_height;

		//	iPoint enemy_relative_pos = pos + pivot_down_central;
		//	iPoint pivot_player = App->module_entity_manager->getPlayer()->pivot_down_central;
		//	if (next_point_pivot_down_central.x + speed < enemy_relative_pos.x - pivot_player.x)
		//		pos.x -= speed;  //left
		//	else if (next_point_pivot_down_central.x - speed > enemy_relative_pos.x - pivot_player.x)
		//		pos.x += speed;  //right

		//	if (next_point_pivot_down_central.y + speed < enemy_relative_pos.y - pivot_player.y)
		//		pos.y -= speed;  //up

		//	else if (next_point_pivot_down_central.y - speed > enemy_relative_pos.y - pivot_player.y)
		//		pos.y += speed;  //down
		//}
		if (next_point != nullptr)
		{
			iPoint next_point_pivot_down_central = App->map->MapToWorld(*next_point);
			next_point_pivot_down_central.x += App->map->data.tile_width * 0.5f;
			next_point_pivot_down_central.y += App->map->data.tile_height - speed;

			iPoint enemy_relative_pos = pos + pivot_down_central;
			iPoint pivot_player = App->module_entity_manager->getPlayer()->pivot_down_central;

			if (in_collision)
			{
				//horizontal----
				if (velocity_x == 0)
				{
					if (next_point_pivot_down_central.x > enemy_relative_pos.x)
						velocity_x = speed;

					else if(next_point_pivot_down_central.x < enemy_relative_pos.x)
						velocity_x = -speed;
				}


				if (velocity_x < 0)
				{
					if (next_point_pivot_down_central.x + App->map->data.tile_width * 0.5f < enemy_relative_pos.x + col->rect.w*0.5f)
					{
						velocity_x = -speed;//left
						pos.x += velocity_x;

					}
					else
						velocity_x = 0;
				}
				else if (velocity_x > 0)
				{
					if (next_point_pivot_down_central.x - App->map->data.tile_width * 0.5f > enemy_relative_pos.x - col->rect.w*0.5f)
					{
						velocity_x = speed; //right
						pos.x += velocity_x;
					}
					else
						velocity_x = 0;
				}


				//Vertical-----

				if (velocity_y == 0)
				{
					if (next_point_pivot_down_central.y - App->map->data.tile_height * 0.5f > pos.y + pivot_down_central.y * 0.5f)
						velocity_y = speed;

					else if (next_point_pivot_down_central.y - App->map->data.tile_height * 0.5f < pos.y + pivot_down_central.y * 0.5f)
						velocity_y = -speed;
				}



					if (velocity_y < 0)
					{
						if (next_point_pivot_down_central.y < enemy_relative_pos.y + 10)
						{
							velocity_y = -speed; //up
							pos.y += velocity_y;
						}
						else
							velocity_y = 0;
					}
					else if (velocity_y > 0)
					{
						if (next_point_pivot_down_central.y - App->map->data.tile_height > enemy_relative_pos.y - col->rect.h)
						{
							velocity_y = speed; //down
							pos.y += velocity_y;
						}
						else
							velocity_y = 0;
					}
			}
			else
			{
				//normal
				iPoint enemy_central_pos = pos;
				enemy_central_pos.x += col->rect.w *0.5f;
				enemy_central_pos.y += col->rect.h *0.5f;
				iPoint pos_central_map = App->map->WorldToMap(enemy_central_pos);
				if (pos_central_map.x < next_point->x)
					pos.x += speed;
				else if (pos_central_map.x > next_point->x)
					pos.x -= speed;

				if (pos_central_map.y < next_point->y)
					pos.y += speed;
				else if (pos_central_map.y > next_point->y)
					pos.y -= speed;
			}
		}
	}
}