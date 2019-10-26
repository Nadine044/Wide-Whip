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

bool j1Player::Start()
{
	LOG("Loading Player textures");
	bool ret = true;

	pos.x = col->rect.x;
	pos.y = col->rect.y;	

	UpdateCameraPos();

	App->player->Load("XMLs/player.xml");
	text = App->tex->Load("player/player.png");
	text2 = App->tex->Load("player/jump.png");



	rect_limit_camera.x = App->render->camera.x + rect_limit_camera_border_x;
	rect_limit_camera.y = App->render->camera.y + rect_limit_camera_border_y;
	rect_limit_camera.w = 450;
	rect_limit_camera.h = 500;

	return ret;
}

void j1Player::UpdateCameraPos()
{
	if (pos.x > MAP_LEFT_OFFSET_X)//final map offset
	{
		if (pos.x < rect_limit_camera.x)
		{
			App->render->camera.x = -(pos.x - rect_limit_camera_border_x);
		}
		else if (pos.x + col->rect.w > rect_limit_camera.x + rect_limit_camera.w)
		{
			App->render->camera.x = -(pos.x + col->rect.w -rect_limit_camera.w - rect_limit_camera_border_x);
		}
		//App->render->camera.x = -pos.x + MAP_LEFT_OFFSET_X;

	}

	if (pos.y < rect_limit_camera.y)
	{
		App->render->camera.y = -(pos.y - rect_limit_camera_border_y);
	}
	else if (pos.y + col->rect.h > rect_limit_camera.y + rect_limit_camera.h)
	{
		App->render->camera.y = -(pos.y + col->rect.h - rect_limit_camera.h - rect_limit_camera_border_y);
	}
	//App->render->camera.y = -pos.y + CAMERA_OFFSET_Y;	
}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
		pos.y -= 3;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) {
		pos.y += 3;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos.x -= 3;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) {
		pos.x += 3;
		flip = SDL_FLIP_NONE;
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

	rect_limit_camera.x = -App->render->camera.x + rect_limit_camera_border_x;
	rect_limit_camera.y = -App->render->camera.y + rect_limit_camera_border_y;
	
	return true;
}

bool j1Player::PostUpdate()
{
	//MYTODO
	Draw();
	//App->render->DrawQuad(rect_limit_camera, 0, 0, 100, 100);
	return true;
}

void j1Player::OnTrigger(Collider* col2)
{
	//Acces to the other colldier when a collision is checked.
	//Do Something when a collisions is checked.
	LOG("it's this a collision!");
	if (col->last_colision_direction == DISTANCE_DIR::UP && velocity <= 0.0f)
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
	App->render->Blit(text, pos.x, pos.y, &(jump.GetCurrentFrame()), 1.0f, flip);

	App->render->Blit(text2, pos.x + 32, pos.y, &(jump.GetCurrentFrame()), 1.0f, flip);

	return true;
}

bool j1Player::CleanUp()
{
	//MYTODO
	LOG("Player unloaded");
	App->tex->UnLoad(text);
	App->tex->UnLoad(text2);
	return true;
}

float j1Player::GetVelocity() const
{
	return velocity;
}