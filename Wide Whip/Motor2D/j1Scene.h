#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
enum class UIButtonType;

enum class Screens
{
	MENU,
	LEVEL1,
	LEVEL2
};

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

	bool ButtonEvent(const UIButtonType type) override;



	// Functions to change across levels
	void StartThisLevel();

	bool IsLevel1Loaded() const;

	bool IsLevel2Loaded() const;

	void ChangeBetweenLevel();

	void ChangeLevelTo(const p2String level);

	p2String GetMapNameLoaded() const;

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

	p2String			level1;
	p2String			level2;

	p2String			music;

	p2String			map_name_loaded;

	float				time_in_fade;

	SDL_Texture* debug_tex;

};

#endif // __j1SCENE_H__