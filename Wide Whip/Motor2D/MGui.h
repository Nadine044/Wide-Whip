#ifndef __MGUI_H__
#define __MGUI_H__

#include "j1Module.h"
#include "UIObject.h"
#include "p2Point.h"
#include "UIButton.h"


#define CURSOR_WIDTH 2

class UIText;

// TODO 1: Create your structure of classes

// ---------------------------------------------------
class MGui : public j1Module
{
public:

	MGui();

	// Destructor
	virtual ~MGui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	UIObject* CreateUIObject(UIType type, iPoint pos, SDL_Rect rect_sprisheet);
	UIText* CreateUIText(iPoint pos, p2String text);
	UIButton* CreateUIButton(iPoint pos, p2String text, SDL_Rect image_rect);

	SDL_Texture* GetAtlas() const;

private:

	p2List<UIObject*> UI_objects;
	SDL_Texture* atlas;
	p2String atlas_file_name;
};

#endif // __MGUI_H__