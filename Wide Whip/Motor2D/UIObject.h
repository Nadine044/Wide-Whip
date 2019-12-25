#ifndef __UIENTITY_H__
#define __UIENTITY_H__

#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "j1Textures.h"

class SDL_Texture;


enum class UIType
{
	NONE = -1,
	IMAGE,
	TEXT,
	BUTTON,
	INPUTTEXT
};

class UIObject
{
public:
	UIObject(iPoint pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent);
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt);
	void SetAllPos(iPoint &mouse_move);
	virtual void SetPos(iPoint & mouse_move);
	virtual bool PostUpdate(SDL_Texture* atlas);

	const bool MouseInRect() const;


public:
	p2List<UIObject*> childrens;
	UIObject* parent;
	bool dragging = false;
	bool draggable = false;
	UIType type = UIType::NONE;
	iPoint local_pos;
	iPoint world_pos_original;
	iPoint world_pos_final;
	SDL_Rect rect_spritesheet_original;
	SDL_Rect rect_spritesheet_final;
	SDL_Rect rect_world;

};

#endif // !__UIENTITY_H__

