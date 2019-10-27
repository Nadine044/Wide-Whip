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

	pugi::xml_node time_in_fade_node	= config.child("time_fade");	
	time_to_do_fade_to_black			= (Uint32)(time_in_fade_node.attribute("time_to_do_fade_to_black").as_float() * 1000.0f);
	time_to_jump						= (Uint32)(time_in_fade_node.attribute("time_to_jump").as_float() * 1000.0f);
	time_in_fade						= time_in_fade_node.attribute("time_in_fade").as_float();

	pugi::xml_node rect_limit			= config.child("rect_limit_camera_border");
	rect_limit_camera_border_x			= rect_limit.attribute("x").as_int();
	rect_limit_camera_border_y			= rect_limit.attribute("y").as_int();
	rect_limit_camera.w					= rect_limit.attribute("w").as_float();
	rect_limit_camera.h					= rect_limit.attribute("h").as_float();
	
	map_left_offset						= config.child("map_left_offset_x").attribute("value").as_int();

	jump_force							= config.child("jump_force").attribute("value").as_uint();
	gravity								= config.child("gravity").attribute("value").as_float();

	dash_force							= config.child("dash_force").attribute("value").as_int();
	resistance_dash						= config.child("resistance_dash").attribute("value").as_float();


	jump_clinged_force_left				= config.child("jump_clinged_force").attribute("value").as_int() * 1.1f;
	jump_clinged_force_right			= config.child("jump_clinged_force").attribute("value").as_int() * 1.f;
	resistance_jump_clinged				= config.child("resistance_jump_clinged").attribute("value").as_float();

	speed								= config.child("speed").attribute("value").as_int();

	text_path							= config.child_value("texture");


	jump_clinged_force_left				= config.child("jump_clinged_force").attribute("value").as_int() * 1.1f;
	jump_clinged_force_right			= config.child("jump_clinged_force").attribute("value").as_int() * 1.f;
	resistance_jump_clinged				= config.child("resistance_jump_clinged").attribute("value").as_float();

	speed								= config.child("speed").attribute("value").as_int();

	text_path							= config.child_value("texture");


	idle.PushBack({ 0, 0, 47, 65 });
	idle.PushBack({ 47, 0, 47, 65 });
	idle.PushBack({ 94, 0, 47, 65 });
	idle.PushBack({ 141, 0, 47, 65 });

	idle.loop = true;
	idle.speed = 0.2;

	//jump.PushBack({ 0, 67, 47, 65 });
	//jump.PushBack({ 47, 67, 47, 65 });
	//jump.PushBack({ 97, 67, 47, 65 });
	jump.PushBack({ 141, 67, 47, 65 });
	jump.PushBack({ 188, 67, 47, 65 });
	jump.PushBack({ 235, 67, 47, 65 });
	jump.PushBack({ 282, 67, 47, 65 });
	//jump.PushBack({ 229, 67, 47, 65 });
	//jump.PushBack({ 502, 67, 47, 65 });

	jump.loop = false;
	jump.speed = 0.1;


	walk.PushBack({ 0, 130, 47, 65});
	walk.PushBack({ 47, 130, 47, 65 });
	walk.PushBack({ 94, 130, 47, 65 });
	walk.PushBack({ 141, 130, 47, 65 });
	walk.PushBack({ 188, 130, 47, 65 });
	walk.PushBack({ 235, 130, 47, 65 });

	walk.loop = true;
	walk.speed = 0.2;

	death.PushBack({ 0, 195, 47, 65 });
	death.PushBack({ 47, 195, 47, 65 });
	death.PushBack({ 94, 195, 47, 65 });
	death.PushBack({ 141, 195, 47, 65 });

	death.loop = true;
	death.speed = 0.2;

	dash.PushBack({ 0, 260, 47, 65 });
	dash.PushBack({ 47, 260, 47, 65 });
	dash.PushBack({ 94, 260, 47, 65 });
	dash.PushBack({ 141, 260, 47, 65 });
	dash.PushBack({ 188, 260, 47, 65 });
	dash.PushBack({ 235, 260, 47, 65 });

	dash.loop = false;
	dash.speed = 0.2;

	climb.PushBack({ 0, 325, 47, 65 });

	climb.loop = false;


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
	if (pos.x > map_left_offset)//final map offset
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

		ToAction();

		Movement();

		Gravity();

		JumpHorizontal();

		UpdateCameraPos();

	

		break;
	case PLAYER_STATE::DASHING:
		pos.x += velocity_dash;

		currentAnimation = &dash;

		
		 if (flip == SDL_RendererFlip::SDL_FLIP_NONE)
		 {
			 velocity_dash -= resistance_dash;
			 if (velocity_dash <= 0.0f)
			 {
				 state = PLAYER_STATE::LIVE;
			 }
		 }
		 else
		 {
			 velocity_dash += resistance_dash;
			 if (velocity_dash >= 0.0f)
			 {
				 state = PLAYER_STATE::LIVE;
			 }
		 }

		 UpdateCameraPos();


		break;
	case PLAYER_STATE::CLIMBING:


		if ((App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && !jump_h_right) || (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && jump_h_right))
		{
			jumped = true;
			clinging = false;
			state = PLAYER_STATE::LIVE;
		}

		ToAction();


		break;
	case PLAYER_STATE::DEAD:

		currentAnimation = &death;

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

void j1Player::JumpHorizontal()
{
	if (velocity_jump_clinged < 0 && !jump_h_right)
	{
		pos.x += velocity_jump_clinged;
		velocity_jump_clinged += resistance_jump_clinged;
	}
	else if (velocity_jump_clinged > 0 && jump_h_right)
	{
		pos.x += velocity_jump_clinged;
		velocity_jump_clinged -= resistance_jump_clinged;
	}
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

void j1Player::ToAction()
{
	// Jump-----------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jumped)
	{
		jump.Reset();
		if (!clinging)
		{
			currentAnimation = &jump;
			velocity = jump_force;
			jumped = true;
		}
		else
		{
			state = PLAYER_STATE::LIVE;
			clinging = false;
			currentAnimation = &jump;
			velocity = jump_force *0.75f; //jump less
			jumped = true;
			jump_h_right ? velocity_jump_clinged = jump_clinged_force_left : velocity_jump_clinged = -jump_clinged_force_right;
		}
	}


	// Dash----------------
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !dashed)
	{
		velocity = 0.0f;
		flip == SDL_RendererFlip::SDL_FLIP_NONE ? velocity_dash = dash_force : velocity_dash = -dash_force;
		state = PLAYER_STATE::DASHING;
		dashed = true;
	}
}

void j1Player::Movement()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (currentAnimation != &jump)
			currentAnimation = &walk;

		pos.x -= speed;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		if (currentAnimation != &jump)
			currentAnimation = &walk;

		pos.x += speed;
		flip = SDL_FLIP_NONE;
		
	}
}

void j1Player::Revive()
{
	state = PLAYER_STATE::LIVE;
	App->scene->StartThisLevel();
	velocity = 0.0f;
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
		jumped = false;
		dashed = false;
	}
	if (col->last_colision_direction == DISTANCE_DIR::LEFT && col2->tag == TAG::WALL)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			jumped = false;
			clinging = true;
			velocity = 0.0f;
			state = PLAYER_STATE::CLIMBING;
			jump_h_right = false;
		}
	}
	if (col->last_colision_direction == DISTANCE_DIR::RIGHT && col2->tag == TAG::WALL)
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			jumped = false;
			clinging = true;
			velocity = 0.0f;
			state = PLAYER_STATE::CLIMBING;
			jump_h_right = true;
		}
	}



}

void j1Player::Death()
{
	col->Disable();
	state = PLAYER_STATE::DEAD;	
	start_time = SDL_GetTicks();
	dead_jumping = false;
}

bool j1Player::Draw() const
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

bool j1Player::Save(pugi::xml_node& save_file) const
{
	pugi::xml_node pos_node = save_file.append_child("position");
	pos_node.append_attribute("x") = pos.x;
	pos_node.append_attribute("y") = pos.y;

	save_file.append_child("velocity").append_attribute("value") = GetVelocity();
	save_file.append_child("state").append_attribute("value") = (int)state;

	save_file.append_child("flip").append_attribute("value") = flip;

	save_file.append_child("collider").append_attribute("enabled") = col->IsEnabled();

	save_file.append_child("jumped").append_attribute("value") = jumped;
	save_file.append_child("dashed").append_attribute("value") = dashed;

	return true;
}

bool j1Player::Load(pugi::xml_node& save_file)
{
	pos.x = save_file.child("position").attribute("x").as_int();
	pos.y = save_file.child("position").attribute("y").as_int();

	velocity = save_file.child("velocity").attribute("value").as_float();
	state = PLAYER_STATE(save_file.child("state").attribute("value").as_int());

	flip = (SDL_RendererFlip)save_file.child("flip").attribute("value").as_int();

	save_file.child("collider").attribute("enabled").as_bool() ? col->Enable() : col->Disable();

	jumped = save_file.child("jumped").attribute("value").as_bool();
	dashed = save_file.child("dashed").attribute("value").as_bool();

	return true;
}

float j1Player::GetVelocity() const
{
	return velocity;
}

