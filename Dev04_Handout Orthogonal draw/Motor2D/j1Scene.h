#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Functions to change across levels
	void StartThisLevel();

	void ChangeBetweenLevel();

	void ChangeLevelTo(const p2SString level);


private:
	void CheckLevelChange();

private:


	bool change_to_level_1			= false;
	bool change_to_level_2			= false;
	bool start_this_level			= false;
	bool change_between_levels		= false;
};

#endif // __j1SCENE_H__