#ifndef __Module_Collision_H__
#define __Module_Collision_H__

#include "j1Module.h"

#include "p2List.h"
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
public:
	SDL_Rect	rect;
	TAG			tag;
public:
	Collider(fPoint, float, float, TAG);

	bool CheckColision(const Collider*) const;
};


class ModuleCollision : public j1Module
{
public:

	ModuleCollision() {};

	bool Update(float dt) override;

	//bool PostUpdate() override;

	bool CleanUp() override { return true; }

	Collider* AddCollider(fPoint pos, float width, float height, TAG tag);

	Collider* player = nullptr;

private:

	p2List<Collider*> colliders_list;

	bool debug = true;

};
#endif // __Module_Collision_H__


