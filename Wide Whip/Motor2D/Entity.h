#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "ModuleCollision.h"

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

	Entity(const int& x, const int& y);
	~Entity();

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	//virtual void Draw(float dt);
	//On Collision??

public:
	EntityType type = EntityType::NO_TYPE;
	iPoint position = { 0, 0 };
	Animation* animation = nullptr;
	Collider* collider = nullptr;
};

#endif // !__ENTITY_H__

