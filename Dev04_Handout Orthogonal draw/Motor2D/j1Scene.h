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
	bool Awake(pugi::xml_node& config) override;

	// Called before the first frame
	bool Start()override;

	// Called before all Updates
	bool PreUpdate()override;

	// Called each loop iteration
	bool Update(float dt)override;

	void CheckSaveLoad();

	// Called before all Updates
	bool PostUpdate()override;

	// Called before quitting
	bool CleanUp()override;

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

	p2SString level1;
	p2SString level2;

	float time_in_fade;

};

#endif // __j1SCENE_H__