#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIImage.h";
#include "UIText.h"

enum class UIButtonType
{
	TEST,
	SETTINGS
};

class UIButton : public UIObject
{
	enum class UIButtonState
	{
		NONE,
		HOVER,
		CLICKED,
	};

	/*enum class UIButtonEvents
	{
		MOUSE_ENTER,
		MOUSE_EXIT,
		CLICKED,
		RELEASED
	};*/



public:
	UIButton(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet, bool draggable, UIObject* parent);

	bool PreUpdate() override;
	bool PostUpdate(SDL_Texture* atlas) override;
public:
	j1Module* listener = nullptr;
	UIButtonType button_type;
	UIButtonState state;
	UIImage* background = nullptr;
	UIImage* hover = nullptr;
	UIImage* clicked = nullptr;
	UIText* text = nullptr;

	UIImage* current_image = nullptr;

};

#endif // !__UIIMAGE_H__
