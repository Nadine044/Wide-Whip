#include "j1App.h"
#include "Enemy.h"
#include "Entity.h"
#include "j1Textures.h"

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

	//TODO: Load all config.xml enemy vars here (the same as player data)

	return ret;
}

bool Enemy::Start()
{
	LOG("Loading Enemy textures");
	bool ret = true;

	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	current_animation = &idle;

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
