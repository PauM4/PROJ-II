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





	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);



	app->audio->PlayMusic("Assets/Sounds/Music/music_battle_2.wav", 0.2f);

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


	//// Menu appear
	//if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	//{
	//	// If player is in pause, close it
	//	if (!isPaused)
	//	{
	//		app->uiModule->currentMenuType = COMBAT;
	//		// Call this function only when scene is changed
	//		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	//		isPaused = true;
	//	}
	//	// If player is NOT in pause, open it
	//	else
	//	{
	//		app->uiModule->currentMenuType = COMBAT_PAUSE;
	//		// Call this function only when scene is changed
	//		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	//		isPaused = false;
	//	}
	//}

	//if (movepressed == true) {

	//	if (characterTurn->stamina >= 3) {

	//		move = true;
	//		ability = false;
	//		atack = false;
	//	}
	//	movepressed = false;
	//}

	//if (attackpressed == true) {

	//	if (characterTurn->stamina >= 5) {
	//		targets.Clear();
	//		DestroyListArea();
	//		CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
	//		GetTargets();
	//		atack = true;
	//		move = false;
	//		ability = false;
	//	}
	//	attackpressed = false;
	//}

	//if (abiltypressed == true) {

	//	if (characterTurn->stamina >= 10) {
	//		targets.Clear();
	//		DestroyListArea();
	//		CreateArea(characterTurn->Ab1Area, 2, characterTurn->tilePos);
	//		GetTargets();
	//		ability = true;
	//		atack = false;
	//		move = false;
	//		abiltypressed = false;
	//	}

	//	abiltypressed = false;
	//}

	//if (endturnpressed == true) {


	//	atack = false;
	//	ability = false;
	//	move = false;
	//	turnstart = false;
	//	characterTurn->GainStamina(10);
	//	endturnpressed = false;

	//}

	app->map->Draw();

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return false;
	}*/

	return true;
}

// Called each loop iteration
bool SceneCombatLHHR::PostUpdate()
{
	bool ret = true;

	//// UI Stats for Battle
	//// UI Stats for Battle
	//// Timmy stats:
	//uint timmyStamina = timmy->stamina;
	//std::string timmyStaminaString = std::to_string(timmyStamina);
	//const char* timmyStaminaChar = timmyStaminaString.c_str();

	//uint timmyHP = timmy->health;
	//std::string timmyHPString = std::to_string(timmyHP);
	//const char* timmyHpChar = timmyHPString.c_str();

	//app->fonts->DrawText("--- TIMMY ---", 80, 200, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- HP: ", 80, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(timmyHpChar, 200, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- Stamina: ", 80, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(timmyStaminaChar, 200, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//// Bunny stats:
	//uint bunnyStamina = bunny->stamina;
	//std::string bunnyStaminaString = std::to_string(bunnyStamina);
	//const char* bunnyStaminaChar = bunnyStaminaString.c_str();

	//uint bunnyHP = bunny->health;
	//std::string bunnyHPString = std::to_string(bunnyHP);
	//const char* bunnyHpChar = bunnyHPString.c_str();

	//app->fonts->DrawText("--- BUNNY ---", 80, 290, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- HP: ", 80, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(bunnyHpChar, 200, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- Stamina: ", 80, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(bunnyStaminaChar, 200, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//// Villager stats:
	//uint villagerStamina = LRRH->stamina;
	//std::string villagerStaminaString = std::to_string(villagerStamina);
	//const char* villagerStaminaChar = villagerStaminaString.c_str();

	//uint villagerHP = LRRH->health;
	//std::string villagerHPString = std::to_string(villagerHP);
	//const char* villagerHpChar = villagerHPString.c_str();

	//int w_window = app->win->width;

	//app->fonts->DrawText("--- LRRH ---", 1690, 200, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- HP: ", 1690, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(villagerHpChar, 1810, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- Stamina: ", 1690, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(villagerStaminaChar, 1810, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("--- NEXT  TURN --- ", 1690, 440, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//// Villager stats:
	//uint sproutStamina = sprout->stamina;
	//std::string sproutStaminaString = std::to_string(sproutStamina);
	//const char* sproutStaminaChar = sproutStaminaString.c_str();

	//uint sproutHP = sprout->health;
	//std::string sproutHPString = std::to_string(sproutHP);
	//const char* sproutHpChar = sproutHPString.c_str();

	//app->fonts->DrawText("--- Sprout ---", 1690, 290, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- HP: ", 1690, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(sproutHpChar, 1810, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("- Stamina: ", 1690, 360, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText(sproutHpChar, 1810, 360, 200, 200, { 255,255,255 }, app->fonts->gameFont);
	//if (turnqueue.At(1)->data->id == 1) {

	//	app->fonts->DrawText("TIMMY", 1690, 480, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//}
	//if (turnqueue.At(1)->data->id == 2) {

	//	app->fonts->DrawText("BUNNY", 1690, 480, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//}
	//if (turnqueue.At(1)->data->id == 4) {

	//	app->fonts->DrawText("RED  HOOD", 1690, 480, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//}
	//if (turnqueue.At(1)->data->id == 5) {

	//	app->fonts->DrawText("SPROUT", 1690, 480, 200, 200, { 255,255,255 }, app->fonts->gameFont);

	//}
	//// End of Stats UI

	//timmy->tilePos = app->map->WorldToMap(timmy->position.x - app->render->camera.x, timmy->position.y - app->render->camera.y);
	//bunny->tilePos = app->map->WorldToMap(bunny->position.x - app->render->camera.x, bunny->position.y - app->render->camera.y);
	//LRRH->tilePos = app->map->WorldToMap(LRRH->position.x - app->render->camera.x, LRRH->position.y - app->render->camera.y);
	//sprout->tilePos = app->map->WorldToMap(sprout->position.x - app->render->camera.x, sprout->position.y - app->render->camera.y);
	//

	//if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	//{
	//	if (godmode)
	//	{
	//		godmode = false;
	//	}
	//	else
	//	{
	//		godmode = true;
	//	}
	//}

	//if (godmode == true) {

	//	bunny->health = bunny->maxHealth;
	//	timmy->health = timmy->maxHealth;
	//	bunny->stamina = bunny->maxStamina;
	//	timmy->stamina = timmy->maxStamina;
	//}

	//
	//if (timmy->health <= 0) {

	//	timmy->isAlive = false;

	//}

	//if (bunny->health <= 0) {

	//	bunny->isAlive = false;

	//}
	//if (LRRH->health <= 0) {

	//	LRRH->health = 0;
	//	LRRH->isAlive = false;

	//}
	//if (sprout->health <= 0) {

	//	sprout->health = 0;
	//	sprout->isAlive = false;

	//}

	////if (LRRH->health <= 0 && timmy->health <= 0) win = true;
	//if (bunny->health <= 0 && timmy->health <= 0) lose = true;

	//if (LRRH->health <= 0 ) {

	//	LRRH->health = 0;
	//	LRRH->isAlive = false;
	//	LRRH->takedmgAnim.Reset();

	//	sprout->health = 0;
	//	sprout->isAlive = false;
	//	sprout->takedmgAnim.Reset();

	//	win = true;

	//}

	//if (characterTurn->isAlive == false) {
	//	turnstart = false;

	//}



	//if (turnstart == false) {

	//	app->pathfinding->ClearLastPath();
	//	length = 1;

	//	moveenemy = false;
	//	GetNext();

	//	origin = characterTurn->tilePos;
	//	if (characterTurn->isEnemy == true) {

	//		targets.Clear();
	//		DestroyListArea();
	//		CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
	//		GetTargets();
	//		moveenemy = true;
	//	}
	//	
	//	ability = false;
	//	turnstart = true;
	//	moveanim = false;
	//}

	//if (app->pathfinding->IsWalkable(origin)) {
	//	originSelected = true;


	//}

	//app->pathfinding->ClearLastPath();

	//int mouseX, mouseY;
	//app->input->GetMousePosition(mouseX, mouseY);


	//mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);
	//iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	//if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x][mouseTile.y].character != false && move == false) {
	//	app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);

	//}

	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{


	//	if (originSelected == true && length == 1 && move == true && moveenemy == false)
	//	{
	//		if (app->pathfinding->IsWalkable(origin) && combatMap[mouseTile.x][mouseTile.y].inRange == true && combatMap[mouseTile.x][mouseTile.y].character == false) {
	//			length = app->pathfinding->CreatePath(origin, mouseTile);
	//			destination.x = mouseTile.x;
	//			destination.y = mouseTile.y;
	//			originSelected = false;
	//			move = false;
	//			moveanim = true;
	//			characterTurn->UseStamina(3);

	//		}
	//		else {
	//			app->pathfinding->ClearLastPath();
	//		}
	//	}
	//	/*		else if(combatMap[mouseTile.x ][ mouseTile.y ].character != false &&length == 1)
	//			{
	//					origin = mouseTile;
	//					if (app->pathfinding->IsWalkable(origin)) {
	//						originSelected = true;
	//						move = true;
	//						characterTurn = combatMap[mouseTile.x][mouseTile.y].characterType;
	//					}
	//					app->pathfinding->ClearLastPath();

	//			}*/

	//}


	//const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();
	//for (uint i = 0; i < lastpath->Count(); ++i)
	//{
	//	iPoint pos = app->map->MapToWorld(lastpath->At(i)->x, lastpath->At(i)->y);

	//	LOG("posTileY= %d", lastpath->At(i)->y);

	//	app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);

	//}

	//// L12: Debug pathfinding
	//iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	//if (app->pathfinding->IsWalkable(origin) && originSelected == true) {

	//	app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	//	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	//}





	//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	//	timmy->health = 0;
	//}

	//if (atack == true) {


	//	DisplayArea(1);
	//	DisplayEnemys();

	//	for (int i = 0; i < targets.Count(); i++) {


	//		if (targets.At(i)->data->tilePos == mouseTile) {
	//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {

	//				targets.At(i)->data->health = targets.At(i)->data->health - (characterTurn->attack - targets.At(i)->data->defense);
	//				atack = false;
	//				characterTurn->UseStamina(5);
	//				turnstart = false;
	//			}
	//		}
	//	}
	//}
	//if (ability == true) {


	//	DisplayArea(2);
	//	DisplayEnemys();

	//	for (int i = 0; i < targets.Count(); i++) {


	//		if (targets.At(i)->data->tilePos == mouseTile) {
	//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {

	//				targets.At(i)->data->health = targets.At(i)->data->health - (characterTurn->Ab1Power - targets.At(i)->data->defense);
	//				ability = false;
	//				characterTurn->UseStamina(10);
	//				turnstart = false;
	//				i = targets.Count();
	//			}
	//		}
	//	}
	//}



	//if (pathIndex != length) {
	//	length = app->pathfinding->CreatePath(origin, destination);
	//}
	//else {
	//	


	//}

	//if (length > 1) {

	//	Move(pathIndex, length);
	//}
	//else
	//{
	//	if (characterTurn->tilePos == destination) {

	//		move = false;
	//		destination = iPoint(0, 0);
	//		turnstart = false;

	//	}
	//}




	//if (move == true || moveenemy == true) {
	//	int j = 0;
	//	int i = 0;
	//	iPoint nexTile;
	//	iPoint pos;
	//	for (i = 0; i < characterTurn->movement; i++) {
	//		for (j = 0; j < characterTurn->movement - i; j++) {

	//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);

	//			combatMap[nexTile.x][nexTile.y].inRange = true;
	//			/*arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/

	//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
	//			combatMap[nexTile.x][nexTile.y].inRange = true;
	//			/*arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

	//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
	//			combatMap[nexTile.x][nexTile.y].inRange = true;
	//			/* arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

	//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
	//			combatMap[nexTile.x][nexTile.y].inRange = true;
	//			/* arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

	//		}

	//	}

	//	for (i = 0; i < characterTurn->movement; i++) {
	//		for (j = 0; j < characterTurn->movement - i; j++) {

	//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
	//			if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
	//				app->pathfinding->IsWalkable(nexTile) == false) {

	//				combatMap[nexTile.x][nexTile.y].inRange = false;
	//				/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
	//			}
	//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
	//			if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
	//				app->pathfinding->IsWalkable(nexTile) == false) {

	//				combatMap[nexTile.x][nexTile.y].inRange = false;
	//				/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
	//			}

	//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
	//			if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
	//				app->pathfinding->IsWalkable(nexTile) == false) {

	//				combatMap[nexTile.x][nexTile.y].inRange = false;
	//				/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
	//			}

	//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
	//			if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
	//				combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
	//				app->pathfinding->IsWalkable(nexTile) == false) {

	//				combatMap[nexTile.x][nexTile.y].inRange = false;
	//				/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
	//			}

	//		}

	//	}





	//}

	//if (move == false && moveenemy==false)
	//{
	//	for (int i = 0; i < 16; i++) {
	//		for (int j = 0; j < 9; j++) {

	//			combatMap[i][j].characterType = nullptr;
	//			combatMap[i][j].character = false;
	//			combatMap[i][j].inRange = false;
	//			combatMap[i][j].enemy = false;
	//			combatMap[i][j].dead = false;
	//		}
	//	}

	//	/*DestroyListArea(arealist);*/
	//}

	//for (int i = 0; i < 16; i++) {
	//	for (int j = 0; j < 9; j++) {
	//		if (combatMap[i][j].inRange == true && combatMap[i][j].character == false && atack == false) {
	//			iPoint pos = iPoint(i, j);
	//			if (app->pathfinding->IsWalkable(pos)) {
	//				pos = app->map->MapToWorld(pos.x, pos.y);
	//				app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
	//			}
	//		}

	//	}

	//}

	//if (moveenemy == true) {

	//    /*	move = true;
	//	moveanim = false;
	//

	//	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {

	//		atack = true;

	//	}*/

	//	if (characterTurn->stamina >= 5 && length==1) {
	//		ListItem<Entity*>* entitylist;
	//		entitylist = targets.start;

	//		while (entitylist != NULL && moveenemy == true) {

	//			/*Combat(characterTurn, targets, 1);*/

	//			if (entitylist->data->isAlive == true) {
	//				entitylist->data->health = entitylist->data->health - (characterTurn->attack - entitylist->data->defense);
	//				targets.Clear();
	//				characterTurn->UseStamina(5);
	//				turnstart = false;
	//				atack = false;
	//				moveenemy = false;

	//			}

	//			entitylist = entitylist->next;

	//		}

	//	}


	//	if (moveenemy == true && characterTurn->stamina >= 3) {

	//		moveanim = true;
	//		move = true;
	//		for (int i = 0; i < 16; i++) {
	//			for (int j = 0; j < 9; j++) {

	//				if (moveenemy == true) {
	//					if (combatMap[i][j].inRange == true && combatMap[i][j].character == false && atack == false) {
	//						iPoint pos = iPoint(i, j);

	//						if (app->pathfinding->IsWalkable(pos)) {
	//							CreateArea(LRRH->AttArea, 1, pos);
	//							for (int i = 0; i < area.Count(); i++) {

	//								if (area.At(i)->data->character == true && area.At(i)->data->dead == false) {

	//									length = app->pathfinding->CreatePath(origin, pos);
	//									destination.x = pos.x;
	//									destination.y = pos.y;
	//									originSelected = false;
	//									moveenemy = false;
	//									characterTurn->UseStamina(3);
	//									i = area.Count();

	//								}

	//							}
	//						}
	//					}
	//					
	//				}

	//			}

	//		}
	//	} 
	//	if(moveenemy == true && characterTurn->stamina >= 3) {

	//		moveanim = true;
	//		
	//				if (moveenemy == true) {
	//					
	//						iPoint pos = iPoint(characterTurn->tilePos.x - 3, characterTurn->tilePos.y);
	//						
	//						if (app->pathfinding->IsWalkable(pos)) {
	//							

	//									length = app->pathfinding->CreatePath(origin, pos);
	//									destination.x = pos.x;
	//									destination.y = pos.y;
	//									originSelected = false;
	//									moveenemy = false;
	//									characterTurn->UseStamina(3);
	//
	//							
	//						}
	//					
	//				
	//				}

	//			

	//		
	//	}
	//	else if (moveenemy == true) {

	//		characterTurn->GainStamina(10);
	//		moveenemy = false;
	//	 }


	//}



	//combatMap[LRRH->tilePos.x][LRRH->tilePos.y].enemy = true;
	//combatMap[LRRH->tilePos.x][LRRH->tilePos.y].characterType = LRRH;

	//combatMap[sprout->tilePos.x][sprout->tilePos.y].enemy = true;
	//combatMap[sprout->tilePos.x][sprout->tilePos.y].characterType = sprout;


	//if (bunny->isAlive == true) {
	//	combatMap[bunny->tilePos.x][bunny->tilePos.y].dead = false;
	//}
	//else {

	//	combatMap[bunny->tilePos.x][bunny->tilePos.y].dead = true;
	//}

	//combatMap[bunny->tilePos.x][bunny->tilePos.y].character = true;
	//combatMap[bunny->tilePos.x][bunny->tilePos.y].characterType = bunny;


	//if (timmy->isAlive == true) {
	//	combatMap[timmy->tilePos.x][timmy->tilePos.y].dead = false;
	//}
	//else {

	//	combatMap[timmy->tilePos.x][timmy->tilePos.y].dead = true;
	//}

	//combatMap[timmy->tilePos.x][timmy->tilePos.y].character = true;
	//combatMap[timmy->tilePos.x][timmy->tilePos.y].characterType = timmy;






	//if (characterTurn->id == 1) {
	//	bunny->currentAnimation = &bunny->idleAnim;
	//	SDL_Rect recta = bunny->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &recta);

	//	LRRH->currentAnimation = &LRRH->idleAnim;
	//	SDL_Rect recti = LRRH->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &recti);

	//	sprout->currentAnimation = &sprout->idleAnim;
	//	SDL_Rect recto = sprout->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &recto);

	//	if (moveanim == false) {
	//		timmy->currentAnimation = &timmy->idleAnim;
	//		SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//		app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//	}
	//	if (moveanim == true) {
	//		if (xDir == 1) {
	//			timmy->currentAnimation = &timmy->walkRightAnim;
	//			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//			timmy->currentAnimation->Update();
	//		}
	//		if (xDir == -1) {
	//			timmy->currentAnimation = &timmy->walkLeftAnim;
	//			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//			timmy->currentAnimation->Update();
	//		}
	//		if (yDir == 1) {
	//			timmy->currentAnimation = &timmy->walkDownAnim;
	//			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//			timmy->currentAnimation->Update();
	//		}
	//		if (yDir == -1) {
	//			timmy->currentAnimation = &timmy->walkUpAnim;
	//			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//			timmy->currentAnimation->Update();
	//		}
	//		if (xDir == 0 || yDir == 0) {
	//			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
	//			timmy->currentAnimation->Update();
	//		}
	//	}

	//}
	//if (characterTurn->id == 2) {

	//	timmy->currentAnimation = &timmy->idleAnim;
	//	SDL_Rect recta = timmy->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &recta);
	//	LRRH->currentAnimation = &LRRH->idleAnim;

	//	SDL_Rect recti = LRRH->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &recti);
	//	sprout->currentAnimation = &sprout->idleAnim;

	//	SDL_Rect recto = sprout->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &recto);

	//	if (moveanim == false) {
	//		bunny->currentAnimation = &bunny->idleAnim;
	//		SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//		app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//	}
	//	if (moveanim == true) {
	//		if (xDir == 1) {
	//			bunny->currentAnimation = &bunny->walkRightAnim;
	//			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//			bunny->currentAnimation->Update();
	//		}
	//		if (xDir == -1) {
	//			bunny->currentAnimation = &bunny->walkLeftAnim;
	//			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//			bunny->currentAnimation->Update();
	//		}
	//		if (yDir == 1) {
	//			bunny->currentAnimation = &bunny->walkDownAnim;
	//			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//			bunny->currentAnimation->Update();
	//		}
	//		if (yDir == -1) {
	//			bunny->currentAnimation = &bunny->walkUpAnim;
	//			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//			bunny->currentAnimation->Update();
	//		}
	//		if (xDir == 0 || yDir == 0) {
	//			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
	//			bunny->currentAnimation->Update();
	//		}
	//	}

	//}

	//if (characterTurn->id == 4) {

	//	timmy->currentAnimation = &timmy->idleAnim;
	//	SDL_Rect recti = timmy->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &recti);

	//	bunny->currentAnimation = &bunny->idleAnim;
	//	SDL_Rect recta = bunny->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &recta);

	//	sprout->currentAnimation = &sprout->idleAnim;
	//	SDL_Rect recto = sprout->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &recto);

	//	if (moveanim == false) {
	//		LRRH->currentAnimation = &LRRH->idleAnim;
	//		SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//		app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//	}
	//	if (moveanim == true) {
	//		if (xDir == 1) {
	//			LRRH->currentAnimation = &LRRH->walkRightAnim;
	//			SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//			LRRH->currentAnimation->Update();
	//		}
	//		if (xDir == -1) {
	//			LRRH->currentAnimation = &LRRH->walkLeftAnim;
	//			SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//			LRRH->currentAnimation->Update();
	//		}
	//		if (yDir == 1) {
	//			LRRH->currentAnimation = &LRRH->walkDownAnim;
	//			SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//			LRRH->currentAnimation->Update();
	//		}
	//		if (yDir == -1) {
	//			LRRH->currentAnimation = &LRRH->walkUpAnim;
	//			SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//			LRRH->currentAnimation->Update();
	//		}
	//		if (xDir == 0 || yDir == 0) {
	//			SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);
	//			LRRH->currentAnimation->Update();
	//		}
	//	}
	//}
	//if (characterTurn->id == 5) {

	//	timmy->currentAnimation = &timmy->idleAnim;
	//	SDL_Rect recti = timmy->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &recti);

	//	bunny->currentAnimation = &bunny->idleAnim;
	//	SDL_Rect recta = bunny->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &recta);

	//	LRRH->currentAnimation = &LRRH->idleAnim;
	//	SDL_Rect rect = LRRH->currentAnimation->GetCurrentFrame();
	//	app->render->DrawTexture(LRRHtexture, LRRH->position.x - 13, LRRH->position.y - 35, &rect);

	//	if (moveanim == false) {
	//		sprout->currentAnimation = &sprout->idleAnim;
	//		SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//		app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//	}
	//	if (moveanim == true) {
	//		if (xDir == 1) {
	//			sprout->currentAnimation = &sprout->walkRightAnim;
	//			SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//			sprout->currentAnimation->Update();
	//		}
	//		if (xDir == -1) {
	//			sprout->currentAnimation = &sprout->walkLeftAnim;
	//			SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//			sprout->currentAnimation->Update();
	//		}
	//		if (yDir == 1) {
	//			sprout->currentAnimation = &sprout->walkDownAnim;
	//			SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//			sprout->currentAnimation->Update();
	//		}
	//		if (yDir == -1) {
	//			sprout->currentAnimation = &sprout->walkUpAnim;
	//			SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//			sprout->currentAnimation->Update();
	//		}
	//		if (xDir == 0 || yDir == 0) {
	//			SDL_Rect rect = sprout->currentAnimation->GetCurrentFrame();
	//			app->render->DrawTexture(sprouttexture, sprout->position.x - 13, sprout->position.y - 35, &rect);
	//			sprout->currentAnimation->Update();
	//		}
	//	}
	//}

	////Print win/lose screen
	//if (win) {
	//	app->render->DrawTexture(winScreen, 0, 0);

	//}
	//if (lose) {
	//	app->render->DrawTexture(loseScreen, 0, 0);
	//}

	//if ((win || lose) && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->sceneManager->LoadScene(GameScene::SCENE);



	
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

	app->pathfinding->ClearLastPath();
	app->map->CleanUp();
	app->entityManager->CleanUp();



	SaveResult();

	return true;
}

void SceneCombatLHHR::SaveResult()
{

	if (app->battleManager->win)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isLRRHDefeated", "true");
	}
	else if (app->battleManager->lose)
	{
		app->UpdateXMLAttributeFromNode("save_game.xml", "BattleInfo", "isLRRHDefeated", "false");
	}
}
