#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UIObject.h"
class UIImage : public UIObject
{
public:
	UIImage(UIType _type, iPoint pos, SDL_Rect rect_spritesheet) : UIObject(pos, rect_spritesheet) { type = _type; }

};

#endif // !__UIIMAGE_H__
