#ifndef __UIINPUTTEXT_H__
#define __UIINPUTTEXT_H__

#include "UIObject.h"
#include "j1Timer.h"

class UIText;
class UIImage;
class p2String;

class UIInputText : public UIObject
{

public:
	UIInputText(UIType _type, iPoint local_pos, SDL_Rect rect_spritesheet_original, bool draggable, UIObject* parent);

	//bool PreUpdate() override;
	bool PostUpdate(SDL_Texture* atlas) override;
	bool Update(float dt) override;

	void SetCursorPos(p2String &string_cuted);

	void SetPos(iPoint & mouse_move)override;
	void GetInput(char* input);
	void AddCharacter(const char* input);
public:
	p2String text_string;
	UIText* text;
	UIText* input;
	UIText* current_text;
	SDL_Rect cursor;
	int cursor_int = 0;
	iPoint cursor_original_pos;
	UIImage* background = nullptr;

private:
	j1Timer timer_blink;
	uint time_to_blink = 500u;

};

#endif // !__UIINPUTTEXT_H__
