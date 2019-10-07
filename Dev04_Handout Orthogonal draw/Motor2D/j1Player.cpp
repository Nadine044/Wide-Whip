#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Player.h"
#include <math.h>

j1Player::j1Player() : j1Module()
{
}

//Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;

	//MYTODO
	folder.create(config.child("folder").child_value());
	
	return ret;
}

bool j1Player::Start()
{
	LOG("Loading Player textures");
	bool ret = true;

	//text = App->tex->Load("player/player.png");

	return ret;
}


bool j1Player::Load(const char* file)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file);

	pugi::xml_parse_result result = player_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load player xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	//Load general info ---------------------------------
	if (ret == true)
	{
		ret = LoadPlayer();
	}

	player_loaded = ret;

	return ret;
}

bool j1Player::LoadPlayer()
{
	bool ret = true;
	pugi::xml_node player = player_file.child("player");

	if (player == NULL)
	{
		LOG("Error parsing player xml file: Cannot find 'player' tag.");
		ret = false;
	}
	else
	{
		data.position_x = player.child("position").attribute("x").as_float();
		data.position_y = player.child("position").attribute("y").as_float();
		data.speed_x = player.child("speed").attribute("x").as_float();
		data.speed_y = player.child("speed").attribute("y").as_float();
		data.acceleration = player.child("acceleration").attribute("a").as_float();
		data.image = player.child("spritesheet").attribute("source").as_string();
		//Load animation info MYTODO
	}

	p2SString player_state(player.child("state").attribute("state").as_string());

	if (player_state == "AIR")
	{
		data.state = AIR;
	}
	else if (player_state == "LAND")
	{
		data.state = LAND;
	}
	else
	{
		data.state = UNKNOWN;
	}

	return ret;

}

bool j1Player::Draw()
{
	//Animation MYTODO
	//----------------------
	SDL_Texture* text2 = nullptr;
	
	text2 = App->tex->Load("player/jump.png");
	text = App->tex->Load("player/player.png");

	SDL_Rect rect2 = { 32, 32 };

	jump.PushBack({ 0, 0, 32, 32});
	jump.PushBack({ 32, 0, 32, 32 });
	jump.PushBack({ 64, 0, 32, 32 });
	jump.PushBack({ 96, 0, 32, 32 });
	jump.PushBack({ 128, 0, 32, 32 });
	jump.PushBack({ 160, 0, 32, 32 });
	jump.PushBack({ 192, 0, 32, 32 });
	jump.PushBack({ 224, 0, 32, 32 });

	jump.loop = true;
	jump.speed = 0.1;

	App->render->Blit(text, 0, 0, &(idle.GetCurrentFrame()), 0.75f);

	//App->render->Blit(text2, 0, 0, &(jump.GetCurrentFrame()), 0.75f);


	return true;
}

bool j1Player::CleanUp()
{
	//MYTODO
	LOG("Player unloaded");

	return true;
}