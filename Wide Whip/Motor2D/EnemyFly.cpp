#include "EnemyFly.h"
#include "j1App.h"
#include "ModuleEntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

EnemyFly::EnemyFly(SDL_Rect& rect) : Enemy(EntityType::FLYENEMY, rect) {};

void EnemyFly::GoToPlayer()
{

	const p2DynArray<iPoint>* path = App->pathfinding_module->GetLastPath();
	if (path != nullptr)
	{
		const iPoint* next_point = path->At(path->Count()-2);

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


			if (next_point_pivot_down_central.x  < enemy_relative_pos.x )
				pos.x -= speed;  //left
			else if (next_point_pivot_down_central.x  > enemy_relative_pos.x)
				pos.x += speed;  //right

			if (next_point_pivot_down_central.y  < enemy_relative_pos.y )
				pos.y -= speed;  //up

			else if (next_point_pivot_down_central.y  > enemy_relative_pos.y)
				pos.y += speed;  //down
		}
	}
}