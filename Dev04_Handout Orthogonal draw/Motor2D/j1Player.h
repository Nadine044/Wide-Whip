#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Textures.h"
//struct SDL_Texture;

#define CAMERA_OFFSET_X 300
#define CAMERA_OFFSET_Y 360
#define MAP_LEFT_OFFSET_X 200


class Collider;

enum class PLAYER_STATE
{
	LIVE,
	DEAD,
	GOD,
	UNKNOWN
};



class j1Player : public j1Module
{
public:

	j1Player();

	//Destructor
	virtual ~j1Player();

	//Called before render is available
	bool Awake(pugi::xml_node& player);

	//Load
	bool Start() override;

	void UpdateCameraPos();

	// Called each loop iteration
	bool Draw();

	bool Update(float dt) override;

	void CheckDebugKeys();

	void VerticalMovement();

	void Gravity();

	void Jump();

	void Movement();

	void Revive();

	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	void OnTrigger(Collider* col2); // this will be virtual in the class object parent when ObjectManager will be created.

	float GetVelocity() const;

public:

	//Animations

	//New player

	Animation*	currentAnimation = nullptr;

	Animation	jump;
	Animation	idle;

	iPoint pos;
	Collider* col;

	


private:

	void Death();

private:

	SDL_Texture*		text							= nullptr;
	p2SString			text_path;

	SDL_Rect			rect_limit_camera;
	int					rect_limit_camera_border_x		= 0;
	int					rect_limit_camera_border_y		= 0;

	uint				jump_force						= 0u;
	float				velocity						= 0.0f;
	float				gravity							= 0.f;

	int					speed							= 0;

	bool				revive							= false;
	bool				dead_jumping					= false;

	bool				draw_debug						= false;

	PLAYER_STATE		state;

	Uint32				start_time						= 0u;
	Uint32				time_to_jump					= 0u;
	Uint32				time_to_do_fade_to_black		= 0u;
	float				time_in_fade					= 0.0f;

	SDL_RendererFlip	flip							= SDL_FLIP_NONE;


};


#endif // !__j1PLAYER_H__
