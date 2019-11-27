#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL_render.h"
#include "p2SString.h"

class Collider;
class Animation;
struct SDL_Texture;

enum class EntityType
{
	NO_TYPE = -1,

	PLAYER,
	FLYENEMY,
	WALKENEMY,

	MAX	
};

class Entity
{
public:

	Entity(EntityType type, SDL_Rect& rect);
	~Entity();

	virtual bool Awake(const pugi::xml_node& config) { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp() { return true; }

	virtual void OnTrigger(Collider* col2) {}; // this will be virtual in the class object parent when ObjectManager will be created.

	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);

	

	void Draw() const;

	float GetVelocity();

public:
	EntityType			type = EntityType::NO_TYPE;
	p2SString			name = "entity";
	iPoint				pos = { 0, 0 };
	SDL_Texture*		text = nullptr;
	Animation*			current_animation = nullptr;
	Collider*			col = nullptr;

	int					offset_animation_x = 0;

	SDL_RendererFlip	flip = SDL_FLIP_NONE;


protected:

	float				velocity = 0.0f;

};

#endif // !_ENTITY_H_

