#ifndef __UITEXT_H__
#define __UITEXT_H__

#include "UIObject.h"

class UIText : public UIObject
{
public:
	UIText(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; }

	bool PostUpdate(SDL_Texture* atlas) override;
	bool CleanUp() override;

	void ChangeText(p2String & new_string);
public:
	SDL_Texture* texture_text = nullptr;
};

#endif // !__UITEXT_H__
