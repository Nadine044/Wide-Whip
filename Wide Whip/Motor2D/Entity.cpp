#include "Entity.h"
#include "ModuleEntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "ModuleCollision.h"
#include "p2Log.h"
#include "Brofiler/Brofiler.h"

Entity::Entity(EntityType type, SDL_Rect& rect) : type(type)
{
	BROFILER_CATEGORY("EntityConstructor", Profiler::Color::Blue);

	pos.x = rect.x;
	pos.y = rect.y;

	pivot_down_central.x = rect.w * 0.5f;
	pivot_down_central.y = rect.h - 10;

	pivot_up_central.x = rect.w * 0.5f;
	pivot_up_central.y = 0;

	TAG tag;
	Color color;
	switch (type)
	{
	case EntityType::NO_TYPE:
		break;
	case EntityType::PLAYER:
		color = Green;
		tag = TAG::PLAYER;
		break;
	case EntityType::FLYENEMY:
		color = Red;
		tag = TAG::ENEMY;
		break;
	case EntityType::WALKENEMY:
		color = Red;
		tag = TAG::ENEMY;
		break;
	case EntityType::COIN:
		color = Blue;
		tag = TAG::COIN;
		break;
	default:
		LOG("EntityType not valid");
		break;
	}

	col = App->collisions->AddCollider(pos, rect.w, rect.h, tag, color, this, true);

	
}

Entity::~Entity()
{}

void Entity::Draw() const
{
	BROFILER_CATEGORY("EntityDraw", Profiler::Color::Blue);

	App->render->Blit(text, pos.x + offset_animation_x, pos.y, &(current_animation->GetCurrentFrame()), 1.0f, flip);
}

const float Entity::GetVelocity() const
{
	return velocity;
}

bool Entity::Save(pugi::xml_node& node) const
{
	BROFILER_CATEGORY("EntitySave", Profiler::Color::Blue);

	return true;
}

bool Entity::Load(pugi::xml_node& node)
{
	BROFILER_CATEGORY("EntityLoad", Profiler::Color::Blue);

	return true;
}