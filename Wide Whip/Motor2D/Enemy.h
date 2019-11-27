#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "PugiXml/src/pugixml.hpp"
#include "p2Log.h"
#include "Entity.h"
#include "Animation.h"
#include "p2SString.h"

class Entity;
class Animation;

enum class EnemyType
{
	NO_TYPE = -1,

	WALKER,
	FLYER,

	MAX
};

enum class ENEMY_STATE
{
	UNKNOWN = -1,
	IDLE,
	PATHFINDING,
	DEAD
};

class Enemy : public Entity
{
public:
	
	Enemy(EntityType type, SDL_Rect& rect);

	//Destructor
	~Enemy();

	//Called before render is available
	bool Awake(const pugi::xml_node& enemy) override;

	//Load
	bool Start();

	bool PreUpdate();
	bool Update(float dt) override;
	bool PostUpdate() override;

	//Called before quitting
	bool CleanUp() override;

	//Save&Load

	bool Save(pugi::xml_node&) const override;
	bool Load(pugi::xml_node&) override;

	//Enemy functions:

public:

	//Animations

	Animation	idle;

	//FX

protected:

	p2SString			text_path;

	ENEMY_STATE			state;

	int					speed = 0;
	float				gravity = 0.f;

	uint				range_detect = 200u; //distance in pixels.
	uint				minim_range_detect = 50u; //distance in pixels.

protected:

	virtual void GoToPlayer() {};

};



#endif // !_ENEMY_H_
