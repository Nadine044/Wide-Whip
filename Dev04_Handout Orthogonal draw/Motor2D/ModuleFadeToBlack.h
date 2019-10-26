#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class ModuleFadeToBlack : public j1Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool FadeToBlack(bool& active, float time = 1.0f);
	bool IsFading() const;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		changing_mode,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	Uint32 fase_time = 0;
	SDL_Rect screen;
	bool* to_active = nullptr;

	float normalized = 0.0f;
};

#endif //__MODULEFADETOBLACK_H__