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
	case EntityType::ENEMY:
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