#ifndef __MODULEENTITYMANAGER_H__
#define __MODULEENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"

class Entity;
class j1Player;

class ModuleEntityManager : public j1Module
{
public:
	ModuleEntityManager();

	//bool Start() { return true; };
	bool PreUpdate() { return true; };
	bool Update(float dt);
	bool PostUpdate() { return true; };
	bool CleanUp() { return true; };

	//bool Save(pugi::xml_node&) const;
	//bool Load(pugi::xml_node&);

	Entity* CreateEntity(Entity::EntityType type);

	j1Player* getPlayer();

private:
	p2List<Entity*> entities;
};

#endif //__MODULEENTITYMANAGER_H__
