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

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	Entity* CreateEntity(Entity::EntityType type);

private:
	std::vector<Entity*> entities;
};

#endif //__MODULEENTITYMANAGER_H__
