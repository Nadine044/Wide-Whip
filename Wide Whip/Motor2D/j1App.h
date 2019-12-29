#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Timer.h"
#include "j1PerfTimer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class ModuleCollision;
class ModuleFadeToBlack;
class ModuleEntityManager;
class j1PathFinding;
class MFonts;
class MGui;
class UIMenu;
class InLevel;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	const pugi::xml_node GetConfig() const;
	const float GetDT();

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2String>& list_to_fill) const;
	void PauseResumeGame();
	const bool IsPaused() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;
	ModuleCollision*	collisions;
	ModuleFadeToBlack*	fade_to_black;
	ModuleEntityManager* module_entity_manager;
	j1PathFinding*		pathfinding_module;
	MFonts*				font;
	MGui*				gui;
	UIMenu*				menu;
	InLevel*			in_level;

private:

	p2List<j1Module*>	modules;

	uint				frames;
	int					argc;
	char**				args;

	p2String			title;
	p2String			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	bool				is_paused = false;

	mutable p2String	save_game_root;

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	j1PerfTimer			ptimer;

	uint				frame_count = 0u;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint				last_sec_frame_count = 0u;
	uint				prev_last_sec_frame_count = 0u;
	uint16_t			framerate_cap = 0u;
	float				avg_fps = 0.0f;
	uint				frames_on_last_update = 0u;
	float				dt = 0.f;

	bool cap = true;

	bool paused = false;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif