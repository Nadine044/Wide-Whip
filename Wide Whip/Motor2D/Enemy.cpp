#include "j1App.h"
#include "Enemy.h"
#include "Entity.h"
#include "j1Textures.h"
#include "ModuleCollision.h"

Enemy::Enemy(SDL_Rect& rect) : Entity(EntityType::ENEMY, rect)
{}

//Destructor
Enemy::~Enemy()
{}

bool Enemy::Awake(const pugi::xml_node& config)
{
	LOG("Loading Enemies Parser");
	bool ret = true;

	pugi::xml_node enemy_node = config.child("enemy");

	pugi::xml_node time_in_fade_node	= enemy_node.child("time_fade");
	time_to_do_fade_to_black = (Uint32)(time_in_fade_node.attribute("time_to_do_fade_to_black").as_float() * 1000.0f);
	time_to_jump = (Uint32)(time_in_fade_node.attribute("time_to_jump").as_float() * 1000.0f);
	time_in_fade = time_in_fade_node.attribute("time_in_fade").as_float();

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
	start_time = 0u;

	return ret;
}

bool Enemy::Update(float dt)
{
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