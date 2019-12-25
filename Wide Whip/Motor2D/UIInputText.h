#ifndef __UIINPUTTEXT_H__
#define __UIINPUTTEXT_H__

#include "UIObject.h"

class UIText;
class UIImage;

class UIInputText : public UIObject
{

public:
	UIInputText(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; };

	//bool PreUpdate() override;
	bool PostUpdate(SDL_Texture* atlas) override;

	void SetPos(iPoint & mouse_move)override;
public:
	UIText* text;
	UIText* input;
	UIText* current_text;
	SDL_Rect cursor;
	UIImage* background = nullptr;

};

#endif // !__UIINPUTTEXT_H__
