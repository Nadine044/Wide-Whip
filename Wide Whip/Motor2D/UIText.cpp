#include "UIText.h"

#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "MGui.h"

bool UIText::PostUpdate(SDL_Texture* atlas) //don't use the atlas in text. Override the function and use their own texture.
{
	//App->render->DrawQuad(rect_world, 255, 0, 0, 50, true, false);
	App->render->Blit(texture_text, world_pos_final.x, world_pos_final.y, &rect_spritesheet_final, 1.0f, SDL_FLIP_NONE, false);

	return true;
}

bool UIText::CleanUp()
{
	UIObject::CleanUp();
	App->tex->UnLoad(texture_text);
	texture_text = nullptr;
	return true;
}