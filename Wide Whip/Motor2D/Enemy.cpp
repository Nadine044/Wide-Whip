#include "j1App.h"
#include "Enemy.h"
#include "Entity.h"
#include "j1Textures.h"
#include "ModuleCollision.h"
#include "ModuleEntityManager.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "p2DynArray.h"
#include "SDL/include/SDL_timer.h"

Enemy::Enemy(EntityType type, SDL_Rect& rect) : Entity(type, rect)
{}

//Destructor
Enemy::~Enemy()
{}

bool Enemy::Awake(const pugi::xml_node& config)
{
	LOG("Loading Enemies Parser");
	bool ret = true;

	pugi::xml_node enemy_node = config.child("enemy");

	speed = enemy_node.child("speed").attribute("value").as_int();
	gravity = enemy_node.child("gravity").attribute("value").as_float();
	text_path = enemy_node.child_value("texture");

	pugi::xml_node animations_node = enemy_node.child("animations");

	idle.LoadAnimation(animations_node.child("idle"));

	return ret;
}

bool Enemy::Start()
{
	LOG("Loading Enemy textures");
	bool ret = true;

	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	current_animation = &idle;
	state = ENEMY_STATE::IDLE;

	speed = 2;
	velocity = 0;

	return ret;
}

bool Enemy::PreUpdate()
{
	iPoint player_pos = App->module_entity_manager->getPlayer()->pos;
	switch (state)
	{
	case ENEMY_STATE::UNKNOWN:
		break;
	case ENEMY_STATE::IDLE:
		
		if (pos.DistanceTo(player_pos) <= range_detect && pos.DistanceTo(player_pos) > minim_range_detect)
		{
			time_to_pathfind_start = SDL_GetTicks();
			state = ENEMY_STATE::PATHFINDING;
			App->pathfinding_module->CreatePath(App->map->WorldToMap(pos + pivot_down_central), App->map->WorldToMap(player_pos + App->module_entity_manager->getPlayer()->pivot_down_central));			
		}			

		break;
	case ENEMY_STATE::PATHFINDING:
		break;
	case ENEMY_STATE::DEAD:
		break;
	default:
		break;
	}
	return true;
}

bool Enemy::Update(float dt)
{
	iPoint p;
	p.x = col->rect.w * 0.5f;
	p.y = col->rect.h * 0.5f;
	switch (state)
	{
	case ENEMY_STATE::UNKNOWN:
		break;
	case ENEMY_STATE::IDLE:
		velocity = 0;
		break;
	case ENEMY_STATE::PATHFINDING:
		if (pos.DistanceTo(App->module_entity_manager->getPlayer()->pos) <= minim_range_detect)
			GoToPlayer();
		else
		{
			if (time_to_pathfind < SDL_GetTicks() - time_to_pathfind_start)
			{
				time_to_pathfind_start = SDL_GetTicks();
				if (velocity_y == 0)
					App->pathfinding_module->CreatePath(App->map->WorldToMap(pos + p), App->map->WorldToMap(App->module_entity_manager->getPlayer()->pos + App->module_entity_manager->getPlayer()->pivot_down_central));
				else if (velocity_y < 0)
					App->pathfinding_module->CreatePath(App->map->WorldToMap(pos + pivot_down_central), App->map->WorldToMap(App->module_entity_manager->getPlayer()->pos + App->module_entity_manager->getPlayer()->pivot_down_central));
				else if (velocity_y > 0)
					App->pathfinding_module->CreatePath(App->map->WorldToMap(pos + pivot_up_central), App->map->WorldToMap(App->module_entity_manager->getPlayer()->pos + App->module_entity_manager->getPlayer()->pivot_down_central));
			}
			GoToNextPoint();
		}
		break;
	case ENEMY_STATE::DEAD:
		break;
	default:
		break;
	}
	return true;
}

bool Enemy::PostUpdate()
{
	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

void Enemy::OnTrigger(Collider* col2)
{
	if (col2->tag == TAG::WALL)
	{
		if(col->last_colision_direction == DISTANCE_DIR::UP || col->last_colision_direction == DISTANCE_DIR::DOWN)
			velocity_y = 0;
	}
}

bool Enemy::Save(pugi::xml_node& save_file) const
{
	pugi::xml_node pos_node = save_file.append_child("position");
	pos_node.append_attribute("x") = pos.x;
	pos_node.append_attribute("y") = pos.y;

	save_file.append_child("entity_type").append_attribute("value") = (int)type;

	//TODO: save_file.append_child("velocity").append_attribute("value") = GetVelocity();
	save_file.append_child("state").append_attribute("value") = (int)state;

	save_file.append_child("flip").append_attribute("value") = flip;

	save_file.append_child("collider").append_attribute("enabled") = col->IsEnabled();

	return true;
}

bool Enemy::Load(pugi::xml_node& save_file)
{
	pos.x = save_file.child("position").attribute("x").as_int();
	pos.y = save_file.child("position").attribute("y").as_int();

	type = EntityType(save_file.child("entity_type").attribute("value").as_int());

	state = ENEMY_STATE(save_file.child("state").attribute("value").as_int());

	flip = (SDL_RendererFlip)save_file.child("flip").attribute("value").as_int();

	save_file.child("collider").attribute("enabled").as_bool() ? col->Enable() : col->Disable();

	return true;
}

void Enemy::GoToPlayer()
{
	if (App->module_entity_manager->getPlayer()->pos.x < pos.x)
	{
		pos.x -= speed;
	}
	else if (App->module_entity_manager->getPlayer()->pos.x > pos.x)
	{
		pos.x += speed;
	}
	if (App->module_entity_manager->getPlayer()->pos.y < pos.y)
	{
		pos.y -= speed;
	}
	else if (App->module_entity_manager->getPlayer()->pos.y > pos.y)
	{
		pos.y += speed;
	}
}