#include "Entity.h"
#include "ModuleEntityManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "ModuleCollision.h"
#include "p2Log.h"

Entity::Entity(EntityType type, SDL_Rect& rect) : type(type)
{
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
	App->render->Blit(text, pos.x + offset_animation_x, pos.y, &(current_animation->GetCurrentFrame()), 1.0f, flip);
}

float Entity::GetVelocity()
{
	return velocity;
}

bool Entity::Save(pugi::xml_node& node) const
{
	return true;
}

bool Entity::Load(pugi::xml_node& node)
{
	return true;
}