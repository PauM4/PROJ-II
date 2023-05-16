#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneCombatLHHR.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "Timmy.h"

#include "Defs.h"
#include "Log.h"

SceneCombatLHHR::SceneCombatLHHR(bool isActive) : Module(isActive)
{
	name.Create("scenecombatLHHR");
}

// Destructor
SceneCombatLHHR::~SceneCombatLHHR()
{}

// Called before render is available
bool SceneCombatLHHR::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	int i = 0;
	for (ListItem<Entity*>* teamItem = app->teamManager->team.start; teamItem != NULL; teamItem = teamItem->next) {
		app->battleManager->AddCharacter(teamItem->data, 4 , 3+i, false);
		app->entityManager->AddEntity(teamItem->data);
		i++;
	}


	if (config.child("enemy_lrrh")) {
		redhoodie = (Enemy_LRRH*)app->entityManager->CreateEntity(EntityType::ENEMYLRRH);
		redhoodie->parameters = config.child("enemy_lrrh");
		redhoodie->stats = config.parent().child("enemy_lrrh");
		app->battleManager->AddCharacter(redhoodie, redhoodie->parameters.attribute("x").as_int() / 120, redhoodie->parameters.attribute("y").as_int() / 120, true);
	}

	if (config.child("sprout")) {
		sprout = (Enemy_CorruptedSprout*)app->entityManager->CreateEntity(EntityType::CORRUPTEDSPROUT);
		sprout->parameters = config.child("sprout");
		sprout->stats = config.parent().child("sprout");
		app->battleManager->AddCharacter(sprout, sprout->parameters.attribute("x").as_int() / 120, sprout->parameters.attribute("y").as_int() / 120, true);
	}


	app->entityManager->Awake(config);
	app->battleManager->Enable();

	return ret;
}


// Called before the first frame
bool SceneCombatLHHR::Start()
{
	//Load map
	bool retLoad = app->map->Load(mapName, mapFolder);


	//Load combat map




	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}





	//// Tell to UIModule which currentMenuType
	//app->uiModule->currentMenuType = COMBAT;
	//// Call this function only when buttons change
	//app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);



	app->audio->PlayMusic("Assets/Sounds/Music/music_battle.ogg", 0.1f);
	app->battleManager->MakeCombatMap();

	sproutPrevPos = sprout->position;
	redhoodiePrevPos = redhoodie->position;
	return true;
}


// Called each loop iteration
bool SceneCombatLHHR::PreUpdate()
{
	bool ret = true;
	return true;
}

// Called each loop iteration
bool SceneCombatLHHR::Update(float dt)
{
	app->map->Draw();

	

	return true;
}

// Called each loop iteration
bool SceneCombatLHHR::PostUpdate()
{
	bool ret = true;




	
	return ret;
}

bool SceneCombatLHHR::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	default:
		break;
	}
	return true;
}


//Called before quitting
bool SceneCombatLHHR::CleanUp()
{
	LOG("Freeing SceneCombatLHHR");


	app->map->CleanUp();
	app->entityManager->CleanUp();
	app->battleManager->CleanUp();



	SaveResult();

	return true;
}

void SceneCombatLHHR::UpdateAnimation(const char* name)
{
	//Movimiento y Idle
	MoveAnimation(name);

}

void SceneCombatLHHR::MoveAnimation(const char* name)
{
	if (strcmp(name, "sprout") == 0)
	{
		//Moverse a la derecha
		if (sprout->position.x > sproutPrevPos.x)
		{
			sprout->currentAnimation = &sprout->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (sprout->position.x < sproutPrevPos.x)
		{
			sprout->currentAnimation = &sprout->walkLeftAnim;
		}
		//Moverse a abajo
		else if (sprout->position.y > sproutPrevPos.y)
		{
			sprout->currentAnimation = &sprout->walkDownAnim;
		}
		//Moverse a arriba
		else if (sprout->position.y < sproutPrevPos.y)
		{
			sprout->currentAnimation = &sprout->walkUpAnim;
		}
		else if (sprout->position.y == sproutPrevPos.y && sprout->position.x == sproutPrevPos.x)
		{

			if (frames > 60)
			{
				sprout->currentAnimation = &sprout->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		sproutPrevPos.x = sprout->position.x;
		sproutPrevPos.y = sprout->position.y;
	}

	if (strcmp(name, "enemy_lrrh") == 0)
	{

		if ((app->battleManager->actionType == ActionType::ATTACK || app->battleManager->actionType == ActionType::ABILITY) && app->battleManager->battleState == BattleState::INACTION)
		{
			if (redhoodie->name == app->battleManager->currentTurn->name)
			{
				redhoodie->currentAnimation = &redhoodie->abilityAnim;
				redhoodie->abilityAnimation = &redhoodie->arrow;

				redhoodie->arrowPos.x = app->battleManager->targetPosForAIAnimation.x;
				redhoodie->arrowPos.y = app->battleManager->targetPosForAIAnimation.y;

			}


		}
		//Moverse a la derecha
		else if (redhoodie->position.x > redhoodiePrevPos.x)
		{
			redhoodie->currentAnimation = &redhoodie->walkRightAnim;
		}
		//Moverse a la izquierda
		else if (redhoodie->position.x < redhoodiePrevPos.x)
		{
			redhoodie->currentAnimation = &redhoodie->walkLeftAnim;
		}
		//Moverse a abajo
		else if (redhoodie->position.y > redhoodiePrevPos.y)
		{
			redhoodie->currentAnimation = &redhoodie->walkDownAnim;
		}
		//Moverse a arriba
		else if (redhoodie->position.y < redhoodiePrevPos.y)
		{
			redhoodie->currentAnimation = &redhoodie->walkUpAnim;
		}
		else if (redhoodie->position.y == redhoodiePrevPos.y && redhoodie->position.x == redhoodiePrevPos.x)
		{

			if (frames > 60)
			{
				redhoodie->currentAnimation = &redhoodie->idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}
		}

		redhoodiePrevPos.x = redhoodie->position.x;
		redhoodiePrevPos.y = redhoodie->position.y;
	}

}

void SceneCombatLHHR::TakeDamageAnimation(const char* name)
{
	//Animaci�n de ser da�ado
	if (strcmp(name, "enemy_lrrh") == 0)
	{
	
		redhoodie->currentAnimation = &redhoodie->takedmgAnim;
	}
	if (strcmp(name, "sprout") == 0)
	{

		sprout->currentAnimation = &sprout->takedmgAnim;
	}

}

void SceneCombatLHHR::SaveResult()
{

	if (app->battleManager->win)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isLRRHDefeated", "true");
		app->teamManager->islrrhplayable = true;
		app->teamManager->bow.ininventory = true;
		app->teamManager->bow.character = 3;
		app->teamManager->UpdateParty();
	}
	else if (app->battleManager->lose)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isLRRHDefeated", "false");
	}
}
