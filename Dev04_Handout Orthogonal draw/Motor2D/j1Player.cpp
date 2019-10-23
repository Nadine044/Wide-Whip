#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Player.h"
#include "ModuleCollision.h"
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

	pos.x = col->rect.x;
	pos.y = col->rect.y;
	col->object = this;	

	UpdateCameraPos();

	App->player->Load("XMLs/player.xml");
	text = App->tex->Load("player/player.png");
	text2 = App->tex->Load("player/jump.png");

	jump.PushBack({ 0, 0, 32, 32 });
	jump.PushBack({ 32, 0, 32, 32 });
	jump.PushBack({ 64, 0, 32, 32 });
	jump.PushBack({ 96, 0, 32, 32 });
	jump.PushBack({ 128, 0, 32, 32 });
	jump.PushBack({ 160, 0, 32, 32 });
	jump.PushBack({ 192, 0, 32, 32 });
	jump.PushBack({ 224, 0, 32, 32 }); //don't work well

	jump.loop = true;
	jump.speed = 0.1;


	return ret;
}

void j1Player::UpdateCameraPos()
{
	App->render->camera.x = -pos.x + CAMERA_OFFSET_X;
	App->render->camera.y = -pos.y + CAMERA_OFFSET_Y;
}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
		pos.y -= 2;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		pos.y += 2;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos.x -= 2;
		App->render->camera.x += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) {
		pos.x += 2;
		App->render->camera.x -= 1;
	}


	// Jump-----------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		velocity = jump_force;
	}	


	velocity -= gravity;
	pos.y += -velocity;

	col->UpdatePos(pos);
	UpdateCameraPos();
	
	return true;
}

bool j1Player::PostUpdate()
{
	//MYTODO
	Draw();
	return true;
}

void j1Player::OnTrigger(Collider* col2)
{
	//Acces to the other colldier when a collision is checked.
	//Do Something when a collisions is checked.
	LOG("it's this a collision!");
	if (col->last_colision_direction == DISTANCE_DIR::UP)
	{
		velocity = 0.0f;
	}

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

	SDL_Rect rect2 = { 0, 0, 32, 32 };	

	SDL_Rect r = SDL_Rect{ 0,0,32,32 };
	App->render->Blit(text, pos.x, pos.y, &(jump.GetCurrentFrame()));

	App->render->Blit(text2, pos.x + 32, pos.y, &(jump.GetCurrentFrame()));


	return true;
}

bool j1Player::CleanUp()
{
	//MYTODO
	LOG("Player unloaded");

	return true;
}