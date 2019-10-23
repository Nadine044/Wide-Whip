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


class Collider;

enum PlayerState
{
	AIR,
	LAND,
	UNKNOWN
};

struct PlayerData
{
	PlayerState		state;


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

	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	//Load player file
	bool Load(const char* path);

	void OnTrigger(Collider* col2); // this will be virtual in the class object parent when ObjectManager will be created.

public:

	//Animations

	//New player
	PlayerData	data;

	//Animation*	currentAnimation = nullptr;

	//Animation	idle;
	Animation	jump;

	iPoint pos;

	Collider* col;

	SDL_Rect playerCamera {pos.x + 300, pos.y - 300, 600, 600};


private:

	bool LoadPlayer();


private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;

	SDL_Texture* text = nullptr;
	SDL_Texture* text2 = nullptr;

	
	uint jump_force = 1;
	float velocity = 0.0f;
	float gravity = 0.005f;

	bool jumping = false;



};


#endif // !__j1PLAYER_H__
