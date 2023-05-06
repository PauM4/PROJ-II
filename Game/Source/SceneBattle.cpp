#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneBattle.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "Timmy.h"
#include "TeamManager.h"
#include "BattleManager.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle(bool isActive) : Module(isActive) {

	name.Create("sceneBattle");
}

// Destructor
SceneBattle::~SceneBattle()
{}

// Called before render is available
bool SceneBattle::Awake(pugi::xml_node& config) {

	LOG("Loading Scene");
	bool ret = true;

	app->teamManager->statsdone = false;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();

	//Add allies from teamManager
	/*int i = 0;
	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		app->battleManager->AddCharacter(teamItem->data, 0, i, false);
		i++;
	}*/
	
	if (config.child("timmy")) {
		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
		timmy->parameters = config.child("timmy");
		timmy->stats = config.parent().child("timmy");
		app->battleManager->AddCharacter(timmy, timmy->parameters.attribute("x").as_int() / 120, timmy->parameters.attribute("y").as_int() / 120, false);
	}

	//Add enemies from config
	if (config.child("enemy_angryVillager")) {
		villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
		villager->parameters = config.child("enemy_angryVillager");
		villager->stats = config.parent().child("enemy_angryVillager");
		app->battleManager->AddCharacter(villager, villager->parameters.attribute("x").as_int()/120, villager->parameters.attribute("y").as_int() / 120, true);
	}

	app->entityManager->Awake(config);
	app->battleManager->Enable();
	
	return ret;
}


// Called before the first frame
bool SceneBattle::Start() {

	bool retLoad = app->map->Load(mapName, mapFolder);


	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	app->battleManager->MakeCombatMap();
	
	

	return true;
}


// Called each loop iteration
bool SceneBattle::PreUpdate() {

	//Buttons :)

	

	bool ret = true;
	return true;
}


// Called each loop iteration
bool SceneBattle::Update(float dt) {

	app->map->Draw();
	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate() {

	bool ret = true;

	


	
	return ret;
}

bool SceneBattle::CleanUp(){

	LOG("Freeing sceneBattle");

	app->map->CleanUp();
	app->entityManager->CleanUp(); 

	return true;
}

void SceneBattle::SaveResult()
{

	if (app->battleManager->win)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isAngryVillagerDefeated", "true");
	}
	else if (app->battleManager->lose)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isAngryVillagerDefeated", "false");
	}

}
