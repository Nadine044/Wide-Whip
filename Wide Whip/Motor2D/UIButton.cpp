#include "UIButton.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1Input.h"
#include "MGui.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Scene.h"

UIButton::UIButton(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent)
{
	type = _type;
}

bool UIButton::PreUpdate()
{
	bool ret = true;

	if (enabled)
	{
		switch (state)
		{
		case UIButton::UIButtonState::NONE:
			if (MouseInRect())
			{
				state = UIButtonState::HOVER;
				current_image = hover;
				LOG("Mouse Enter!");
				App->audio->PlayFx(App->scene->button_hover_sfx.id);
			}
			break;
		case UIButton::UIButtonState::HOVER:
			if (!MouseInRect())
			{
				state = UIButtonState::NONE;
				current_image = background;
				LOG("Mouse Exit!");
			}
			else if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				state = UIButtonState::CLICKED;
				current_image = clicked;
				App->audio->PlayFx(App->scene->button_click_sfx.id);
			}
			break;
		case UIButton::UIButtonState::CLICKED:
			if (App->input->GetMouseButtonDown(1) == KEY_UP)
			{
				if (MouseInRect())
				{
					state = UIButtonState::HOVER;
					current_image = hover;
					ret = listener->ButtonEvent(button_type);
				}
				else
				{
					state = UIButtonState::NONE;
					current_image = background;
				}
			}

			break;
		default:
			break;
		}
	}

	return ret;
}

bool UIButton::Update(float dt)
{
	if (focus && (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) && enabled)
		listener->ButtonEvent(button_type);
	return false;
}

bool UIButton::PostUpdate(SDL_Texture* atlas)
{
	//App->render->DrawQuad(rect_world, 255, 0, 0, 50, true, false);
	current_image->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	text->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	if(!enabled)
		App->render->DrawQuad(rect_world, 0, 0, 0, 150, true, false);

	else if(focus)
		App->render->DrawQuad(rect_world, 0, 0, 0, 50, true, false);


	return true;
}

bool UIButton::CleanUp()
{
	UIObject::CleanUp();
	listener = nullptr;

	text->CleanUp();
	RELEASE(text);

	background->CleanUp();
	RELEASE(background);
	hover->CleanUp();
	RELEASE(hover);
	clicked->CleanUp();
	RELEASE(clicked);

	current_image = nullptr;

	return true;
}
