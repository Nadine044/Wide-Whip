#ifndef __Module_Collision_H__
#define __Module_Collision_H__

#include "j1Module.h"

#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "Colors.h"

class Entity;

class Player;
enum class TAG : uint
{
	NONE = 0,
	WALL,
	PLATFORM,
	WATER,
	PLAYER,
	CHANGE_LEVEL,
	ENEMY,
	COIN,
	MAX

};


enum class DISTANCE_DIR : int
{
	NONE = -1,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

class Collider
{

public:
	SDL_Rect		rect;
	TAG				tag;
	bool			dynamic = false;
	Entity*			object; //will be replace by object when we have entity manager.
	bool			first_time_collision = false;
	

	DISTANCE_DIR	last_colision_direction = DISTANCE_DIR::NONE;

private:

	bool			to_delete = false;
	bool			enable = false;

	Color			color;

public:
	Collider(iPoint, int, int, TAG, Color, Entity*, bool);

	bool CheckColision(const Collider*) const;

	bool CheckColision(const iPoint&) const;

	bool CheckColisionBottom(const Collider*) const;

	void UpdatePos(const iPoint pos);

	const bool IsToDelete() const;

	void Remove();

	Color GetColor() const;

	bool IsEnabled() const;

	void Disable();

	void Enable();

};



class ModuleCollision : public j1Module
{
public:



	ModuleCollision();

	bool Awake(pugi::xml_node&) override;

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	Collider* AddCollider(iPoint pos, int width, int height, TAG tag, Color color, Entity* parent = nullptr, bool dymanic = false);

	DISTANCE_DIR OverlapDS(Collider* c_dynamic, Collider* c_static);
	DISTANCE_DIR OverlapPlatform(Collider* c_dynamic, Collider* c_static);

	Uint8 GetAlphaDebug() const;

	const bool IsInSomeCollider(const iPoint& pos_map) const;

public:

	Collider* player = nullptr; //temporally to move a collider;
	Collider* enemyWalkable = nullptr;

private:

	p2List<Collider*>	colliders_static_list;
	p2List<Collider*>	colliders_dynamic_list;

	bool				trigger_matrix[(int)TAG::MAX][(int)TAG::MAX];
	bool				physics_matrix[(int)TAG::MAX][(int)TAG::MAX];

	bool				debug			= false;
	Uint8				alpha_debug		= 0u;

private:

	void DeleteCollidersToRemove();// delete colliders to delete of the list. only for the preupdate and cleanUp.

	void SetAllCollidersToDelete();

};
#endif // __Module_Collision_H__


