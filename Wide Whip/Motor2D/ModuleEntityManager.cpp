#include "j1App.h"
#include "p2Log.h"
#include "ModuleEntityManager.h"
#include "ModuleCollision.h"
#include "Entity.h"
#include "j1Player.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"
#include "Coin.h"

ModuleEntityManager::ModuleEntityManager() : j1Module()
{
	name.create("enitity_manager");
}


bool ModuleEntityManager::PreUpdate()
{
	bool ret = true;
	if (!App->IsPaused())
	{
		for (p2List_item<Entity*>* iter = entities.start; iter && ret; iter = iter->next)
		{
			ret = iter->data->PreUpdate();
		}
	}
	return ret;
}

bool ModuleEntityManager::Update(float dt)
{
	bool ret = true;
	if (!App->IsPaused())
	{
		for (p2List_item<Entity*>* iter = entities.start; iter && ret; iter = iter->next)
		{
			if (iter->data != nullptr)
			{
				ret = iter->data->Update(dt);
				if (ret)
					iter->data->col->UpdatePos(iter->data->pos);
			}

		}
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
		ret = new Player(rect);
		break;
	case EntityType::FLYENEMY:
		ret = new EnemyFly(rect);
		break;
	case EntityType::WALKENEMY:
		ret = new EnemyWalk(rect);
		break;
	case EntityType::COIN:
		ret = new Coin(rect);
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

void ModuleEntityManager::DeleteEntity(Entity* entity_to_delete)
{
	p2List_item<Entity*>* item = entities.start;

	for (item = entities.start; item; item = item->next)
	{
		if (item->data == entity_to_delete)
		{
			entity_to_delete->col->Remove();
			entities.del(item);
			item->data->CleanUp();
			RELEASE(item->data);
			return;
		}
	}
}

Player* ModuleEntityManager::GetPlayer() const
{
	Entity* ret = nullptr;

	for (p2List_item<Entity*>* iter = entities.start ; iter; iter = iter->next )
	{
		if (iter->data->type == EntityType::PLAYER)
		{
			ret = iter->data;
			return (Player*)ret;
		}
	}
	LOG("Player not found!");
	return (Player*)ret;
}

bool ModuleEntityManager::Save(pugi::xml_node& save_file) const
{
	bool ret = true;

	LOG("Saving all entities");

	p2List_item<Entity*>* item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(save_file.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret == true)
	{
		//TODO : //data.save_file(save_game_root.GetString());
		LOG("... finished saving");
	}
	else
		LOG("Save process halted from an error in entity %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	return ret;
}

bool ModuleEntityManager::Load(pugi::xml_node& load_file)
{
	bool ret = false;

	LOG("Loading all entities");

	p2List_item<Entity*>* item = entities.start;

	pugi::xml_node entity_node = load_file.child("entity");

	while (entity_node != NULL)
	{
		ret = item->data->Load(entity_node);
		item = item->next;

		entity_node = entity_node.next_sibling("entity");
	}

	if (ret == true)
		LOG("... finished loading");
	else
		LOG("Load process halted from an error in entity %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	
	return ret;
}