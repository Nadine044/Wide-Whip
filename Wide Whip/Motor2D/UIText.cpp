#include "UIText.h"

#include "j1App.h"
#include "j1Render.h"
#include "MGui.h"

bool UIText::PostUpdate(SDL_Texture* atlas) //don't use the atlas in text. Override the function and use their own texture.
{
	App->render->Blit(texture_text, world_pos.x, world_pos.y, &rect_spritesheet, 1.0f, SDL_FLIP_NONE, false);

	return true;
}