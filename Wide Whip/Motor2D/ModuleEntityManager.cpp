#include "ModuleEntityManager.h"

ModuleEntityManager::ModuleEntityManager()
{
	name.create("enitity_manager");
}

bool ModuleEntityManager::Update(float dt)
{
	for (p2List_item<Entity*>* iter = entities.start; iter; iter = iter->next)
	{
		iter->data->Update(dt);
	}
}