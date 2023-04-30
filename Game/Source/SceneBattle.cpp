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

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();

	//Add allies from teamManager
	/*int i = 0;
	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		app->battleManager->AddCharacter(teamItem->data, 0, i, false);
		i++;
	}*/

	//Add enemies from config
	if (config.child("enemy_angryVillager")) {
		Enemy_AngryVillager* villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
		int x = config.child("enemy_angryVillager").attribute("x").as_int();
		int y = config.child("enemy_angryVillager").attribute("y").as_int();
		app->battleManager->AddCharacter(villager, x, y, true);
	}

	return ret;
}


// Called before the first frame
bool SceneBattle::Start() {

	bool retLoad = app->map->Load(mapName, mapFolder);

	app->battleManager->MakeCombatMap();
	app->battleManager->MakeTurnList();
	
	battleState = BattleState::NORMAL;

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

	Entity* character = app->battleManager->GetCurrentTurn();

	//Check for mouse/controller hovering on action button
	app->battleManager->GetActionArea(character);

	//Check for mouse/controller clicking on action button

	
	//Checks win/lose state
	battleState = app->battleManager->CheckState();
	switch (battleState) {
	case BattleState::WIN:
		//Win sheganigans
		break;
	case BattleState::LOSE:
		//Lose screen plus reset
		break;
	}
}

// Called each loop iteration
bool SceneBattle::PostUpdate() {

	bool ret = true;

	app->map->Draw();

	app->battleManager->DisplayTurnList();
	
	return ret;
}

bool SceneBattle::CleanUp(){

	LOG("Freeing sceneBattle");

	app->map->CleanUp();
	app->entityManager->CleanUp(); 

	return true;
}
