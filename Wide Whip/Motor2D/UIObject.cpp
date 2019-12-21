#include "UIObject.h"
#include "j1App.h"
#include "j1Render.h"
#include "MGui.h"


UIObject::UIObject(iPoint pos, SDL_Rect rect_spritesheet) : pos(pos), rect_spritesheet(rect_spritesheet)
{
	rect_world.x = pos.x;
	rect_world.y = pos.y;
	rect_world.w = rect_spritesheet.w;
	rect_world.h = rect_spritesheet.h;
}

bool UIObject::PostUpdate(SDL_Texture* atlas)
{
	App->render->Blit(atlas, pos.x, pos.y, &rect_spritesheet, 1.0f, SDL_FLIP_NONE, false);

	return true;
}