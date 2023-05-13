#include "BattleManager.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiButton.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "UIModule.h"
#include "Fonts.h"
#include "Defs.h"
#include "Physics.h"
#include "Log.h"
#include"Enemy_AngryVillager.h"
#include "SceneBattle.h"
#include "Timer.h"

BattleManager::BattleManager(bool isActive) : Module(isActive) {

	name.Create("battleManager");
}

// Destructor
BattleManager::~BattleManager() {}

bool BattleManager::Awake(pugi::xml_node& config) {


	return true;
}

bool BattleManager::Start() {



	winScreen = app->tex->Load("Assets/UI/Win_screen.png");
	loseScreen = app->tex->Load("Assets/UI/lose_screen.png");

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	MakeTurnList();

	
	currentTurn = turnList.start->data;
	origin = currentTurn->tilePos;
	enemyTimer = 0;
	pathIndex = 1;
	battleState = BattleState::THINKING;
	return true;
}

bool BattleManager::PreUpdate() {

	MousePosition();


	return true;
}

bool BattleManager::Update(float dt) {

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		PauseMenuAppear();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode)
		{
			godMode = false;
		}
		else
		{
			godMode = false;
		}
	}

	if (godMode)
		GodMode();

	UpdateEntitiesTilePos();

	switch (battleState)
	{
	case BattleState::UNKNOWN:
		break;
	case BattleState::THINKING:
		origin = currentTurn->tilePos;
		targets.Clear();
		actionArea.Clear();
		area.Clear();
		if (currentTurn->isEnemy)
		{
			enemyTimer = 200;
			battleState = BattleState::ENEMY;
			
		}
		break;
	case BattleState::SELCETED:

	
		actionArea.Clear();
		targets.Clear();
		GetActionArea(currentTurn, actionType);

		if (!currentTurn->isEnemy) {

			if (actionType == ActionType::MOVE) {

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					for (ListItem<TileData*>* area = actionArea.start; area != NULL; area = area->next) {
						iPoint pos = iPoint(area->data->x, area->data->y);
						if (app->pathfinding->IsWalkable(origin) && mouseTile == pos && combatMap[mouseTile.x][mouseTile.y].character == nullptr) {
							length = app->pathfinding->CreatePath(origin, mouseTile);
							destination.x = mouseTile.x;
							destination.y = mouseTile.y;
							moveanim = true;
							currentTurn->UseStamina(3);
							battleState = BattleState::INACTION;
						}
					}
				}
			}
			else if (actionType == ActionType::END_TURN) {

				currentTurn->GainStamina(10);
				battleState = BattleState::INACTION;

			}
			else {


				SelectTargets();

				for (int i = 0; i < targets.Count(); i++) {


					if (targets.At(i)->data->tilePos == mouseTile) {
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {


							targets.At(i)->data->health = targets.At(i)->data->health - (currentTurn->attack - targets.At(i)->data->defense);
							app->sceneBattle->TakeDamageAnimation(targets.At(i)->data->name.GetString());

							currentTurn->UseStamina(5);
							battleState = BattleState::INACTION;

						}
					}
				}
			}
		}


		break;
	case BattleState::INACTION:

		if (actionType == ActionType::MOVE) {

			if (pathIndex != length) {

				
				Move(pathIndex, length);
			}
			else
			{
				if (currentTurn->tilePos == destination) {


					destination = iPoint(0, 0);
					length = 1;
					pathIndex = 1;
					app->pathfinding->ClearLastPath();
					battleState = BattleState::THINKING;
					UpdateTurnList();
				}
			}

		}
		else {

			battleState = BattleState::THINKING;
			UpdateTurnList();

		}

		break;
	case BattleState::ENEMY:

		////Miramos si tiene que atacar o tiene que moverse
		//app->sceneBattle->conditionToRangeChecker = IaEnemyAttack();

		////Miramos si tiene suficiente stamina para que se mueva
		//app->sceneBattle->noStaminaToMove = IaEnemyMove();

		//if (enemyTimer > 300) {

		//	//Se Ejecuta el arbol
		//	app->sceneBattle->RunTree();
		//	enemyTimer = 0;
		//	battleState = BattleState::INACTION;
		//}
		//else {
		//	enemyTimer++;
		//}

		GetActionArea(currentTurn, ActionType::ATTACK);
		SelectTargets();

		if (currentTurn->stamina >= 5 && battleState == BattleState::ENEMY)
		{
			ListItem<Entity*>* entitylist;
			entitylist = targets.start;

			while (entitylist != NULL)
			{
				if (entitylist->data->isAlive == true)
				{
					entitylist->data->health = entitylist->data->health - (currentTurn->attack - entitylist->data->defense);
					targets.Clear();
					currentTurn->UseStamina(5);
					entitylist = NULL;
					actionType = ActionType::ATTACK;
					battleState = BattleState::INACTION;
					break;
				}

				entitylist = entitylist->next;

			}

		}
		if (currentTurn->stamina >= 3 && battleState == BattleState::ENEMY) {

			targets.Clear();
			actionArea.Clear();

	/*		iPoint pos = iPoint(currentTurn->tilePos.x, currentTurn->tilePos.y+1);
			length = app->path  finding->CreatePath(origin, pos);
			destination.x = pos.x;
			destination.y = pos.y;
			currentTurn->UseStamina(3);
			actionType = ActionType::MOVE;
			battleState = BattleState::INACTION;*/

			GetActionArea(currentTurn, ActionType::MOVE);

			if (battleState == BattleState::ENEMY) {
				ListItem<TileData*>* tiledata;
				for (tiledata = actionArea.start; tiledata != NULL; tiledata = tiledata->next) {

					if (battleState == BattleState::ENEMY && tiledata->data->character==NULL) {
						
						iPoint pos = iPoint(tiledata->data->x, tiledata->data->y);
						CreateArea(currentTurn->AttArea, 1, pos);
						ListItem<TileData*>* tileListItem;

						if (battleState == BattleState::ENEMY) {
							for (tileListItem = area.start; tileListItem != NULL; tileListItem = tileListItem->next) {

								if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy == false) {

									length = app->pathfinding->CreatePath(origin, pos);
									destination.x = pos.x;
									destination.y = pos.y;
									currentTurn->UseStamina(3);
									actionType = ActionType::MOVE;
									battleState = BattleState::INACTION;

								}

							}
						}

					}

				}
			}

				

			
			
		}
	
		if (battleState == BattleState::ENEMY) {
			currentTurn->GainStamina(10);
			battleState = BattleState::INACTION;
		}

		break;

	case BattleState::WIN:
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->sceneManager->LoadScene(GameScene::SCENE);
		break;
	case BattleState::LOSE:
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->sceneManager->LoadScene(GameScene::SCENE);
		break;
	default:
		break;
	}


	if (enemyTimer > 0) {
		enemyTimer--;
	}
	
	CheckWinCondition();

	UpdateCombatMap();


	return true;
}

bool BattleManager::PostUpdate() {

	if (battleState == BattleState::SELCETED || battleState==BattleState::ENEMY ) {

		DisplayAtackArea(actionType);
		DisplayEnemys();

    }

	
	app->sceneBattle->UpdateAnimation(currentTurn->name.GetString());

	UIStatsForBattle();
	DisplayTurnList();

	app->render->DrawRectangle({ int(allies.start->data->position.x) + 35, int(allies.start->data->position.y) + 35, 50, 50 }, 0, 233, 0, 250, true);
	app->render->DrawRectangle({ int(enemies.start->data->position.x) + 35, int(enemies.start->data->position.y) + 35, 50, 50 }, 255, 233, 0, 250, true);
	return true;
}



bool BattleManager::CleanUp() {


	return true;
}

Entity* BattleManager::GetCurrentTurn() { return currentTurn;}



bool BattleManager::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	if (battleState != BattleState::INACTION) {
		switch (control->id)
		{
			// Attack
		case 16:
			if (currentTurn->stamina >= 5) {
				app->battleManager->buttonPressed = CombatButtons::ATTACK;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::ATTACK;
			}
			break;
			// Ability
		case 17:
			if (currentTurn->stamina >= 10) {
				app->battleManager->buttonPressed = CombatButtons::ABILITY;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::ABILITY;
			}
			break;
			// Move
		case 18:
			if (currentTurn->stamina >= 3) {
				app->battleManager->buttonPressed = CombatButtons::MOVE;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::MOVE;
			}
			break;

			// End turn
		case 19:
			app->battleManager->buttonPressed = CombatButtons::END;
			app->battleManager->battleState = BattleState::THINKING;
			app->battleManager->actionType = ActionType::END_TURN;
			app->battleManager->battleState = BattleState::SELCETED;


			break;
		}

		
	}
	return true;

}

void BattleManager::UIStatsForBattle()
{
	int i = 0;
	// UI Stats for Battle
	for (ListItem<Entity*>* allyItem = allies.start; allyItem != NULL; allyItem = allyItem->next) {


		uint stamina = allyItem->data->stamina;
		std::string staminaString = std::to_string(stamina);
		const char* staminaChar = staminaString.c_str();

		uint hp = allyItem->data->health;
		std::string hpString = std::to_string(hp);
		const char* hpChar = hpString.c_str();
	    
		const char* nameChar = allyItem->data->namechar.GetString();
		app->fonts->DrawText(nameChar, 80, 200 + i, 200, 200, {255,255,255}, app->fonts->gameFont);
		app->fonts->DrawText("- HP: ", 80, 230 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(hpChar, 200, 230 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText("- Stamina: ", 80, 260 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(staminaChar, 200, 260 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		i+= 100;
	}

	for (ListItem<Entity*>* enemyItem = enemies.start; enemyItem != NULL; enemyItem = enemyItem->next) {

	}
	//// Bunny stats:
	//app->fonts->DrawText("--- BUNNY ---", 80, 290, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- HP: ", 80, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(UintToChar(bunny->health), 200, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- Stamina: ", 80, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(UintToChar(bunny->stamina), 200, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//// Villager stats:
	uint villagerStamina = enemies.start->data->stamina;
	std::string villagerStaminaString = std::to_string(villagerStamina);
	const char* villagerStaminaChar = villagerStaminaString.c_str();

	uint villagerHP = enemies.start->data->health;
	std::string villagerHPString = std::to_string(villagerHP);
	const char* villagerHpChar = villagerHPString.c_str();

	int w_window = app->win->width;

	app->fonts->DrawText("--- VILLAGER ---", 1690, 200, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	app->fonts->DrawText("- HP: ", 1690, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	app->fonts->DrawText(villagerHpChar, 1810, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	app->fonts->DrawText("- Stamina: ", 1690, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	app->fonts->DrawText(villagerStaminaChar, 1810, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	app->fonts->DrawText("--- NEXT  TURN --- ", 1690, 340, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	app->fonts->DrawText(turnList.At(1)->data->namechar.GetString(), 1690, 365, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	
	
}

// Loads combat map from Map module using GID tile metadata
bool BattleManager::MakeCombatMap() {

	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].inRange = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
}

void BattleManager::UpdateCombatMap() {

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].character= nullptr;
			combatMap[i][j].inRange = false;
			combatMap[i][j].isCharacter = false;
	
		}
	}

	for (ListItem<Entity*>* entity = turnList.start; entity != NULL; entity = entity->next) {

         combatMap[entity->data->tilePos.x][entity->data->tilePos.y].character = entity->data;
		 combatMap[entity->data->tilePos.x][entity->data->tilePos.y].isCharacter = true;
	}

}

void BattleManager::UpdateEntitiesTilePos()
{

	ListItem<Entity*>* entity;
	entity = turnList.start;
	while(entity!=NULL) {
		
		entity->data->tilePos = app->map->WorldToMap(entity->data->position.x - app->render->camera.x, entity->data->position.y - app->render->camera.y);
		
		entity = entity->next;
	}
}

bool BattleManager::GetActionArea(Entity* character, ActionType type) {

	iPoint nexTile;
	iPoint pos;
	switch (type) {
	case ActionType::MOVE:
		//actionArea = character.attackArea

		for (int i = 0; i < character->movement; i++) {
			for (int j = 0; j < character->movement - i; j++) {

				nexTile = iPoint(currentTurn->tilePos.x + j, currentTurn->tilePos.y + i);
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}
				nexTile = iPoint(currentTurn->tilePos.x - j, currentTurn->tilePos.y + i);
				
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

				nexTile = iPoint(currentTurn->tilePos.x - j, currentTurn->tilePos.y - i);
			
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

				nexTile = iPoint(currentTurn->tilePos.x + j, currentTurn->tilePos.y - i);
				
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

			}

		}

		break;
	case ActionType::ATTACK:
		//actionArea = character.abiltyArea;
		for (int i = 1; i <= character->AttArea; i++) {
			if (combatMap[character->tilePos.x + i][character->tilePos.y].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x + i][character->tilePos.y]);
			}
			if (combatMap[character->tilePos.x - i][character->tilePos.y].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x - i][character->tilePos.y]);
			}
			if (combatMap[character->tilePos.x][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x][character->tilePos.y + i]);
			}
			if (combatMap[character->tilePos.x][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x][character->tilePos.y - i]);
			}
		}
		break;
	case ActionType::ABILITY:
		//actionArea = character.moveArea;


		for (int i = 0; i <= character->Ab1Area; i++) {
			for (int j = 0; j <= character->Ab1Area - i; j++) {
				if (combatMap[character->tilePos.x + j][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x + j][character->tilePos.y + i]);
				}
				if (combatMap[character->tilePos.x - j][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x - j][character->tilePos.y + i]);
				}
				if (combatMap[character->tilePos.x - j][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x - j][character->tilePos.y - i]);
				}
				if (combatMap[character->tilePos.x + j][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x + j][character->tilePos.y - i]);
				}
			}

		}
		break;
	default:
		break;
	}

	return true;
}

bool BattleManager::MousePosition() {
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);


	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x][mouseTile.y].character != false) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);

	}

	return true;
}

bool BattleManager::Move(int pathindex, int length) {

	iPoint dist;
	fPoint pixelPosition;
	fPoint finalPosition;
	float distance;
	b2Vec2 vel = b2Vec2(0, 0);

	const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();


	pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
	pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;

	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;


	dist.x = pixelPosition.x - currentTurn->position.x;
	dist.y = pixelPosition.y - currentTurn->position.y;


	xDir = 0;
	yDir = 0;
	xDir = (dist.x > 0) ? 1 : -1;
	yDir = (dist.y > 0) ? 1 : -1;
	if (dist.x == 0) {
		xDir = 0;
	}
	if (dist.y == 0) {
		yDir = 0;
	}
	if (dist.x == 0 && dist.y == 0) {
		pathIndex++;

	}
	else if (abs(dist.x) > 0) {
		vel.x = 5 * xDir;

	}
	else if (abs(dist.y) > 0) {
		vel.y = 5 * yDir;

	}

	currentTurn->position.x = currentTurn->position.x + vel.x;
	currentTurn->position.y = currentTurn->position.y + vel.y;

	return true;
}

bool BattleManager::AddCharacter(Entity* character, int x, int y, bool isEnemy) {

	if (isEnemy) enemies.Add(character);
	else allies.Add(character);
	character->position = iPoint(x * 120, y * 120);
	character->position = character->position;
	combatMap[x][y].character = character;

	return true;
}

bool BattleManager::MakeTurnList() {

	List<Entity*> auxList;

	//Fill auxList
	for (ListItem<Entity*>* allyItem = allies.start; allyItem != NULL; allyItem = allyItem->next) {
		auxList.Add(allyItem->data);
	}
	for (ListItem<Entity*>* enemyItem = enemies.start; enemyItem != NULL; enemyItem = enemyItem->next) {
		auxList.Add(enemyItem->data);
	}

	//Sort auxList into turnList based on Entity->speed
	while (auxList.Count() > 0) {
		ListItem<Entity*>* auxIndex = auxList.start;

		for (ListItem<Entity*>* auxItem = auxList.start; auxItem != NULL; auxItem = auxItem->next) {
			if (auxItem->data->speed > auxIndex->data->speed || auxIndex == NULL) {
				auxIndex = auxItem;
			}
		}

		auxList.Del(auxIndex);
		turnList.Add(auxIndex->data);
		auxIndex = NULL;
	}
	
	return true;
}

bool BattleManager::SelectTargets() {


	ListItem<TileData*>* tileListItem;
	


	for (tileListItem = actionArea.start; tileListItem != NULL; tileListItem = tileListItem->next) {

		if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy==true && currentTurn->isEnemy==false) {

			targets.Add(tileListItem->data->character);
		}
		else if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy == false && currentTurn->isEnemy == true) {

			targets.Add(tileListItem->data->character);
		}

	}

	return true;
}

bool BattleManager::DisplayEnemys() {


	for (ListItem<Entity*>* entitylist = targets.start; entitylist != NULL; entitylist = entitylist->next) {
		iPoint pos = iPoint(entitylist->data->position.x, entitylist->data->position.y);

		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 250, 0, 0, 100);

	}


	return true;
}

bool BattleManager::UpdateTurnList() {

	ListItem<Entity*>* auxItem;
	auxItem = turnList.start;

	turnList.Del(auxItem);
	turnList.Add(auxItem->data);

	currentTurn = turnList.start->data;

	return true;
}

bool BattleManager::DisplayTurnList() {

	ListItem<Entity*>* listItem;

	for (listItem = turnList.start; listItem != NULL; listItem = listItem->next) {
		//call icon draw function in entity. To yet be implemented
	}


	return true;
}

bool BattleManager::DisplayAtackArea(ActionType type) {

	bool ret = true;

	ListItem<TileData*>* tileListItem;
	tileListItem = actionArea.start;

	uint color[3];

	switch (type)
	{
	case ActionType::MOVE:
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		break;
	case ActionType::ATTACK:
		color[0] = 0;
		color[1] = 255;
		color[2] = 0;
		break;
	case ActionType::ABILITY:
		color[0] = 0;
		color[1] = 0;
		color[2] = 255;
		break;
	default:
		break;
	}

	while (tileListItem != NULL) {

		iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, color[0], color[1], color[2], 100);

		tileListItem = tileListItem->next;
	}

	return ret;
}

bool BattleManager::DisplayMoveArea(ActionType type) {
	
		bool ret = true;
	
		ListItem<TileData*>*tileListItem;
		tileListItem = area.start;
	
		uint color[3];
	
	
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		
	
		while (tileListItem != NULL) {
	
			
			iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
			app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 0, 0, 250, 100);
	
			tileListItem = tileListItem->next;
		}
	
		return ret;
	}

bool BattleManager::ApplyAction(Entity* character, ActionType type) {

	for (ListItem<Entity*>* targetItem = targets.start; targetItem != NULL; targetItem = targetItem->next) {
		switch (type) {
		case ActionType::ATTACK:
			targetItem->data->TakeDamage(character->attack);
			break;
		case ActionType::ABILITY:
			targetItem->data->TakeDamage(character->Ab1Power);
			break;
		default:
			break;
		}
	}

	return true;
}

void BattleManager::CheckWinCondition()
{


	if (allies.Count() == 0 || app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {

		battleState = BattleState::LOSE;
	}
	
	if (enemies.Count() == 0 || app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {

		battleState = BattleState::WIN;
	}
	
}

void BattleManager::LiveCondition() {



	for (ListItem<Entity*>* ally = allies.start; ally != NULL; ally = ally->next) {

		if (ally->data->health <= 0) {

			ally->data->isAlive = false;
		}
		if (ally->data->isAlive == false) {

			allies.Del(ally);
		 }
	}
	for (ListItem<Entity*>* enemy = enemies.start; enemy != NULL; enemy = enemy->next) {


		if (enemy->data->health <= 0) {

			enemy->data->isAlive = false;
		}
		if (enemy->data->isAlive == false) {

			enemies.Del(enemy);
		}
	}
}

void BattleManager::PauseMenuAppear()
{
	// If player is in pause, close it
	if (!isPaused)
	{
		app->uiModule->currentMenuType = COMBAT;
		// Call this function only when scene is changed
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		isPaused = true;
	}
	// If player is NOT in pause, open it
	else
	{
		app->uiModule->currentMenuType = COMBAT_PAUSE;
		// Call this function only when scene is changed
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		isPaused = false;
	}
}

void BattleManager::GodMode()
{

	for (ListItem<Entity*>* ally = allies.start; ally != NULL; ally = ally->next) {

		ally->data->health = ally->data->maxHealth;
		ally->data->stamina = ally->data->maxStamina;
		
	}
}

void BattleManager::TriggerAIAttack()
{
	ListItem<Entity*>* entitylist;
		entitylist = targets.start;
	
		while (entitylist != NULL) {
	
			if (entitylist->data->isAlive == true) {
				entitylist->data->health = entitylist->data->health - (currentTurn->attack - entitylist->data->defense);
				targets.Clear();
				currentTurn->UseStamina(5);
				app->sceneBattle->TakeDamageAnimation(entitylist->data->name.GetString());

			}
	
			entitylist = entitylist->next;
	
		}
}

bool BattleManager::IaEnemyAttack() {

	GetActionArea(currentTurn, ActionType::ATTACK);
	SelectTargets();

	if (currentTurn->stamina >= 5) {
		ListItem<Entity*>* entitylist;
		for (entitylist = targets.start; entitylist != NULL; entitylist = entitylist->next) {
			if (entitylist->data->isAlive == true) {
				return true;
			}

		}
	}
	return false;
}

//Mira si puedes desplazarte, hacia donde te mueves y crea el path.
bool BattleManager::IaEnemyMove() {

	GetActionArea(currentTurn, ActionType::MOVE);

	if (currentTurn->stamina >= 5) {
		ListItem<TileData*>* tiledata;
		for (tiledata = actionArea.start; tiledata != NULL; tiledata = tiledata->next) {

			if (tiledata->data->type == TILE_TYPE::FLOOR) {
				iPoint pos = iPoint(tiledata->data->x, tiledata->data->y);

				CreateArea(currentTurn->AttArea, 1, pos);
				for (int i = 0; i < area.Count(); i++) {

					if (area.At(i)->data->character!=NULL &&area.At(i)->data->character->isAlive==true) {

						length = app->pathfinding->CreatePath(origin, pos);
						destination.x = pos.x;
						destination.y = pos.y;
						currentTurn->UseStamina(3);
						i = area.Count();

						return true;

					}

				}

			}
		}


	}
	return false;

}

bool BattleManager::CreateArea(int range, int type, iPoint posTile) {


	switch (type) {

	case 0:
		//attack
		if (combatMap[posTile.x + 1][posTile.y].type == TILE_TYPE::FLOOR) {

			area.Add(&combatMap[posTile.x + 1][posTile.y]);
		}
		if (combatMap[posTile.x - 1][posTile.y].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x - 1][posTile.y]);
		}
		if (combatMap[posTile.x][posTile.y + 1].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x][posTile.y + 1]);
		}
		if (combatMap[posTile.x][posTile.y - 1].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x][posTile.y - 1]);
		}
		break;
	case 1:
		//lineal
		for (int i = 1; i <= range; i++) {
			if (combatMap[posTile.x + i][posTile.y].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x + i][posTile.y]);
			}
			if (combatMap[posTile.x - i][posTile.y].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x - i][posTile.y]);
			}
			if (combatMap[posTile.x][posTile.y + i].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x][posTile.y + i]);
			}
			if (combatMap[posTile.x][posTile.y - i].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x][posTile.y - i]);
			}
		}
		break;
	case 2:
		//circular
		int i;
		int j;
		for (i = 0; i <= range; i++) {
			for (j = 0; j <= range - i; j++) {
				if (combatMap[posTile.x + j][posTile.y + i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x + j][posTile.y + i]);
				}
				if (combatMap[posTile.x - j][posTile.y + i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x - j][posTile.y + i]);
				}
				if (combatMap[posTile.x - j][posTile.y - i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x - j][posTile.y - i]);
				}
				if (combatMap[posTile.x + j][posTile.y - i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x + j][posTile.y - i]);
				}
			}

		}

		break;
	case 3:
		//global
		for (int i = 0; i < COMBAT_MAP_HEIGHT; i++) {
			for (int j = 0; j < COMBAT_MAP_WIDTH; j++) {
				iPoint pos = iPoint(i, j);
				if (combatMap[j][i].type == TILE_TYPE::FLOOR) {

					area.Add(&combatMap[j][i]);

				}

			}
		}
		break;
	case 4:
		//LRRH attack area

		//for (int i = -1; i < 2; i++) {

		//	for (int j = 0; j < 8; i++) {

		//		if (posTile.x - j <2  && posTile.y + i < 9 && combatMap[posTile.x + j][posTile.y+i].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x + i][posTile.y+i]);
		//		}
		//		if (posTile.x - j < 2 && posTile.y + i > 9 && combatMap[posTile.x - j][posTile.y+i].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x - j][posTile.y+i]);
		//		}
		//		if (posTile.x+i >14 && posTile.y + j>6 && combatMap[posTile.x+i][posTile.y + j].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x+i][posTile.y + i]);
		//		}
		//		if (posTile.x + i > 14 && posTile.y - j < 2 && combatMap[posTile.x+i][posTile.y - j].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x+i][posTile.y - j]);
		//		}
		//	}

		//}

		break;
	}


	return true;

}

