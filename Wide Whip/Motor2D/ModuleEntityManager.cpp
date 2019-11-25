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
	for (p2List_item<Entity*>* iter = entities.start; iter; iter = iter->next)
	{
		iter->data->Update(dt);
	}

	return true;
}

Entity* ModuleEntityManager::CreateEntity(EntityType type)
{
	//STATIC ASSERT
	//It will cause a compilation failure and produce an error message 
	//that in most IDE's be caught and displayed in the IDE's error window.

	//static_assert(EntityType::NO_TYPE == 4, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:
		ret = new j1Player();
		break;
	case EntityType::ENEMY:
		break;
	case EntityType::COIN:
		break;
	case EntityType::NO_TYPE:
		break;
	}

	if (ret != nullptr)
	{
		entities.add(ret);
	}

	return ret;
}

j1Player* ModuleEntityManager::getPlayer()
{
	Entity* ret = nullptr;

	for (p2List_item<Entity*>* iter = entities.start ; iter < entities.end ; iter = iter->next )
	{
		if (iter->data->type == EntityType::PLAYER)
		{
			ret = iter->data;
			return (j1Player*)ret;
		}
	}
	LOG("Player not found!");
	return (j1Player*)ret;
}