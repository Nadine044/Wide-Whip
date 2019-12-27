#ifndef __MGUI_H__
#define __MGUI_H__

#include "j1Module.h"
#include "UIObject.h"
#include "p2Point.h"
#include "UIButton.h"
#include "UIScrollBar.h"


#define CURSOR_WIDTH 2

class UIText;
class UIImage;
class UIInputText;
class UIScrollBar;
enum class UIButtonType;

// TODO 1: Create your structure of classes

// ---------------------------------------------------
class MGui : public j1Module
{
public:

	MGui();

	// Destructor
	virtual ~MGui();

	// Called when before render is available
	bool Awake(pugi::xml_node&) override;

	// Call before first frame
	bool Start() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called before all Updates
	bool Update(float dt) override;

	// Called after all Updates
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	// TODO 2: Create the factory methods
	// Gui creation functions

	UIImage* CreateUIImage(iPoint pos, SDL_Rect rect_spritesheet_original,  bool draggable = false, UIObject* parent = nullptr);
	UIText* CreateUIText(iPoint pos, p2String text, bool draggable = false, UIObject* parent = nullptr);
	UIButton* CreateUIButton(iPoint pos, p2String text, SDL_Rect image_rect, UIButtonType type, j1Module* listener, bool draggable = false, UIObject* parent = nullptr);
	UIInputText* CreateUIInputText(iPoint pos, p2String text, SDL_Rect image_rect, bool draggable = false, UIObject* parent = nullptr);
	UIScrollBar* CreateUIScrollBar(iPoint pos, SDL_Rect image_rect, Orientation orientation, UIScrollBarType type, j1Module* listener, UIObject* parent = nullptr);

	void InputEvent(char* input);

	SDL_Texture* GetAtlas() const;
	void SetFocus(UIObject* new_focus);
	void SetDragging(UIObject* object);
private:
	UIObject* CreateUIObject(UIType type, iPoint pos, SDL_Rect rect_sprisheet, bool draggable, UIObject* parent);

private:

	p2List<UIObject*> UI_objects;
	SDL_Texture* atlas;
	p2String atlas_file_name;
	UIObject* focus = nullptr;
};

#endif // __MGUI_H__