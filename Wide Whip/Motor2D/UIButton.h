#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIImage.h";
#include "UIText.h"



class UIButton : public UIObject
{
	enum class UIButtonState
	{
		NONE,
		HOVER,
		CLICKED,
	};

	enum class UIButtonEvents
	{
		MOUSE_ENTER,
		MOUSE_EXIT,
		CLICKED,
		RELEASED
	};

public:
	UIButton(UIType _type, iPoint pos, SDL_Rect rect_spritesheet);

	bool PreUpdate() override;
	bool PostUpdate(SDL_Texture* atlas) override;

	const bool MouseInRect() const;

public:
	UIButtonState state;
	UIImage* background = nullptr;
	UIImage* hover = nullptr;
	UIImage* clicked = nullptr;
	UIText* text = nullptr;

	UIImage* current_image = nullptr;

};

#endif // !__UIIMAGE_H__
