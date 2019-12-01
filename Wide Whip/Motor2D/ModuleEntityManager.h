#ifndef __MODULEENTITYMANAGER_H__
#define __MODULEENTITYMANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "Entity.h"
#include "j1Player.h"
#include "PugiXml\src\pugixml.hpp"

class j1Player;

class ModuleEntityManager : public j1Module
{
public:
	ModuleEntityManager();

	//bool Start() { return true; };
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp();

	//bool LoadAllEntities();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	Entity* CreateEntity(EntityType type, SDL_Rect& rect);
	void DeleteEntity(Entity* entity_to_delete);

	j1Player* getPlayer();

private:
	p2List<Entity*> entities;

	mutable p2SString	save_game_root;
};

#endif //__MODULEENTITYMANAGER_H__
