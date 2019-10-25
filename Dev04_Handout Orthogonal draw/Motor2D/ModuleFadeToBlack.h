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
	bool FadeToBlack(float time = 1.0f);
	bool IsFading() const;


public: 
	bool to_load = false;
private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	Uint32 fase_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__