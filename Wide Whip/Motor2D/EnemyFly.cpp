#include "EnemyFly.h"
#include "j1App.h"
#include "j1Pathfinding.h"

EnemyFly::EnemyFly(SDL_Rect& rect) : Enemy(EntityType::FLYENEMY, rect) {};

void EnemyFly::GoToPlayer()
{
	const p2DynArray<iPoint>* path = App->pathfinding_module->GetLastPath();

	LOG("GOTOPLAYER");
}