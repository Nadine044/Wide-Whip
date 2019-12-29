#ifndef __INLEVEL_H__
#define __INLEVEL_H__

#include "j1Module.h"
#include "j1Audio.h"

class UIImage;
class UIText;

class InLevel : public j1Module
{
public:
	InLevel();

	bool Awake(pugi::xml_node& config) override;
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	bool Load(pugi::xml_node&);

	bool ButtonEvent(const UIButtonType type) override;
	bool SliderEvent(const UIScrollBarType type, const float ratio) override;

	int					coins_count = 0;
	int					lifes = 3;
	bool				delete_coin_text = false;

	bool				esc_menu = false;

public:
	p2String			music;
	FX					pause_open_sfx;
	FX					pause_close_sfx;

	UIImage*			first_life = nullptr;
	UIImage*			second_life = nullptr;
	UIImage*			third_life = nullptr;
	UIImage*			pause = nullptr;
	UIText*				coins_text = nullptr;

};
#endif // !__INLEVEL_H__
