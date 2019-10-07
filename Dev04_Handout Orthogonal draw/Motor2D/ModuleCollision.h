#ifndef __Module_Collision_H__
#define __Module_Collision_H__

#include "j1Module.h"

#include <list>
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

enum class TAG : int
{
	NONE = 0,
	WALL,
	PLAYER

};


class Collider
{
private:
	SDL_Rect	rect;
public:
	Collider() {};
};


class ModuleCollision : public j1Module
{
public:

	ModuleCollision() {};

	bool Update(float dt) override { return true; }

	bool PostUpdate() override { return true; }

	bool CleanUp() override { return true; }

	Collider* AddCollider(fPoint pos, float width, float height, TAG tag) { Collider* ret = new Collider(); return ret; };

private:

	std::list<Collider*> colliders_list;

	bool debug = false;

};
#endif // __Module_Collision_H__


