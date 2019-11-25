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
	bool ret = true;
	for (p2List_item<Entity*>* iter = entities.start; iter && ret; iter = iter->next)
	{
		if (iter->data != nullptr)
			ret = iter->data->Update(dt);

	}

	return true;
}

bool ModuleEntityManager::PostUpdate()
{
	bool ret = true;

	for (p2List_item<Entity*>* iter = entities.start; iter && ret; iter = iter->next)
	{
		ret = iter->data->PostUpdate();
		iter->data->Draw();
	}
	return ret;
}

bool ModuleEntityManager::CleanUp()
{
	for (p2List_item<Entity*>* iter = entities.start; iter; iter = iter->next)
	{
		iter->data->CleanUp();
		RELEASE(iter->data);
	}
	entities.clear();
	return true;
}

Entity* ModuleEntityManager::CreateEntity(EntityType type, SDL_Rect& rect)
{
	//STATIC ASSERT
	//It will cause a compilation failure and produce an error message 
	//that in most IDE's be caught and displayed in the IDE's error window.

	//static_assert(EntityType::NO_TYPE == 4, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:
		ret = new j1Player(rect);
		break;
	case EntityType::ENEMY:
		break;
	case EntityType::NO_TYPE:
		break;
	}

	ret->Awake(App->GetConfig().child("entity"));
	ret->Start();


	if (ret != nullptr)
	{
		entities.add(ret);
	}

	return ret;
}

j1Player* ModuleEntityManager::getPlayer()
{
	Entity* ret = nullptr;

	for (p2List_item<Entity*>* iter = entities.start ; iter; iter = iter->next )
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