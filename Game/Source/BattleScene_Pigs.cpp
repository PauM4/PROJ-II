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

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//Add allies from teamManager
	int i = 0;
	
	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		app->battleManager->AddCharacter(teamItem->data, 4, 3 + i, false);
		app->entityManager->AddEntity(teamItem->data);
		i++;
	}
	
	if (config.child("enemy_littlePig")) {
		littlePig = (Enemy_LittlePig*)app->entityManager->CreateEntity(EntityType::ENEMYLPIG);
		littlePig->parameters = config.child("enemy_littlePig");
		littlePig->stats = config.parent().child("enemy_littlePig");
		app->battleManager->AddCharacter(littlePig, littlePig->parameters.attribute("x").as_int() / 120, littlePig->parameters.attribute("y").as_int() / 120, true);
	}

	if (config.child("enemy_middlePig")) {
		middlePig = (Enemy_MiddlePig*)app->entityManager->CreateEntity(EntityType::ENEMYMPIG);
		middlePig->parameters = config.child("enemy_middlePig");
		middlePig->stats = config.parent().child("enemy_middlePig");
		app->battleManager->AddCharacter(middlePig, middlePig->parameters.attribute("x").as_int() / 120, middlePig->parameters.attribute("y").as_int() / 120, true);
	}

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


	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}


	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);



	app->audio->PlayMusic("Assets/Sounds/Music/music_battle_2.wav", 0.1f);

	app->battleManager->MakeCombatMap();
	


	littlePigPrevPos = littlePig->position;
	middlePigPrevPos = middlePig->position;

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

	app->pathfinding->ClearLastPath();
	app->map->CleanUp();
	app->entityManager->CleanUp();
	app->battleManager->Disable();


	SaveResult();

	return true;
}

void BattleScene_Pigs::MoveAnimation(const char* name)
{
	if (strcmp(name, "enemy_littlePig") == 0)
	{
		//Moverse a la derecha
		if (littlePig->position.x > littlePigPrevPos.x)
		{
			littlePig->currentAnimation = &littlePig->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (littlePig->position.x < littlePigPrevPos.x)
		{
			littlePig->currentAnimation = &littlePig->walkLeftAnim;
		}
		//Moverse a abajo
		else if (littlePig->position.y > littlePigPrevPos.y)
		{
			littlePig->currentAnimation = &littlePig->walkDownAnim;
		}
		//Moverse a arriba
		else if (littlePig->position.y < littlePigPrevPos.y)
		{
			littlePig->currentAnimation = &littlePig->walkUpAnim;
		}
		else if (littlePig->position.y == littlePigPrevPos.y && littlePig->position.x == littlePigPrevPos.x)
		{

			if (frames > 60)
			{
				littlePig->currentAnimation = &littlePig->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		littlePigPrevPos.x = littlePig->position.x;
		littlePigPrevPos.y = littlePig->position.y;
	}

	if (strcmp(name, "enemy_middlePig") == 0)
	{
		//Moverse a la derecha
		if (middlePig->position.x > middlePigPrevPos.x)
		{
			middlePig->currentAnimation = &middlePig->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (middlePig->position.x < middlePigPrevPos.x)
		{
			middlePig->currentAnimation = &middlePig->walkLeftAnim;
		}
		//Moverse a abajo
		else if (middlePig->position.y > middlePigPrevPos.y)
		{
			middlePig->currentAnimation = &middlePig->walkDownAnim;
		}
		//Moverse a arriba
		else if (middlePig->position.y < middlePigPrevPos.y)
		{
			middlePig->currentAnimation = &middlePig->walkUpAnim;
		}
		else if (middlePig->position.y == middlePigPrevPos.y && middlePig->position.x == middlePigPrevPos.x)
		{

			if (frames > 60)
			{
				middlePig->currentAnimation = &middlePig->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		middlePigPrevPos.x = middlePig->position.x;
		middlePigPrevPos.y = middlePig->position.y;
	}
}

void BattleScene_Pigs::TakeDamageAnimation(const char* name)
{
	//Animación de ser dañado
	if (strcmp(name, "enemy_middlePig") == 0)
	{
		middlePig->currentAnimation = &middlePig->takedmgAnim;
	}
	else if (strcmp(name, "enemy_littlePig") == 0)
	{
		littlePig->currentAnimation = &littlePig->takedmgAnim;
	}


}

void BattleScene_Pigs::SaveResult()
{

	if (app->battleManager->win)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isPigDefeated", "true");
		app->UpdateXMLAttributeFromNode("save_game.xml", "playable", "islilpigplayable", "true");
		app->UpdateXMLAttributeFromNode("save_game.xml", "playable", "ismidpigplayable", "true");
		app->teamManager->islilpigplayable = true;
		app->teamManager->ismidpigplayable = true;
	}
	else if (app->battleManager->lose)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isPigDefeated", "false");
	}

}