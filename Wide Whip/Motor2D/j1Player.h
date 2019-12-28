#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "Entity.h"


#define CAMERA_OFFSET_X 300
#define CAMERA_OFFSET_Y 360
#define MAP_LEFT_OFFSET_X 200

class Entity;
class Collider;
class Animation;

enum class PLAYER_STATE
{
	LIVE = 0,
	DASHING,
	CLIMBING,
	ATTACK,
	DEAD,
	GOD,
	UNKNOWN
};



class Player : public Entity
{
public:

	Player(SDL_Rect& rect);

	//Destructor
	~Player();

	//Called before render is available
	bool Awake(const pugi::xml_node& player) override;

	//Load
	bool Start();
		
	bool Update(float dt) override;
	void JumpHorizontal(float dt);

	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	bool Save(pugi::xml_node&) const override;

	bool Load(pugi::xml_node&) override;

	void OnTrigger(Collider* col2) override;

	float GetVelocity() const;

	void UpdateCameraPos();

	bool* GetReviveBoolAdress() { return &revive; }

private:

	void CheckDebugKeys();

	void Gravity(float dt);

	void ToAction();

	void Movement(float dt);

	void VerticalMovement(float dt);

	void Attack();

	void Revive();

	void Death();

private:

	Animation		jump;
	Animation		idle;

	Animation		walk;
	Animation		death;
	Animation		dash;
	Animation		climb;
	Animation		fall;
	Animation		attack;


	FX jump_fx;
	FX dash_fx;
	FX death_init_fx;
	FX death_finish_fx;

	//SDL_Texture*		text							= nullptr;
	p2String			text_path;

	SDL_Rect			rect_limit_camera;

	int					rect_limit_camera_border_x		= 0;
	int					rect_limit_camera_border_y		= 0;
	int					map_left_offset					= 0;

	uint				jump_force						= 0u;

	int					dash_force						= 0;
	float				velocity_dash					= 0.0f;
	float				resistance_dash					= 0.0f;

	float				jump_clinged_force_left			= 0;
	float				jump_clinged_force_right		= 0;
	float				velocity_jump_clinged			= 0.0f;
	float				resistance_jump_clinged			= 0.0f;

	bool				jump_h_right = false;

	
	bool				dead_jumping					= false;
	bool				revive							= false;

	bool				draw_debug						= false;

	bool				jumped							= false;
	bool				dashed							= false;
	bool				clinging						= false;

	PLAYER_STATE		state;

	Uint32				start_time						= 0u;
	Uint32				time_to_jump					= 0u;
	Uint32				time_to_do_fade_to_black		= 0u;
	float				time_in_fade					= 0.0f;

	int					offset_value					= 0;

	float				max_speed						= 0.0f;
	float				min_speed						= 0.0f;
	float				dt_multiplied					= 0.0f;

};


#endif // !__j1PLAYER_H__
