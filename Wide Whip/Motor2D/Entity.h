#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "ModuleCollision.h"
#include "Entity.h"
#include <vector>

class Entity
{
public:
	enum class EntityType
	{
		PLAYER,
		ENEMY,
		COIN,

		NO_TYPE
	};

	Entity(EntityType type);
	~Entity();

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual bool Save(pugi::xml_node&) const override;
	virtual bool Load(pugi::xml_node&) override;

	//virtual void Draw(float dt);
	//On Collision??

public:
	EntityType type = EntityType::NO_TYPE;
	iPoint position = { 0, 0 };
	Animation* animation = nullptr;
	Collider* collider = nullptr;
};

#endif // !__ENTITY_H__

