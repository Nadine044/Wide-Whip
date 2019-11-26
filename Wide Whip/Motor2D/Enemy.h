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
	LIVE = 0,
	WALKING,
	//RUNNING,
	ATTACKING,
	PATHFINDING,
	DEAD,
	UNKNOWN
};

class Enemy : public Entity
{
public:
	
	Enemy(SDL_Rect& rect);

	//Destructor
	~Enemy();

	//Called before render is available
	bool Awake(const pugi::xml_node& enemy) override;

	//Load
	bool Start();

	bool Update(float dt) override;
	bool PostUpdate() override;

	//Called before quitting
	bool CleanUp() override;

	//Save&Load

	//bool Save(pugi::xml_node&) const;
	//bool Load(pugi::xml_node&);

	//Enemy functions:

public:

	//Animations

	Animation	idle;

	//FX

private:

	p2SString			text_path;

	ENEMY_STATE			state;

	int					speed = 0;
	float				gravity = 0.f;

	Uint32				start_time = 0u;
	Uint32				time_to_jump = 0u;
	Uint32				time_to_do_fade_to_black = 0u;
	float				time_in_fade = 0.0f;
	
};



#endif // !_ENEMY_H_
