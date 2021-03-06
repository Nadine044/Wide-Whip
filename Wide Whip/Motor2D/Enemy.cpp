#include "j1App.h"
#include "Enemy.h"
#include "Entity.h"
#include "j1Textures.h"
#include "ModuleCollision.h"
#include "Brofiler/Brofiler.h"
#include "ModuleEntityManager.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "SDL/include/SDL_timer.h"
#include "j1Input.h"
#include "j1Render.h"

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
	//text_path = enemy_node.child_value("texture");
	dt_multiplied = config.previous_sibling("app").child("dt_multiplied").attribute("value").as_int();

	pugi::xml_node animations_node = enemy_node.child("animations");

	//idle.LoadAnimation(animations_node.child("idle"));
	//walk.LoadAnimation(animations_node.child("walk"));

	return ret;
}

bool Enemy::Start()
{
	LOG("Loading Enemy textures");
	bool ret = true;

	pos.x = col->rect.x;
	pos.y = col->rect.y;

	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	current_animation = &idle;
	state = ENEMY_STATE::IDLE;

	//speed = 1;
	//velocity = 0;

	debug_tex = App->tex->Load("maps/path2.png");

	return ret;
}

bool Enemy::PreUpdate()
{
	BROFILER_CATEGORY("EnemyPreUpdate", Profiler::Color::Red);

	iPoint player_pos = App->module_entity_manager->GetPlayer()->pos;
	switch (state)
	{
	case ENEMY_STATE::UNKNOWN:
		break;
	case ENEMY_STATE::IDLE:
		
		if (pos.DistanceTo(player_pos) <= range_detect && pos.DistanceTo(player_pos) > minim_range_detect)
		{
			time_to_pathfind_start = SDL_GetTicks();
			state = ENEMY_STATE::PATHFINDING;
			App->pathfinding_module->CreatePath(path, App->map->WorldToMap(pos + pivot_down_central), App->map->WorldToMap(player_pos + App->module_entity_manager->GetPlayer()->pivot_down_central));			
		}			

		break;
	case ENEMY_STATE::PATHFINDING:
		if (pos.DistanceTo(player_pos) > range_detect)
		{
			state = ENEMY_STATE::IDLE;
		}
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
	BROFILER_CATEGORY("EnemyUpdate", Profiler::Color::Red);

	iPoint p;
	p.x = col->rect.w * 0.5f;
	p.y = col->rect.h * 0.5f;

	switch (state)
	{
	case ENEMY_STATE::UNKNOWN:
		break;
	case ENEMY_STATE::IDLE:
		break;
	case ENEMY_STATE::PATHFINDING:
		if (pos.DistanceTo(App->module_entity_manager->GetPlayer()->pos) <= minim_range_detect)
			GoToPlayer(dt);
		else
		{
			if (time_to_pathfind < SDL_GetTicks() - time_to_pathfind_start)
			{
				time_to_pathfind_start = SDL_GetTicks();
				App->pathfinding_module->CreatePath(path, App->map->WorldToMap(pos + p), App->map->WorldToMap(App->module_entity_manager->GetPlayer()->pos + App->module_entity_manager->GetPlayer()->pivot_down_central));
			}
			GoToNextPoint(dt);
		}
		break;
	case ENEMY_STATE::DEAD:
		break;
	default:
		break;
	}
	in_collision = false; //reset every frame. (dirty but works)

	return true;
}

bool Enemy::PostUpdate()
{
	BROFILER_CATEGORY("EnemyPostUpdate", Profiler::Color::Red);

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		draw_debug = !draw_debug;

	if (draw_debug)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p);
		p = App->map->MapToWorld(p);

		SDL_Rect rect = { 0, 0, 32, 32 };
		App->render->Blit(debug_tex, p.x, p.y, &rect);

		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(*path.At(i));
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}
	return true;
}

bool Enemy::CleanUp()
{
	BROFILER_CATEGORY("EnemyCleanUp", Profiler::Color::Red);

	LOG("Enemy unloaded");
	App->tex->UnLoad(text);

	return true;
}

void Enemy::OnTrigger(Collider* col2)
{
	if (col2->tag == TAG::WALL)
	{
		in_collision = true;
		if (col->last_colision_direction == DISTANCE_DIR::UP || col->last_colision_direction == DISTANCE_DIR::DOWN)
			VerticalDirection = VerticalMovementDirection::NO_DIRECTION; //reset Direction in vertical movement to avoid tremors.
	}
	if (col2->tag == TAG::PLAYER)
	{
		LOG("Enemy attacking Player");
	}
}

bool Enemy::Save(pugi::xml_node& save_file) const
{
	BROFILER_CATEGORY("EnemySave", Profiler::Color::Red);

	pugi::xml_node pos_node = save_file.append_child("position");
	pos_node.append_attribute("x") = pos.x;
	pos_node.append_attribute("y") = pos.y;

	save_file.append_child("entity_type").append_attribute("value") = (int)type;

	save_file.append_child("state").append_attribute("value") = (int)state;

	save_file.append_child("flip").append_attribute("value") = flip;

	save_file.append_child("collider").append_attribute("enabled") = col->IsEnabled();

	return true;
}

bool Enemy::Load(pugi::xml_node& save_file)
{
	BROFILER_CATEGORY("EnemyLoad", Profiler::Color::Red);

	pos.x = save_file.child("position").attribute("x").as_int();
	pos.y = save_file.child("position").attribute("y").as_int();

	type = EntityType(save_file.child("entity_type").attribute("value").as_int());

	state = ENEMY_STATE(save_file.child("state").attribute("value").as_int());

	flip = (SDL_RendererFlip)save_file.child("flip").attribute("value").as_int();

	save_file.child("collider").attribute("enabled").as_bool() ? col->Enable() : col->Disable();

	return true;
}

void Enemy::GoToPlayer(float dt)
{
	int final_speed = speed * dt;
	if (App->module_entity_manager->GetPlayer()->pos.x < pos.x)
	{
		pos.x -= final_speed;
	}
	else if (App->module_entity_manager->GetPlayer()->pos.x > pos.x)
	{
		pos.x += final_speed;
	}
	if (App->module_entity_manager->GetPlayer()->pos.y < pos.y)
	{
		pos.y -= final_speed;
	}
	else if (App->module_entity_manager->GetPlayer()->pos.y > pos.y)
	{
		pos.y += final_speed;
	}
}