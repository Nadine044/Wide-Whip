#include "UIButton.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1Input.h"
#include "MGui.h"
#include "j1Render.h"

UIButton::UIButton(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent)
{
	type = _type;
}

bool UIButton::PreUpdate()
{
	if (MouseInRect() && App->input->GetMouseButtonDown(1) == KEY_DOWN)
		App->gui->SetFocus(this);


	switch (state)
	{
	case UIButton::UIButtonState::NONE:
		if (MouseInRect())
		{
			state = UIButtonState::HOVER;
			current_image = hover;
			LOG("Mouse Enter!");
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

		}
		break;
	case UIButton::UIButtonState::CLICKED:
		if (App->input->GetMouseButtonDown(1) == KEY_UP)
		{
			if (MouseInRect())
			{
				state = UIButtonState::HOVER;
				current_image = hover;
				listener->ButtonEvent(button_type);
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

	return true;
}

bool UIButton::PostUpdate(SDL_Texture* atlas)
{
	App->render->DrawQuad(rect_world, 255, 0, 0, 50, true, false);
	current_image->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"
	text->PostUpdate(atlas);// TODO:should be this in a fucnction "Draw"

	return true;
}

