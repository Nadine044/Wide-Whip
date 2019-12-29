#ifndef __INLEVEL_H__
#define __INLEVEL_H__

#include "j1Module.h"
#include "j1Audio.h"

class UIImage;
class UIText;
class j1Timer;

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

	bool Save(pugi::xml_node&) const  override;

	bool Load(pugi::xml_node&) override;

	bool ButtonEvent(const UIButtonType type) override;
	bool SliderEvent(const UIScrollBarType type, const float ratio) override;


public:
	p2String			music;
	FX					pause_open_sfx;
	FX					pause_close_sfx;

	UIImage*			first_life = nullptr;
	UIImage*			second_life = nullptr;
	UIImage*			third_life = nullptr;
	UIImage*			pause = nullptr;
	UIText*				coins_text = nullptr;
	UIText*				counter_text = nullptr;
	UIText*				player_name_text = nullptr;


	j1Timer timer_counter;
	int					coins_count = 0;
	int					counter_secs = 0;
	int					lifes = 3;
	p2String player_name;
	bool				esc_menu = false;
};
#endif // !__INLEVEL_H__
