#include "j1App.h"
#include "Coin.h"
#include "Entity.h"
#include "ModuleEntityManager.h"
#include "p2Log.h"
#include "ModuleCollision.h"

Coin::Coin(SDL_Rect& rect) : Entity(EntityType::COIN, rect)
{}

Coin::~Coin()
{}

bool Coin::Awake(const pugi::xml_node& config)
{
	LOG("Loading Coin Parser");
	bool ret = true;

	pugi::xml_node coin_node = config.child("coin");
	pugi::xml_node animations_node = coin_node.child("animations");
	pugi::xml_node audio_node = coin_node.child("audios");

	text_path = coin_node.child_value("texture");
	
	rotating.LoadAnimation(animations_node.child("rotating"));

	return ret;
}

bool Coin::Start()
{
	LOG("Loading Coin Textures");
	bool ret = true;

	pos.x = col->rect.x;
	pos.y = col->rect.y;

	velocity = 0.f;

	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	current_animation = &rotating;
	state = COIN_STATE::IN_GAME;

	return ret;
}

bool Coin::Update(float dt)
{
	switch (state)
	{
	case COIN_STATE::UNKNOWN:
		break;
	case COIN_STATE::IN_GAME:
		current_animation = &rotating;
		break;
	case COIN_STATE::PICKED:
		LOG("YEEEY MAN COIN PICKEEED!!!!");
		break;
	default:
		break;
	}

	return true;
}

bool Coin::CleanUp()
{
	return true;
}

bool Coin::Save(pugi::xml_node& save) const
{
	return true;
}

bool Coin::Load(pugi::xml_node& load)
{
	return true;
}

