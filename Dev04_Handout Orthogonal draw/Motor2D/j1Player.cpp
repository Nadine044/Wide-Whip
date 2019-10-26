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
#include "ModuleFadeToBlack.h"
#include "j1Scene.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

//Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;

	pugi::xml_node time_in_fade_node = config.child("time_fade");	
	time_to_do_fade_to_black = (Uint32)(time_in_fade_node.attribute("time_to_do_fade_to_black").as_float() * 1000.0f);
	time_to_jump = (Uint32)(time_in_fade_node.attribute("time_to_jump").as_float() * 1000.0f);
	time_in_fade = time_in_fade_node.attribute("time_in_fade").as_float();

	pugi::xml_node rect_limit = config.child("rect_limit_camera_border");
	rect_limit_camera_border_x = rect_limit.attribute("x").as_int();
	rect_limit_camera_border_y = rect_limit.attribute("y").as_int();
	rect_limit_camera.w = rect_limit.attribute("w").as_float();
	rect_limit_camera.h = rect_limit.attribute("h").as_float();

	jump_force = config.child("jump_force").attribute("value").as_uint();
	gravity = config.child("gravity").attribute("value").as_float();

	speed = config.child("speed").attribute("value").as_int();

	text_path = config.child_value("texture");


	idle.PushBack({ 0, 67, 47, 61 });
	idle.PushBack({ 72, 67, 47, 61 });
	idle.PushBack({ 144, 67, 48, 61 });
	idle.PushBack({ 216, 67, 50, 61 });

	idle.loop = true;
	idle.speed = 0.2;

	//jump.PushBack({ 0, 0, 47, 67 });
	//jump.PushBack({ 70, 0, 51, 67 });
	//jump.PushBack({ 141, 0, 54, 67 });
	jump.PushBack({ 215, 0, 47, 67 });
	jump.PushBack({ 287, 0, 47, 67 });
	jump.PushBack({ 357, 0, 51, 67 });
	//jump.PushBack({ 430, 0, 52, 67 });
	//jump.PushBack({ 502, 0, 47, 67 });

	jump.loop = false;
	jump.speed = 0.1;


	return ret;
}

bool j1Player::Start()
{
	LOG("Loading Player textures");
	bool ret = true;

	pos.x = col->rect.x;
	pos.y = col->rect.y;	

	//App->player->Load("XMLs/player.xml");
	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	currentAnimation = &idle;
	start_time = 0u;

	rect_limit_camera.x = -App->render->camera.x + rect_limit_camera_border_x;
	rect_limit_camera.y = -App->render->camera.y + rect_limit_camera_border_y;


	UpdateCameraPos();

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
	}

	if (pos.y < rect_limit_camera.y)
	{
		App->render->camera.y = -(pos.y - rect_limit_camera_border_y);
	}
	else if (pos.y + col->rect.h > rect_limit_camera.y + rect_limit_camera.h)
	{
		App->render->camera.y = -(pos.y + col->rect.h - rect_limit_camera.h - rect_limit_camera_border_y);
	}
	
	rect_limit_camera.x = -App->render->camera.x + rect_limit_camera_border_x;
	rect_limit_camera.y = -App->render->camera.y + rect_limit_camera_border_y;
}

bool j1Player::Update(float dt)
{

	switch (state)
	{
	case PLAYER_STATE::LIVE:

		Movement();

		Jump();

		Gravity();

		UpdateCameraPos();

		

		break;
	case PLAYER_STATE::DEAD:
		if (SDL_GetTicks() - start_time >= time_to_jump && !dead_jumping)
		{
			velocity = jump_force;
			dead_jumping = true;
			start_time = SDL_GetTicks();
		}

		if (dead_jumping)
		{
			Gravity();
			if (SDL_GetTicks() - start_time >= time_to_do_fade_to_black)
			{
				App->fade_to_black->FadeToBlack(revive, time_in_fade);
			}
		}

		if (revive)
		{
			Revive();
		}

		break;
	case PLAYER_STATE::GOD:

		Movement();

		VerticalMovement();

		UpdateCameraPos();

		break;
	case PLAYER_STATE::UNKNOWN:
		LOG("This state is Unknown! check the current state and change it");
		break;
	default:
		break;
	}
	CheckDebugKeys();
	col->UpdatePos(pos);
	return true;
}

void j1Player::CheckDebugKeys()
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (state == PLAYER_STATE::LIVE)
		{
			state = PLAYER_STATE::GOD;
			velocity = 0.f;
			col->Disable();
		}
		else if (state == PLAYER_STATE::GOD)
		{
			state = PLAYER_STATE::LIVE;
			col->Enable();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		draw_debug = !draw_debug;
}

void j1Player::VerticalMovement()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		pos.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		pos.y += speed;
	}
}

void j1Player::Gravity()
{
	velocity -= gravity;
	pos.y += -velocity;
}

void j1Player::Jump()
{
	// Jump-----------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		currentAnimation = &jump;
		velocity = jump_force;
	}
}

void j1Player::Movement()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		pos.x -= speed;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos.x += speed;
		flip = SDL_FLIP_NONE;
	}
}

void j1Player::Revive()
{
	state = PLAYER_STATE::LIVE;
	App->scene->StartThisLevel();
}

bool j1Player::PostUpdate()
{
	//MYTODO
	Draw();
	if (draw_debug)
	{
		App->render->DrawQuad(rect_limit_camera, White.r, White.g, White.b, App->collisions->GetAlphaDebug());
	}
	return true;
}

void j1Player::OnTrigger(Collider* col2)
{
	if (col2->tag == TAG::WATER)
	{
		Death();
	}
	//Acces to the other colldier when a collision is checked.
	//Do Something when a collisions is checked.
	LOG("it's this a collision!");
	if (col->last_colision_direction == DISTANCE_DIR::UP && velocity <= 0.0f)
	{
		velocity = 0.0f;
		jump.Reset();
		currentAnimation = &idle;
	}

}

void j1Player::Death()
{
	col->Disable();
	state = PLAYER_STATE::DEAD;	
	start_time = SDL_GetTicks();
	dead_jumping = false;
}

bool j1Player::Draw()
{
	//Animation MYTODO
	//----------------------	
	
		//currentAnimation = &idle;
	App->render->Blit(text, pos.x, pos.y, &(currentAnimation->GetCurrentFrame()), 1.0f, flip);
	

	//App->render->Blit(text2, pos.x, pos.y, &(jump.GetCurrentFrame()), 1.0f, flip);
	//App->render->Blit(text, pos.x, pos.y, &(idle.GetCurrentFrame()), 1.0f, flip);

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Player unloaded");
	App->tex->UnLoad(text);
	return true;
}

float j1Player::GetVelocity() const
{
	return velocity;
}

