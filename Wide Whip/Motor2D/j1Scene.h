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

	bool Save(pugi::xml_node&) const  override;

	bool Load(pugi::xml_node&) override;



	// Functions to change across levels
	void StartThisLevel();

	bool IsLevel1Loaded() const;

	bool IsLevel2Loaded() const;

	void ChangeBetweenLevel();

	void ChangeLevelTo(const p2SString level);

	p2SString GetMapNameLoaded() const;

	void ChangeToLevel1Fade();

	void ChangeToLevel2Fade();

	void StartThisLevelFade();

	void ChangeBetweenLevelFade();



	bool* GetChangeToLevel1BoolAdress() { return &change_to_level_1; }

	bool* GetChangeToLevel2BoolAdress() { return &change_to_level_2; }

	bool* GetStartThisLevelBoolAdress() { return &start_this_level; }

	bool* GetChangeBetweenLevelsBoolAdress() { return &change_between_levels; }

private:
	void CheckLevelChange();



private:


	bool				change_to_level_1		= false;
	bool				change_to_level_2		= false;
	bool				change_between_levels	= false;
	bool				start_this_level		= false;

	p2SString			level1;
	p2SString			level2;

	p2SString			music;

	p2SString			map_name_loaded;

	float				time_in_fade;

};

#endif // __j1SCENE_H__