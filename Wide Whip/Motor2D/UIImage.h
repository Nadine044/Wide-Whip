#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UIObject.h"
class UIImage : public UIObject
{
public:
	UIImage(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; }

};

#endif // !__UIIMAGE_H__
