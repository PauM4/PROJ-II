#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

//-----SCENES-----
#include "SceneIntro.h"
#include "SceneMainMenu.h"

//World_01
#include "Scene.h"
#include "SceneCombatLHHR.h"
#include "SceneBattle.h"

//World_02
#include "W2_Scene.h"

//World_03
#include "W3_Scene.h"

//-------------------
#include "SceneFoxQuest.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "SceneManager.h"
#include "BattleManager.h"
#include "FadeToBlack.h"
#include "UIModule.h"
#include "TeamManager.h"
#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{

	frames = 0;

	input = new Input(true);
	win = new Window(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	//L07 DONE 2: Add Physics module
	fadeToBlack = new FadeToBlack(true);
	physics = new Physics(true);
	pathfinding = new PathFinding(true);
	sceneManager = new SceneManager(true); 
	
	//-----SCENES-----
	sceneMainMenu = new SceneMainMenu(false);
	sceneIntro = new SceneIntro(false);

	//World_01
	scene = new Scene(false);
	sceneCombatLHHR = new SceneCombatLHHR(false);
	sceneBattle = new SceneBattle(false);
	sceneGrandma = new SceneGrandma(false);
	sceneFoxQuest = new SceneFoxQuest(false); 

	//World_02
	w2_scene = new W2_Scene(false);

	//World_03
	w3_scene = new W3_Scene(false);

	//-----------------
	battleManager = new BattleManager(false);
	entityManager = new EntityManager(true);
	map = new Map(true);
	guiManager = new GuiManager(true);
	fonts = new Fonts(true);
	uiModule = new UIModule(true);
	teamManager = new TeamManager(true);
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	//L07 DONE 2: Add Physics module
	AddModule(physics);
	AddModule(fonts);
	AddModule(pathfinding);
	AddModule(sceneManager);

	//-----SCENES-----
	AddModule(sceneIntro);
	AddModule(sceneMainMenu);

	//World_01
	AddModule(scene);
	//AddModule(sceneCombatLHHR);
	AddModule(sceneBattle);
	AddModule(sceneGrandma);
	AddModule(sceneFoxQuest); 

	//World_02
	AddModule(w2_scene);

	//World_03
	AddModule(w3_scene);

	//-----------------
	AddModule(battleManager);
	AddModule(entityManager);
	AddModule(map);
	AddModule(guiManager);
	AddModule(uiModule);
	AddModule(teamManager);
	AddModule(fadeToBlack);

	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}


// Called before render is available
bool App::Awake()
{
	timer = Timer();

	bool ret = false;

	ret = LoadConfig();

	if (ret == true)
	{
		title = configNode.child("app").child("title").child_value(); 
		
		maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			if (item->data->active == true) {
				pugi::xml_node node = configNode.child(item->data->name.GetString());
				ret = item->data->Awake(node);
				std::cout << "Awake: " << item->data->name.GetString() << " " << ret << std::endl;
			}
			item = item->next;
		}
	}

	LOG("---------------- Time Awake: %f/n",timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	timer.Start();
	startupTime.Start();
	lastSecFrameTime.Start();


	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true) {
			ret = item->data->Start();
			std::cout << "Start: " << item->data->name.GetString() << " " << ret << std::endl;
		}
		item = item->next;
	}

	LOG("----------------- Time Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	barTimer.Start();
	bool ret = true;
	PrepareUpdate();
	prepareUpdate = barTimer.ReadMs();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();
	preUpdate = barTimer.ReadMs();

	if (ret == true)
		ret = DoUpdate();
	doUpdate = barTimer.ReadMs();

	
	if (ret == true)
		ret = PostUpdate();
	postUpdate = barTimer.ReadMs();

	FinishUpdate();
	finishUpdate = barTimer.ReadMs();
	update = prepareUpdate + preUpdate + doUpdate + postUpdate + finishUpdate;
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}
	
	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadFromFile();
	if (saveGameRequested == true) SaveToFile();

	// Amount of frames since startup
	frameCount++;
	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	// Amount of ms took the last update
	dt = frameTime.ReadMSec();
	// Amount of frames during the last second
	lastSecFrameCount++;

	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	float delay = float(maxFrameDuration) - dt;

	PerfTimer delayTimer = PerfTimer();
	delayTimer.Start();
	if (maxFrameDuration > 0 && delay > 0) {
		SDL_Delay(delay);
		//LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
		dt = maxFrameDuration;
	}
	else {
		//LOG("No wait");
	}

	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt/1000);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() 
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveToFile() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (!result) {
		std::cerr << "Error cargando el archivo: " << result.description() << std::endl;
		
	}
	else
	{
		pugi::xml_node saveNodeAuxiliar = gameStateFile.child("save_state");
		if (!saveNodeAuxiliar) {
			std::cerr << "No se encontr� el nodo 'save_state' en el archivo." << std::endl;
			
		}
		else
		{
			pugi::xml_node battleInfoNodeAuxiliar = saveNodeAuxiliar.child("BattleInfo");
			if (!battleInfoNodeAuxiliar) {
				std::cerr << "No se encontr� el nodo 'BattleInfo' en el archivo." << std::endl;

				//Si no se encuentra el nodo BattleInfo (primera vez que se crea el save_game), lo creamos.
				pugi::xml_node battleInfoNode = saveStateNode.append_child("BattleInfo");
				battleInfoNode.append_attribute("isAngryVillagerDefeated");
				battleInfoNode.append_attribute("isLRRHDefeated");
				battleInfoNode.append_attribute("isPigDefeated");
				battleInfoNode.append_attribute("isPereDefeated");
				battleInfoNode.append_attribute("isWolfDefeated");
				
			}
			else
			{
				//De ya estar creado: Cogemos la informaci�n del ya creado previamente y la dejamos tal cual:

				pugi::xml_node battleInfoNodeToReplace = saveStateNode.append_child("BattleInfo");
				battleInfoNodeToReplace.append_attribute("isAngryVillagerDefeated") = battleInfoNodeAuxiliar.attribute("isAngryVillagerDefeated").value();
				battleInfoNodeToReplace.append_attribute("isLRRHDefeated") = battleInfoNodeAuxiliar.attribute("isLRRHDefeated").value();
				battleInfoNodeToReplace.append_attribute("isPigDefeated") = battleInfoNodeAuxiliar.attribute("isPigDefeated").value();
				battleInfoNodeToReplace.append_attribute("isPereDefeated") = battleInfoNodeAuxiliar.attribute("isPereDefeated").value();
				battleInfoNodeToReplace.append_attribute("isWolfDefeated") = battleInfoNodeAuxiliar.attribute("isWolfDefeated").value();

			}

		}
	}


	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}


bool App::UpdateXMLAttributeFromNode(const char* file_name, const char* node_name, const char* attribute_name, const char* value)
{
	pugi::xml_document doc;
	if (!doc.load_file(file_name)) return false;

	pugi::xml_node padre;

	if (file_name == "config.xml")
	{
		padre = doc.child("config");
	}
	else if (file_name == "save_game.xml")
	{
		padre = doc.child("save_state");
	}


	for (pugi::xml_node hijo : padre.children(node_name))
	{

		for (pugi::xml_attribute attr : hijo.attributes())
		{
			std::cout << " " << attr.name() << "=" << attr.value();

			if (std::strcmp(attr.name(), attribute_name) == 0) {
				attr.set_value(value);

				std::cout << "Attribute: " << attribute_name << " modified with value :" << value << std::endl;
			}

		}

	}
	if (!doc.save_file(file_name)) {
		std::cout << "Could not save XML file" << std::endl;
		return false;
	}

	return true;

}