#include "j1App.h"
#include "p2Log.h"
#include "ModuleEntityManager.h"
#include "Entity.h"
#include "j1Player.h"

ModuleEntityManager::ModuleEntityManager() : j1Module()
{
	name.create("enitity_manager");
}

bool ModuleEntityManager::Update(float dt)
{
	for (p2List_item<Entity*>* iter = entities.cbegin; iter; iter = iter->next)
	{
		iter->data->Update(dt);
	}

	return true;
}

Entity* ModuleEntityManager::CreateEntity(Entity::EntityType type)
{
	//STATIC ASSERT
	//It will cause a compilation failure and produce an error message 
	//that in most IDE's be caught and displayed in the IDE's error window.

	//static_assert(Entity::EntityType::NO_TYPE == 4, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case Entity::EntityType::PLAYER:
		ret = new j1Player();
		break;

	case Entity::EntityType::ENEMY:
		break;
	case Entity::EntityType::COIN:
		break;
	case Entity::EntityType::NO_TYPE:
		break;
	}

	if (ret != nullptr)
	{
		entities.push_back(ret);
	}

	return ret;
}