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

struct PlayerData
{

	float			position_x, position_y;
	float			speed_x, speed_y;
	float			acceleration;
	const char*		image;
	//animations attributes load MYTODO

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

	void Gravity();

	void Jump();

	void Movement();

	void Revive();

	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	//Load player file
	bool Load(const char* path);

	void OnTrigger(Collider* col2); // this will be virtual in the class object parent when ObjectManager will be created.

	float GetVelocity() const;

public:

	//Animations

	//New player
	PlayerData	data;

	//Animation*	currentAnimation = nullptr;

	//Animation	idle;
	Animation	jump;

	iPoint pos;

	Collider* col;

	SDL_RendererFlip flip = SDL_FLIP_NONE;


private:

	bool LoadPlayer();

	void Death();

private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;

	SDL_Texture* text = nullptr;
	SDL_Texture* text2 = nullptr;

	SDL_Rect rect_limit_camera;
	int rect_limit_camera_border_x = 100;
	int rect_limit_camera_border_y = 150;


	uint jump_force = 10;
	float velocity = 0.0f;
	float gravity = 0.3f;

	bool jumping = false;

	bool revive = false;

	PLAYER_STATE		state;

	Uint32 start_time = 0u;
	float time_to_jump = 1.f;
	float time_to_do_fade_to_black = 2.f;

	Uint32 Utime_to_jump = 1.f;
	Uint32 Utime_to_do_fade_to_black = 2.f;

	bool dead_jumping = false;

};


#endif // !__j1PLAYER_H__
