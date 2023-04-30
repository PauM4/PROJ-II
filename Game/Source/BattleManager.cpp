#include "BattleManager.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

BattleManager::BattleManager(bool isActive) : Module(isActive) {

	name.Create("battleManager");
}

// Destructor
BattleManager::~BattleManager() {}

bool BattleManager::Awake(pugi::xml_node& config) {


	return true;
}

bool BattleManager::Start() {


	return true;
}

bool BattleManager::PreUpdate() {


	return true;
}

bool BattleManager::Update(float dt) {


	return true;
}

bool BattleManager::PostUpdate() {


	return true;
}

bool BattleManager::CleanUp() {


	return true;
}

Entity* BattleManager::GetCurrentTurn() { return currentTurn;}


// Loads combat map from Map module using GID tile metadata
bool BattleManager::MakeCombatMap() {

	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
}

bool BattleManager::GetActionArea(Entity* character, ActionType type) {

	switch (type) {
	case ActionType::MOVE:
		//actionArea = character.attackArea;
		break;
	case ActionType::ATTACK:
		//actionArea = character.abiltyArea;
		break;
	case ActionType::ABILITY:
		//actionArea = character.moveArea;
		break;
	default:
		break;
	}

	return true;
}


bool BattleManager::AddCharacter(Entity* character, int x, int y, bool isEnemy) {

	if (isEnemy) enemies.Add(character);
	else allies.Add(character);

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
		ListItem<Entity*>* auxIndex = NULL;

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

bool BattleManager::DisplayArea(ActionType type) {

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
