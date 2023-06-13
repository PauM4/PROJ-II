#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "BattleScene_Wolf.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "BattleManager.h"

#include "Defs.h"
#include "Log.h"

BattleScene_Wolf::BattleScene_Wolf(bool isActive) : Module(isActive) {

	name.Create("battleScene_Wolf");
	}

// Destructor
BattleScene_Wolf::~BattleScene_Wolf()
{}

// Called before render is available
bool BattleScene_Wolf::Awake(pugi::xml_node& config) {

	LOG("Loading Scene");
	bool ret = true;

	

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//Add allies from teamManager
	int i = 0;
	
	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		teamItem->data->health = teamItem->data->maxHealth;
			teamItem->data->isAlive = true;
			app->battleManager->AddCharacter(teamItem->data, 4, 3 + i, false);
			app->entityManager->AddEntity(teamItem->data);
		i++;
	}
	
	if (config.child("enemy_wolf")) {
		wolf = (Enemy_Wolf*)app->entityManager->CreateEntity(EntityType::WOLF);
		wolf->parameters = config.child("enemy_wolf");
		wolf->stats = config.parent().child("wolf");
		app->battleManager->AddCharacter(wolf, wolf->parameters.attribute("x").as_int() / 120, wolf->parameters.attribute("y").as_int() / 120, true);
	}

	if (config.child("enemy_finalwolf")) {
		finalwolf = (Enemy_FinalWolf*)app->entityManager->CreateEntity(EntityType::FINALWOLF);
		finalwolf->parameters = config.child("enemy_finalwolf");
		finalwolf->stats = config.parent().child("finalwolf");
		
	}

	app->entityManager->Awake(config);
	app->battleManager->Enable();
	
	return ret;
}


// Called before the first frame
bool BattleScene_Wolf::Start() {

	bool retLoad = app->map->Load(mapName, mapFolder);

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	finalstate = true;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;	
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);



	app->audio->PlayMusic("Assets/Sounds/Music/music_battle_4.ogg", 0.1f);

	app->battleManager->MakeCombatMap();
	

		
	wolfPrevPos = wolf->position;
	finalwolfPrevPos = finalwolf->position;

	return true;
}


// Called each loop iteration
bool BattleScene_Wolf::PreUpdate() {

	//Buttons :)

	

	bool ret = true;
	return true;
}


// Called each loop iteration
bool BattleScene_Wolf::Update(float dt) {
	int i = 0;
	app->map->Draw();
	if (wolf->isAlive == false && finalstate == true) {
		app->battleManager->finalboss = true;
		app->battleManager->battleState = BattleState::THINKING;
	}
	if (app->battleManager->finalboss == true && finalstate== true) {
		app->battleManager->enemies.Add(finalwolf);
		finalwolf->position = iPoint(960, 360);
	/*	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
			teamItem->data->health = teamItem->data->maxHealth;
			teamItem->data->isAlive = true;
			app->battleManager->AddCharacter(teamItem->data, 4, 3 + i, false);
			i++;
		}*/
		app->battleManager->MakeTurnList();
		app->battleManager->currentTurn = app->battleManager->turnList.start->data;
		app->battleManager->origin = app->battleManager->currentTurn->tilePos;
		app->battleManager->finish = false;
		finalstate = false;
		app->battleManager->finalboss = false;
	}

	return true;
}

void BattleScene_Wolf::RunTree()
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
bool BattleScene_Wolf::PostUpdate() {

	bool ret = true;

	
	return ret;
}


void BattleScene_Wolf::UpdateAnimation(const char* name)
{
	//Movimiento y Idle
	MoveAnimation(name);

}

bool BattleScene_Wolf::CleanUp(){

	LOG("Freeing sceneBattle");

	app->pathfinding->ClearLastPath();
	app->map->CleanUp();
	app->entityManager->CleanUp();
	app->battleManager->Disable();


	SaveResult();

	return true;
}

void BattleScene_Wolf::MoveAnimation(const char* name)
{
	if (strcmp(name, "enemy_wolf") == 0)
	{
		if ((app->battleManager->actionType == ActionType::ATTACK || app->battleManager->actionType == ActionType::ABILITY) && app->battleManager->battleState == BattleState::INACTION)
		{
			if (wolf->name == app->battleManager->currentTurn->name)
			{
				wolf->currentAnimation = &wolf->abilityAnim;


			}
		}
		//Moverse a la derecha
		else if (wolf->position.x > wolfPrevPos.x)
		{
			wolf->currentAnimation = &wolf->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (wolf->position.x < wolfPrevPos.x)
		{
			wolf->currentAnimation = &wolf->walkLeftAnim;
		}
		//Moverse a abajo
		else if (wolf->position.y > wolfPrevPos.y)
		{
			wolf->currentAnimation = &wolf->walkDownAnim;
		}
		//Moverse a arriba
		else if (wolf->position.y < wolfPrevPos.y)
		{
			wolf->currentAnimation = &wolf->walkUpAnim;
		}
		else if (wolf->position.y == wolfPrevPos.y && wolf->position.x == wolfPrevPos.x)
		{

			if (frames > 60)
			{
				wolf->currentAnimation = &wolf->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		wolfPrevPos.x = wolf->position.x;
		wolfPrevPos.y = wolf->position.y;
	}

	if (strcmp(name, "enemy_finalwolf") == 0)
	{
		if ((app->battleManager->actionType == ActionType::ATTACK || app->battleManager->actionType == ActionType::ABILITY) && app->battleManager->battleState == BattleState::INACTION)
		{
			if (finalwolf->name == app->battleManager->currentTurn->name)
			{
				finalwolf->currentAnimation = &finalwolf->abilityAnim;


			}
		}
		//Moverse a la derecha
		else if (finalwolf->position.x > finalwolfPrevPos.x)
		{
			finalwolf->currentAnimation = &finalwolf->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (finalwolf->position.x < finalwolfPrevPos.x)
		{
			finalwolf->currentAnimation = &finalwolf->walkLeftAnim;
		}
		//Moverse a abajo
		else if (finalwolf->position.y > finalwolfPrevPos.y)
		{
			finalwolf->currentAnimation = &finalwolf->walkDownAnim;
		}
		//Moverse a arriba
		else if (finalwolf->position.y < finalwolfPrevPos.y)
		{
			finalwolf->currentAnimation = &finalwolf->walkUpAnim;
		}
		else if (finalwolf->position.y == finalwolfPrevPos.y && finalwolf->position.x == finalwolfPrevPos.x)
		{

			if (frames > 60)
			{
				finalwolf->currentAnimation = &finalwolf->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		finalwolfPrevPos.x = finalwolf->position.x;
		finalwolfPrevPos.y = finalwolf->position.y;
	}
}

void BattleScene_Wolf::TakeDamageAnimation(const char* name)
{
	//Animación de ser dañado

	if (strcmp(name, "enemy_wolf") == 0)
	{
		wolf->currentAnimation = &wolf->takedmgAnim;
	}

	if (strcmp(name, "enemy_wolf") == 0)
	{
		wolf->currentAnimation = &wolf->takedmgAnim;
	}


}

void BattleScene_Wolf::SaveResult()
{

	if (app->battleManager->win)
	{

	}
	else if (app->battleManager->lose)
	{
	
	}

}