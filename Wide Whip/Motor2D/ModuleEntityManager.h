#ifndef __MODULEENTITYMANAGER_H__
#define __MODULEENTITYMANAGER_H__

#include "j1Module.h"
#include "Entity.h"
#include "p2List.h"

class Entity;

class ModuleEntityManager : public j1Module
{
public:
	ModuleEntityManager();

	bool Update(float dt) override;
	bool PostUpdate() override { return true; };
	bool CleanUp() override { return true; };

	Entity* CreateEntity(Entity::EntityType type);

private:
	std::vector<Entity*> entities;
};

#endif //__MODULEENTITYMANAGER_H__
