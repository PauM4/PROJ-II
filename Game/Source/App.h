#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PerformanceBar2.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;

//-----SCENES-----
class SceneIntro;
class SceneMainMenu;

//World_01
class Scene;
class SceneCombatLHHR;
class SceneBattle;
class SceneGrandma;

//World_02
class W2_Scene;
class W2_Scene_Maze;

//World_03
class W3_Scene;

//----------
class SceneFoxQuest; 
class BattleScene_Pigs;
class EntityManager;
class Map;
class Fonts;
class FadeToBlack;
class SceneManager;
class BattleManager;
//L07 DONE 2: Add Physics module
class Physics;
class PathFinding;
class GuiManager;
class UIModule;
class TeamManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest();
	bool LoadFromFile();
	bool SaveToFile();


	bool UpdateXMLAttributeFromNode(const char* file_name, const char* node_name, const char* attribute_name, const char* value);

private:

	// Load config file
	bool LoadConfig();

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

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;

	//-----SCENES-----
	SceneIntro* sceneIntro;
	SceneMainMenu* sceneMainMenu;

	//World_01
	Scene* scene;
	SceneCombatLHHR* sceneCombatLHHR;
	SceneBattle* sceneBattle;
	BattleScene_Pigs* battleScene_Pigs;
	SceneGrandma* sceneGrandma;

	//World_02
	W2_Scene* w2_scene;
	W2_Scene_Maze* w2_scene_maze;

	//World_03
	W3_Scene* w3_scene;

	//----------
	SceneFoxQuest* sceneFoxQuest; 
	EntityManager* entityManager;
	Map* map;
	Fonts* fonts; 
	FadeToBlack* fadeToBlack;
	SceneManager* sceneManager; 
	BattleManager* battleManager;

	Physics* physics;
	PathFinding* pathfinding;
	GuiManager* guiManager;
	UIModule* uiModule;
	TeamManager* teamManager;

	double prepareUpdate;
	double finishUpdate;
	double preUpdate;
	double doUpdate;
	double postUpdate;
	double update;
	PerformanceBar perfBar; 
	PerfTimer barTimer;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

    bool saveGameRequested;
	bool loadGameRequested;
	
	Timer timer;
	PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;

	uint32 maxFrameDuration = 0;

};

extern App* app;

#endif	// __APP_H__