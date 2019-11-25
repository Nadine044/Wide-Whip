#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "p2Point.h"

class Collider;
class Animation;

enum class EntityType
{
	PLAYER,
	ENEMY,
	COIN,

	NO_TYPE
};

class Entity
{
public:

	Entity(EntityType type, iPoint pos);
	~Entity();

	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp() { return true; }

	//virtual bool Save(pugi::xml_node&) const;
	//virtual bool Load(pugi::xml_node&);

	//virtual void Draw(float dt);
	//On Collision??

public:
	EntityType type = EntityType::NO_TYPE;
	char* name = nullptr;
	iPoint pos = { 0, 0 };
	Animation* current_animation = nullptr;
	Collider* col = nullptr;
};

#endif // !_ENTITY_H_

