#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Player.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "j1Scene.h"
#include "Entity.h"
#include "ModuleEntityManager.h"


j1Player::j1Player(SDL_Rect& rect) : Entity(EntityType::PLAYER, rect)
{}

//Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(const pugi::xml_node& config)
{
	LOG("Loading Player Parser");
	bool ret = true;

	pugi::xml_node player_node = config.child("player");

	pugi::xml_node time_in_fade_node	= player_node.child("time_fade");
	time_to_do_fade_to_black			= (Uint32)(time_in_fade_node.attribute("time_to_do_fade_to_black").as_float() * 1000.0f);
	time_to_jump						= (Uint32)(time_in_fade_node.attribute("time_to_jump").as_float() * 1000.0f);
	time_in_fade						= time_in_fade_node.attribute("time_in_fade").as_float();

	dt_multiplied						= config.previous_sibling("app").child("dt_multiplied").attribute("value").as_int();

	pugi::xml_node rect_limit			= player_node.child("rect_limit_camera_border");
	rect_limit_camera_border_x			= rect_limit.attribute("x").as_int();
	rect_limit_camera_border_y			= rect_limit.attribute("y").as_int();
	rect_limit_camera.w					= rect_limit.attribute("w").as_float();
	rect_limit_camera.h					= rect_limit.attribute("h").as_float();
	
	map_left_offset						= player_node.child("map_left_offset_x").attribute("value").as_int();

	jump_force							= player_node.child("jump_force").attribute("value").as_uint();
	gravity								= player_node.child("gravity").attribute("value").as_float();

	dash_force							= player_node.child("dash_force").attribute("value").as_float();
	resistance_dash						= player_node.child("resistance_dash").attribute("value").as_float();


	jump_clinged_force_left				= player_node.child("jump_clinged_force").attribute("value").as_int() * 1.1f;
	jump_clinged_force_right			= player_node.child("jump_clinged_force").attribute("value").as_int() * 1.f;
	resistance_jump_clinged				= player_node.child("resistance_jump_clinged").attribute("value").as_float();

	speed								= player_node.child("speed").attribute("value").as_int();
	max_speed							= player_node.child("max_speed").attribute("value").as_int();
	min_speed							= player_node.child("min_speed").attribute("value").as_int();

	text_path							= player_node.child_value("texture");


	jump_clinged_force_left				= player_node.child("jump_clinged_force").attribute("value").as_int() * 1.1f;
	jump_clinged_force_right			= player_node.child("jump_clinged_force").attribute("value").as_int() * 1.f;
	resistance_jump_clinged				= player_node.child("resistance_jump_clinged").attribute("value").as_float();

	speed								= player_node.child("speed").attribute("value").as_int();

	text_path							= player_node.child_value("texture");

	pugi::xml_node animations_node = player_node.child("animations");
	idle.LoadAnimation(animations_node.child("idle"));
	jump.LoadAnimation(animations_node.child("jump"));
	walk.LoadAnimation(animations_node.child("walk"));
	jump.LoadAnimation(animations_node.child("jump"));
	death.LoadAnimation(animations_node.child("death"));
	dash.LoadAnimation(animations_node.child("dash"));
	climb.LoadAnimation(animations_node.child("climb"));
	fall.LoadAnimation(animations_node.child("fall"));

	pugi::xml_node audio_node = player_node.child("audios");

	jump_fx.path = audio_node.child_value("jump");
	jump_fx.id = App->audio->LoadFx(jump_fx.path.GetString());
	dash_fx.path = audio_node.child_value("dash");
	dash_fx.id = App->audio->LoadFx(dash_fx.path.GetString());
	death_init_fx.path = audio_node.child_value("death_start");
	death_init_fx.id = App->audio->LoadFx(death_init_fx.path.GetString());
	death_finish_fx.path = audio_node.child_value("death_finish");
	death_finish_fx.id = App->audio->LoadFx(death_finish_fx.path.GetString());

	offset_value = player_node.child("offset_value").attribute("value").as_int();

	return ret;
}

bool j1Player::Start()
{
	LOG("Loading Player textures");
	bool ret = true;

	pos.x = col->rect.x;
	pos.y = col->rect.y;	

	velocity = 0.f;

	//App->player->Load("XMLs/player.xml");
	//App->module_entity_manager->getPlayer()->Load("XMLs/player.xml");
	
	text = App->tex->Load(text_path.GetString());

	flip = SDL_FLIP_NONE;

	current_animation = &idle;
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
		rect_limit_camera.x = -App->render->camera.x + rect_limit_camera_border_x;
	}
	if (pos.y < 675)
	{

		if (pos.y < rect_limit_camera.y)
		{
			App->render->camera.y = -(pos.y - rect_limit_camera_border_y);
		}
		else if (pos.y + col->rect.h > rect_limit_camera.y + rect_limit_camera.h)
		{
			App->render->camera.y = -(pos.y + col->rect.h - rect_limit_camera.h - rect_limit_camera_border_y);
		}
		rect_limit_camera.y = -App->render->camera.y + rect_limit_camera_border_y;
	}
	

}

bool j1Player::Update(float dt)
{

	switch (state)
	{
	case PLAYER_STATE::LIVE:

		if (velocity < -gravity && current_animation != &jump)
		{
			current_animation = &fall;
		}

		ToAction();

		Movement(dt);

		Gravity(dt);

		JumpHorizontal(dt);

		UpdateCameraPos();

		

		break;
	case PLAYER_STATE::DASHING:
		pos.x += velocity_dash * dt * dt_multiplied;

		current_animation = &dash;

		
		 if (flip == SDL_RendererFlip::SDL_FLIP_NONE)
		 {
			 velocity_dash -= resistance_dash * dt * dt_multiplied;
			 if (velocity_dash <= 0.0f)
			 {
				 state = PLAYER_STATE::LIVE;
			 }
		 }
		 else
		 {
			 velocity_dash += resistance_dash * dt * dt_multiplied;
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
			//jumped = true;
			clinging = false;
			state = PLAYER_STATE::LIVE;
		}

		ToAction();


		break;
	case PLAYER_STATE::DEAD:

		current_animation = &death;

		if (SDL_GetTicks() - start_time >= time_to_jump && !dead_jumping)
		{
			velocity = jump_force;
			dead_jumping = true;
			start_time = SDL_GetTicks();
			App->audio->PlayFx(death_finish_fx.id);
		}

		if (dead_jumping)
		{
			Gravity(dt);
			if (SDL_GetTicks() - start_time >= time_to_do_fade_to_black)
			{
				App->fade_to_black->FadeToBlack(revive, time_in_fade);
			}
		}

		if (revive)
		{
			Revive();
			return false;
		}

		break;
	case PLAYER_STATE::GOD:

		Movement(dt);

		VerticalMovement(dt);

		UpdateCameraPos();

		break;
	case PLAYER_STATE::UNKNOWN:
		LOG("This state is Unknown! check the current state and change it");
		break;
	default:
		break;
	}
	if (current_animation == &climb)
		flip == SDL_RendererFlip::SDL_FLIP_NONE ? offset_animation_x = offset_value : offset_animation_x = -offset_value;
	else
		offset_animation_x = 0;

	CheckDebugKeys();

	return true;
}

void j1Player::JumpHorizontal(float dt)
{
	if (velocity_jump_clinged < 0 && !jump_h_right)
	{
		pos.x += velocity_jump_clinged * dt * dt_multiplied;
		velocity_jump_clinged += resistance_jump_clinged * dt * dt_multiplied;
	}
	else if (velocity_jump_clinged > 0 && jump_h_right)
	{
		pos.x += velocity_jump_clinged * dt * dt_multiplied;
		velocity_jump_clinged -= resistance_jump_clinged * dt * dt_multiplied;
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

void j1Player::VerticalMovement(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		pos.y -= speed * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		pos.y += speed * dt;
	}
}

void j1Player::Gravity(float dt)
{
	velocity -= gravity * dt * dt_multiplied;

	int v_y_final = velocity * dt * dt_multiplied;

	if (v_y_final > max_speed)
		v_y_final = max_speed;

	if (v_y_final < min_speed)
		v_y_final = min_speed;

	LOG("%i", v_y_final);
	pos.y -= v_y_final;

}

void j1Player::ToAction()
{
	// Jump-----------------
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jumped)
	{
		App->audio->PlayFx(jump_fx.id);
		jump.Reset();
		if (!clinging)
		{
			current_animation = &jump;
			velocity = jump_force;
			jumped = true;
		}
		else
		{
			state = PLAYER_STATE::LIVE;
			clinging = false;
			current_animation = &jump;
			velocity = jump_force * 0.75f;//jump less
			jumped = true;
			jump_h_right ? velocity_jump_clinged = jump_clinged_force_left : velocity_jump_clinged = -jump_clinged_force_right;
		}
	}


	// Dash----------------
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && !dashed)
	{
		App->audio->PlayFx(dash_fx.id);
		velocity = 0.0f;
		flip == SDL_RendererFlip::SDL_FLIP_NONE ? velocity_dash = dash_force : velocity_dash = -dash_force;
		state = PLAYER_STATE::DASHING;
		dashed = true;
	}
}

void j1Player::Movement(float dt)
{
	int final_v = speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (current_animation != &jump && current_animation != &fall)
			current_animation = &walk;

		pos.x -= final_v;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

		if (current_animation != &jump && current_animation != &fall)
			current_animation = &walk;

		pos.x += final_v;
		flip = SDL_FLIP_NONE;
		
	}
}

void j1Player::Revive()
{
	state = PLAYER_STATE::LIVE;
	velocity = 0.0f;
	App->scene->StartThisLevel();
}

bool j1Player::PostUpdate()
{
	//MYTODO
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
	if (col->last_colision_direction == DISTANCE_DIR::UP && velocity <= 0.0f)
	{
		velocity = 0.0f;
		jump.Reset();
		fall.Reset();
		if (current_animation == &walk)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
				current_animation = &idle;
		}
		else
			current_animation = &idle;

		jumped = false;
		dashed = false;
	}
	else if (col->last_colision_direction == DISTANCE_DIR::LEFT && col2->tag == TAG::WALL && current_animation != &walk)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			jumped = false;
			clinging = true;
			velocity = 0.0f;
			state = PLAYER_STATE::CLIMBING;
			jump_h_right = false;
			current_animation = &climb;
		}
	}
	else if (col->last_colision_direction == DISTANCE_DIR::RIGHT && col2->tag == TAG::WALL && current_animation != &walk)
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			jumped = false;
			clinging = true;
			velocity = 0.0f;
			state = PLAYER_STATE::CLIMBING;
			jump_h_right = true;
			current_animation = &climb;
		}
	}

	if (col2->tag == TAG::CHANGE_LEVEL)
	{
		App->scene->ChangeBetweenLevelFade();
	}


}

void j1Player::Death()
{
	col->Disable();
	App->audio->PlayFx(death_init_fx.id);
	state = PLAYER_STATE::DEAD;	
	start_time = SDL_GetTicks();
	dead_jumping = false;
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

	save_file.append_child("entity_type").append_attribute("value") = (int)type;

	save_file.append_child("velocity").append_attribute("value") = GetVelocity();
	save_file.append_child("state").append_attribute("value") = (int)state;

	save_file.append_child("flip").append_attribute("value") = flip;

	save_file.append_child("collider").append_attribute("enabled") = col->IsEnabled();

	save_file.append_child("jumped").append_attribute("value") = jumped;
	save_file.append_child("dashed").append_attribute("value") = dashed;

	save_file.append_child("velocity_dash").append_attribute("value") = velocity_dash;
	save_file.append_child("velocity_jump_clinged").append_attribute("value") = velocity_jump_clinged;

	return true;
}

bool j1Player::Load(pugi::xml_node& save_file)
{
	pos.x = save_file.child("position").attribute("x").as_int();
	pos.y = save_file.child("position").attribute("y").as_int();

	type = EntityType(save_file.child("entity_type").attribute("value").as_int());

	velocity = save_file.child("velocity").attribute("value").as_float();
	state = PLAYER_STATE(save_file.child("state").attribute("value").as_int());

	flip = (SDL_RendererFlip)save_file.child("flip").attribute("value").as_int();

	save_file.child("collider").attribute("enabled").as_bool() ? col->Enable() : col->Disable();

	jumped = save_file.child("jumped").attribute("value").as_bool();
	dashed = save_file.child("dashed").attribute("value").as_bool();

	velocity_dash = save_file.child("velocity_dash").attribute("value").as_float();
	velocity_jump_clinged = save_file.child("velocity_jump_clinged").attribute("value").as_float();

	return true;
}

float j1Player::GetVelocity() const
{
	return velocity;
}

