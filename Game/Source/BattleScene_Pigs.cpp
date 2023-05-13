#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "BattleScene_Pigs.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "BattleManager.h"

#include "Defs.h"
#include "Log.h"

BattleScene_Pigs::BattleScene_Pigs(bool isActive) : Module(isActive) {

	name.Create("battleScene_Pigs");
	}

// Destructor
BattleScene_Pigs::~BattleScene_Pigs()
{}

// Called before render is available
bool BattleScene_Pigs::Awake(pugi::xml_node& config) {

	LOG("Loading Scene");
	bool ret = true;

	

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();

	//Add allies from teamManager
	int i = 0;
	/*for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		app->battleManager->AddCharacter(teamItem->data, 0, i, false);
		i++;
	}*/
	
	/*if (config.child("timmy")) {
		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
		timmy->parameters = config.child("timmy");
		timmy->stats = config.parent().child("timmy");
		app->battleManager->AddCharacter(timmy, timmy->parameters.attribute("x").as_int() / 120, timmy->parameters.attribute("y").as_int() / 120, false);
	}*/

	//Add enemies from config
	/*if (config.child("enemy_angryVillager")) {
		villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
		villager->parameters = config.child("enemy_angryVillager");
		villager->stats = config.parent().child("enemy_angryVillager");
		app->battleManager->AddCharacter(villager, villager->parameters.attribute("x").as_int()/120, villager->parameters.attribute("y").as_int() / 120, true);
	}*/

	app->entityManager->Awake(config);
	app->battleManager->Enable();
	
	return ret;
}


// Called before the first frame
bool BattleScene_Pigs::Start() {

	bool retLoad = app->map->Load(mapName, mapFolder);
	timmyPrevPosBool = false;
	bunnyPrevPosBool = false;
	villagerPrevPosBool = false;

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	app->battleManager->MakeCombatMap();
	
	//timmyPrevPos = timmy->position;
	//bunnyPrevPos = bunny->position;
	//villagerPrevPos = villager->position;

	return true;
}


// Called each loop iteration
bool BattleScene_Pigs::PreUpdate() {

	//Buttons :)

	

	bool ret = true;
	return true;
}


// Called each loop iteration
bool BattleScene_Pigs::Update(float dt) {

	

	app->map->Draw();
	return true;
}

void BattleScene_Pigs::RunTree()
{
	//Esto tiene que ser false por defecto para que se ejecute solo la parte del arbol que es la que
	//utilizaremos debido a que al final la IA del EnemyVillager es diferente
	//villager->isEnemyTooClose->SetCondition(false);

	//Miramos si tiene que atacar o tiene que moverse
	//villager->inRangeChecker->SetCondition(conditionToRangeChecker); //true=atacar, false=mover

	//Si se tiene que mover:
	if (!conditionToRangeChecker)
	{
		//Chequeamos que tiene suficente stamina
		if (!noStaminaToMove)
		{
			//si no tiene no se mueve
			return;
		}
		{
			//si tiene se mueve
			app->battleManager->battleState = BattleState::INACTION;
		}
	}
	
	//ejecutamos el arbol
	//villager->behaviorTree->Run();
}

// Called each loop iteration
bool BattleScene_Pigs::PostUpdate() {

	bool ret = true;

	
	return ret;
}


void BattleScene_Pigs::UpdateAnimation(const char* name)
{
	//Movimiento y Idle
	MoveAnimation(name);

}

bool BattleScene_Pigs::CleanUp(){

	LOG("Freeing sceneBattle");

	app->map->CleanUp();
	app->entityManager->CleanUp(); 


	SaveResult();

	return true;
}

void BattleScene_Pigs::MoveAnimation(const char* name)
{
	//if (strcmp(name,"enemy_angryVillager") == 0)
	//{
	//	//Moverse a la derecha
	//	if (villager->position.x > villagerPrevPos.x)
	//	{
	//		villager->currentAnimation = &villager->walkRightAnim;
	//	}
	//	//Moverse a la izquierda
	//	else if (villager->position.x < villagerPrevPos.x)
	//	{
	//		villager->currentAnimation = &villager->walkLeftAnim;
	//	}
	//	//Moverse a abajo
	//	else if (villager->position.y > villagerPrevPos.y)
	//	{
	//		villager->currentAnimation = &villager->walkDownAnim;
	//	}
	//	//Moverse a arriba
	//	else if (villager->position.y < villagerPrevPos.y)
	//	{
	//		villager->currentAnimation = &villager->walkUpAnim;
	//	}
	//	else if (villager->position.y == villagerPrevPos.y && villager->position.x == villagerPrevPos.x)
	//	{

	//		if (frames > 60)
	//		{
	//			villager->currentAnimation = &villager->idleAnim;
	//			frames = 0;
	//		}
	//		else
	//		{
	//			frames++;
	//		}
	//	}

	//	villagerPrevPos.x = villager->position.x;
	//	villagerPrevPos.y = villager->position.y;
	//}
	//else if (strcmp(name, "timmy") == 0)
	//{

	//	//Moverse a la derecha
	//	if (timmy->position.x > timmyPrevPos.x)
	//	{
	//		timmy->currentAnimation = &timmy->walkRightAnim;
	//	}
	//	//Moverse a la izquierda
	//	else if (timmy->position.x < timmyPrevPos.x)
	//	{
	//		timmy->currentAnimation = &timmy->walkLeftAnim;
	//	}
	//	//Moverse a abajo
	//	else if (timmy->position.y > timmyPrevPos.y)
	//	{
	//		timmy->currentAnimation = &timmy->walkDownAnim;
	//	}
	//	//Moverse a arriba
	//	else if (timmy->position.y < timmyPrevPos.y)
	//	{
	//		timmy->currentAnimation = &timmy->walkUpAnim;
	//	}
	//	else if (timmy->position.y == timmyPrevPos.y && timmy->position.x == timmyPrevPos.x)
	//	{
	//		if (frames > 60)
	//		{
	//			timmy->currentAnimation = &timmy->idleAnim;
	//			frames = 0;
	//		}
	//		else
	//		{
	//			frames++;
	//		}
	//		
	//	}

	//	timmyPrevPos.x = timmy->position.x;
	//	timmyPrevPos.y = timmy->position.y;

	//}
	//else if (strcmp(name, "bunny") == 0)
	//{

	//	//Moverse a la derecha
	//	if (bunny->position.x > bunnyPrevPos.x)
	//	{
	//		bunny->currentAnimation = &bunny->walkRightAnim;
	//	}
	//	//Moverse a la izquierda
	//	else if (bunny->position.x < bunnyPrevPos.x)
	//	{
	//		bunny->currentAnimation = &bunny->walkLeftAnim;
	//	}
	//	//Moverse a abajo
	//	else if (bunny->position.y > bunnyPrevPos.y)
	//	{
	//		bunny->currentAnimation = &bunny->walkDownAnim;
	//	}
	//	//Moverse a arriba
	//	else if (bunny->position.y < bunnyPrevPos.y)
	//	{
	//		bunny->currentAnimation = &bunny->walkUpAnim;
	//	}
	//	else if (bunny->position.y == bunnyPrevPos.y && bunny->position.x == bunnyPrevPos.x)
	//	{
	//		if (frames > 60)
	//		{
	//			bunny->currentAnimation = &bunny->idleAnim;
	//			frames = 0;
	//		}
	//		else
	//		{
	//			frames++;
	//		}

	//		
	//	}
	//	

	//	bunnyPrevPos.x = bunny->position.x;
	//	bunnyPrevPos.y = bunny->position.y;

	//}
}

void BattleScene_Pigs::TakeDamageAnimation(const char* name)
{
	//Animación de ser dañado
	/*if (strcmp(name, "enemy_angryVillager") == 0)
	{
		villager->currentAnimation = &villager->takedmgAnim;
	}
	else if (strcmp(name, "timmy") == 0)
	{
		timmy->currentAnimation = &timmy->takedmgAnim;
	}
	else if (strcmp(name, "bunny") == 0)
	{
		bunny->currentAnimation = &bunny->takedmgAnim;
	}*/

}

void BattleScene_Pigs::SaveResult()
{

	if (app->battleManager->win)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isPigDefeated", "true");
	}
	else if (app->battleManager->lose)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isPigDefeated", "false");
	}

}