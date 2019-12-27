#ifndef __UISCROLLBAR_H__
#define __UISCROLLBAR_H__

#include "UIObject.h"
class UIImage;

enum class Orientation
{
	HORIZONTAL,
	VERTICAL
};

enum class UIScrollBarType
{
	TEST,
	MUSIC,
	SFX
};

class UIScrollBar : public UIObject
{
public:

	UIScrollBar(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent) : UIObject(local_pos, rect_spritesheet_original, draggable, parent) { type = _type; }

	bool Update(float dt) override;
	bool PostUpdate(SDL_Texture* atlas) override;
	bool CleanUp() override;

	void SetPosThumb(iPoint& mouse_move);

	void MoveThumbAxisX(uint mouse_move_x);

	void MoveThumbAxisY(uint mouse_move_y);

public:
	Orientation orientation;
	UIImage* background;
	UIImage* thumb;
	j1Module* listener;
	UIScrollBarType scrollbar_type;	

	int musicScrollValue = 0;
};

#endif // !__UISCROLLBAR_H__