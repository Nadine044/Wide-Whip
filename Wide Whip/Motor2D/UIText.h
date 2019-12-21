#ifndef __UITEXT_H__
#define __UITEXT_H__

#include "UIObject.h"

class UIText : public UIObject
{
public:
	UIText(UIType _type, iPoint pos, SDL_Rect rect_spritesheet) : UIObject(pos, rect_spritesheet) { type = _type; }

	bool PostUpdate(SDL_Texture* atlas) override;

public:
	SDL_Texture* texture_text = nullptr;
};

#endif // !__UITEXT_H__
