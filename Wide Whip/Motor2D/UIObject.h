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
	INPUTTEXT,
	SCROLLBAR
};

class UIObject
{
public:
	UIObject(iPoint pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent);
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool CleanUp();
	void SetAllPos(iPoint &mouse_move);
	virtual void SetPos(iPoint & mouse_move);
	virtual bool PostUpdate(SDL_Texture* atlas);

	const bool MouseInRect() const;

	const bool GetVisible() const;
	void SetAllVisible(const bool visible);
	virtual void SetFocusThis(bool focus_value);

private:
	void SetVisible(const bool visible);


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

protected:
	bool focus = false;
	bool visible = true;
};

#endif // !__UIENTITY_H__

