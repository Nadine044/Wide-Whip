#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Defs.h"

struct SDL_Texture;

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
	//bool Awake(pugi::xml_node& player);

	// Called each loop iteration
	//void Draw();

	// Called before quitting
	//bool CleanUp();

	//Load player file
	bool Load(const char* path);


public:

	//Animations

	//New player
	PlayerData	data;

private:

	bool LoadPlayer();


private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;
};


#endif // !__j1PLAYER_H__
