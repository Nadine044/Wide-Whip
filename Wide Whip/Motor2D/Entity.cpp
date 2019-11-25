#include "Entity.h"
#include "ModuleEntityManager.h"

Entity::Entity(EntityType type, iPoint pos) : type(type), pos(pos)
{}

Entity::~Entity()
{}