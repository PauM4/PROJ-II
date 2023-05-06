//#include "App.h"
//#include "Input.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Render.h"
//#include "Window.h"
//#include "BattleSystem.h"
//#include "EntityManager.h"
//#include "Entity.h"
//#include "Map.h"
//#include "PathFinding.h"
//#include "GuiManager.h"
//#include "Fonts.h"
//#include "Physics.h"
//#include "Timmy.h"
//
//#include "Defs.h"
//#include "Log.h"
//
//BattleSystem::BattleSystem(bool isActive) : Module(isActive)
//{
//	name.Create("sceneBattle");
//}
//
//// Destructor
//BattleSystem::~BattleSystem()
//{}
//
//// Called before render is available
//bool BattleSystem::Awake(pugi::xml_node& config)
//{
//	LOG("Loading Scene");
//	bool ret = true;
//
//	mapName = config.attribute("name").as_string();
//	mapFolder = config.attribute("path").as_string();
//
//	//This reads some parameters from xml
//	if (config.child("timmy")) {
//		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
//		timmy->parameters = config.child("timmy");
//	}
//	if (config.child("bunny")) {
//		bunny = (Bunny*)app->entityManager->CreateEntity(EntityType::BUNNY);
//		bunny->parameters = config.child("bunny");
//	}
//	if (config.child("enemy_angryVillager")) {
//		villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
//		villager->parameters = config.child("enemy_angryVillager");
//	}
//	//This reads stats from xml
//	if (config.parent().child("timmy")) {
//		timmy->stats = config.parent().child("timmy");
//	}
//	if (config.parent().child("bunny")) {
//		bunny->stats = config.parent().child("bunny");
//	}
//	if (config.parent().child("enemy_angryVillager")) {
//		villager->stats = config.parent().child("enemy_angryVillager");
//	}
//	app->entityManager->Awake(config);
//
//	return ret;
//}
//
//
//// Called before the first frame
//bool BattleSystem::Start()
//{
//	//Load map
//	bool retLoad = app->map->Load(mapName, mapFolder);
//	move = false;
//	originSelected = false;
//	pathIndex = 1;
//	length = 1;
//	turnEnded = false;
//	destination = iPoint(0, 0);
//	movepressed = false;
//	attackpressed = false;
//	abiltypressed = false;
//	endturnpressed = false;
//	godMode = false;
//	moveenemy = false;
//	awchanim1 = false;
//	awchanim2 = false;
//	awchanim3 = false;
//
//	win = false;
//	lose = false;
//
//	app->entityManager->Start();
//	//Load combat map
//
//
//	MakeCombatMap();
//
//	if (retLoad) {
//		int w, h;
//		uchar* data = NULL;
//
//		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
//		if (retWalkMap) app->pathfinding->SetMap(w, h, data);
//
//		RELEASE_ARRAY(data);
//
//	}
//
//	app->render->camera.x = 0;
//	app->render->camera.y = 0;
//
//	moveanim = false;
//	mouseTileTex = app->tex->Load("Assets/Maps/Scenes/Path.png");
//	timmytexture = app->tex->Load("Assets/Characters/Medidas_sprites_anim-sombra_def.png");
//	bunnytexture = app->tex->Load("Assets/Characters/F_sprites_bunny.png");
//	villagertexture = app->tex->Load("Assets/Characters/F_sprites_angry_Villager.png");
//	originTex = app->tex->Load("Assets/Maps/Scenes/Cruz.png");
//
//	winScreen = app->tex->Load("Assets/UI/Win_screen.png");
//	loseScreen = app->tex->Load("Assets/UI/lose_screen.png");
//
//	timmy->tilePos = app->map->WorldToMap(timmy->position.x - app->render->camera.x, timmy->position.y - app->render->camera.y);
//	bunny->tilePos = app->map->WorldToMap(bunny->position.x - app->render->camera.x, bunny->position.y - app->render->camera.y);
//	villager->tilePos = app->map->WorldToMap(villager->position.x - app->render->camera.x, villager->position.y - app->render->camera.y);
//
//
//	//Hacer push de mas iniciativa a menos
//	allentities.Add(timmy);
//	allentities.Add(bunny);
//	allentities.Add(villager);
//
//
//	// Tell to UIModule which currentMenuType
//	app->uiModule->currentMenuType = COMBAT;
//	// Call this function only when buttons change
//	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
//
//	origin = characterTurn->tilePos;
//
//	targets.Clear();
//	DestroyListArea();
//	CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
//	GetTargets();
//	// UI Things
//	isPaused = false;
//
//	app->audio->PlayMusic("Assets/Sounds/Music/music_battle_2.wav", 0.2f);
//
//	// ----------------------------------- NEW -------------------------------
//
//	turnQueue.push_back(timmy);
//	turnQueue.push_back(bunny);
//	turnQueue.push_back(villager);
//
//	currentTurn = turnQueue.size();
//
//	staminaToMove = 3;
//	staminaToAttack = 5;
//	staminaToAbility = 10;
//
//	return true;
//}
//
//
//// Called each loop iteration
//bool BattleSystem::PreUpdate()
//{
//	bool ret = true;
//
//	if (turnEnded)
//	{
//		currentTurn = currentTurn % turnQueue.size();
//		turnEnded = false;
//
//		//cuando acabe el turno: currentTurn++;
//	}
//	
//	
//	return true;
//}
//
//
//// Called each loop iteration
//bool BattleSystem::Update(float dt)
//{
//
//	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
//		PauseMenuAppear();
//
//	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
//		return false;
//
//	TriggerButtonPressed(buttonPressed);
//
//	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
//	{
//		if (godMode)
//		{
//			godMode = false;
//		}
//		else
//		{
//			godMode = false;
//		}
//	}
//
//	if (godMode)
//		GodMode();
//
//	UpdateEntitiesTilePos();
//
//	CheckWinCondition();
//
//
//	if ((win || lose) && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) app->sceneManager->LoadScene(GameScene::SCENE);
//
//	app->map->Draw();
//	return true;
//}
//
//void BattleSystem::UpdateEntitiesTilePos()
//{
//	for (auto& entity : turnQueue)
//	{
//		entity->tilePos = app->map->WorldToMap(entity->position.x - app->render->camera.x, entity->position.y - app->render->camera.y);
//	}
//}
//
//void BattleSystem::UpdateTilesInRange()
//{
//	int j = 0;
//	int i = 0;
//	iPoint nexTile;
//	iPoint pos;
//	for (i = 0; i < characterTurn->movement; i++) {
//		for (j = 0; j < characterTurn->movement - i; j++) {
//
//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
//			combatMap[nexTile.x][nexTile.y].inRange = true;
//			
//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
//			combatMap[nexTile.x][nexTile.y].inRange = true;
//			
//			nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
//			combatMap[nexTile.x][nexTile.y].inRange = true;
//
//			nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
//			combatMap[nexTile.x][nexTile.y].inRange = true;
//		}
//	}
//}
//
//
//
//// Called each loop iteration
//bool BattleSystem::PostUpdate()
//{
//	bool ret = true;
//
//	
//
//	UIStatsForBattle();
//
//	//Print win/lose screen
//	if (win) {
//		app->render->DrawTexture(winScreen, 0, 0);
//
//	}
//	if (lose) {
//		app->render->DrawTexture(loseScreen, 0, 0);
//	}
//	
//	return ret;
//}
//
//// ----------------------------------- NEW -------------------------------
//
//void BattleSystem::UIStatsForBattle()
//{
//
//	// UI Stats for Battle
//	// Timmy stats:
//	app->fonts->DrawText("--- TIMMY ---", 80, 200, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- HP: ", 80, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(timmy->health), 200, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- Stamina: ", 80, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(timmy->stamina), 200, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//
//	// Bunny stats:
//	app->fonts->DrawText("--- BUNNY ---", 80, 290, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- HP: ", 80, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(bunny->health), 200, 320, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- Stamina: ", 80, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(bunny->stamina), 200, 350, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//
//	// Villager stats:
//	uint villagerStamina = villager->stamina;
//	std::string villagerStaminaString = std::to_string(villagerStamina);
//	const char* villagerStaminaChar = villagerStaminaString.c_str();
//
//	uint villagerHP = villager->health;
//	std::string villagerHPString = std::to_string(villagerHP);
//	const char* villagerHpChar = villagerHPString.c_str();
//
//	int w_window = app->win->width;
//
//	app->fonts->DrawText("--- VILLAGER ---", 1690, 200, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- HP: ", 1690, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(villager->health), 1810, 230, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText("- Stamina: ", 1690, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	app->fonts->DrawText(UintToChar(villager->stamina), 1810, 260, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//
//	app->fonts->DrawText("--- NEXT  TURN --- ", 1690, 340, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//	int nextTurn = (currentTurn + 1) % turnQueue.size();
//	app->fonts->DrawText(turnQueue.at(nextTurn)->name.GetString(), 1690, 365, 200, 200, { 255,255,255 }, app->fonts->gameFont);
//
//}
//
//const char* BattleSystem::UintToChar(uint param)
//{
//	std::string timmyStaminaString = std::to_string(param);
//	return timmyStaminaString.c_str();
//	
//}
//
//void BattleSystem::TriggerButtonPressed(CombatButtons button)
//{
//	
//	switch (button)
//	{
//	case CombatButtons::ATTACK:
//
//		if (turnQueue.at(currentTurn)->GetStamina() >= staminaToAttack)
//		{
//			targets.Clear();
//			DestroyListArea();
//			CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
//			GetTargets();
//			ActionFromButton(button);
//		
//		}
//
//		button = CombatButtons::NONE;
//
//		break;
//
//	case CombatButtons::ABILITY:
//
//		if (turnQueue.at(currentTurn)->GetStamina() >= staminaToAbility)
//		{
//			targets.Clear();
//			DestroyListArea();
//			CreateArea(characterTurn->Ab1Area, 2, characterTurn->tilePos);
//			GetTargets();
//			ActionFromButton(button);
//			abiltypressed = false;
//		}
//		
//		button = CombatButtons::NONE;
//
//		break;
//
//	case CombatButtons::MOVE:
//
//		if (turnQueue.at(currentTurn)->GetStamina() >= staminaToMove)
//		{
//			ActionFromButton(button);
//		}
//		UpdateTilesInRange(); //no estoy seguro si esto va aqui
//
//		break;
//
//	case CombatButtons::END:
//
//	
//		turnEnded = true;
//		characterTurn->GainStamina(10);
//		endturnpressed = false;
//
//		button = CombatButtons::NONE;
//		break;
//
//	case CombatButtons::NONE:
//
//		break;
//	}
//
//	
//}
//
//
//void BattleSystem::ActionFromButton(CombatButtons button)
//{
//
//	switch (button)
//	{
//	case CombatButtons::ATTACK:
//
//		DoDamageToEnemy(button);
//		
//		break;
//
//	case CombatButtons::ABILITY:
//
//		DoDamageToEnemy(button);
//
//		break;
//
//	case CombatButtons::MOVE:
//
//		button = CombatButtons::NONE;
//		break;
//	}
//}
//
//void BattleSystem::DoDamageToEnemy(CombatButtons button)
//{
//
//	DisplayArea((int)button);
//	DisplayEnemys();
//
//	for (int i = 0; i < targets.Count(); i++) {
//
//		if (targets.At(i)->data->tilePos == mouseTile) {
//
//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
//
//				targets.At(i)->data->health = targets.At(i)->data->health - (turnQueue.at(currentTurn)->attack - targets.At(i)->data->defense);
//
//				int stamina = StaminaToTakeAway(button);
//				turnQueue.at(currentTurn)->UseStamina(stamina);
//
//				turnEnded = true;
//				villager->takedmgAnim.Reset();
//
//				if (targets.At(i)->data->id == 3)
//				{
//					awchanim3 = true;
//					villager->takedmgAnim.Reset();
//				}
//			}
//		}
//	}
//
//	button = CombatButtons::NONE;
//
//}
//
//int BattleSystem::StaminaToTakeAway(CombatButtons button)
//{
//	int stamina = 0;
//	switch (button)
//	{
//	case CombatButtons::NONE:
//
//		break;
//	case CombatButtons::ATTACK:
//		stamina = staminaToAttack;
//		break;
//	case CombatButtons::ABILITY:
//		stamina = staminaToAbility;
//		break;
//	case CombatButtons::MOVE:
//		stamina = staminaToMove;
//
//		break;
//	case CombatButtons::END:
//
//		break;
//	default:
//		break;
//	}
//
//	return stamina;
//}
//
//void BattleSystem::PauseMenuAppear()
//{
//	// If player is in pause, close it
//	if (!isPaused)
//	{
//		app->uiModule->currentMenuType = COMBAT;
//		// Call this function only when scene is changed
//		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
//
//		isPaused = true;
//	}
//	// If player is NOT in pause, open it
//	else
//	{
//		app->uiModule->currentMenuType = COMBAT_PAUSE;
//		// Call this function only when scene is changed
//		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
//
//		isPaused = false;
//	}
//}
//
//void BattleSystem::GodMode()
//{
//	bunny->health = bunny->maxHealth;
//	timmy->health = timmy->maxHealth;
//	bunny->stamina = bunny->maxStamina;
//	timmy->stamina = timmy->maxStamina;
//}
//
//void BattleSystem::CheckWinCondition()
//{
//
//	if (timmy->health <= 0) {
//
//		timmy->isAlive = false;
//		timmy->takedmgAnim.Reset();
//	}
//
//	if (bunny->health <= 0) {
//
//		bunny->isAlive = false;
//		bunny->takedmgAnim.Reset();
//
//	}
//
//	if (bunny->health <= 0 && timmy->health <= 0) lose = true;
//
//	if (villager->health <= 0) {
//
//		villager->health = 0;
//		villager->isAlive = false;
//		villager->takedmgAnim.Reset();
//
//		win = true;
//
//	}
//
//}
//
//bool BattleSystem::OnGuiMouseClickEvent(GuiControl* control)
//{
//	LOG("Event by %d ", control->id);
//
//	switch (control->id)
//	{
//	case 16:
//		buttonPressed = CombatButtons::ATTACK;
//		break;
//	
//	case 17:
//		buttonPressed = CombatButtons::ABILITY;
//		break;
//
//	case 18:
//		buttonPressed = CombatButtons::MOVE;
//		break;
//
//	case 19:
//		buttonPressed = CombatButtons::END;
//		break;
//	}
//
//	return true;
//
//}
//
//// ----------------------------------- NEW -------------------------------
//
//bool BattleSystem::ChekRangeEnemy() {
//	for (int i = 0; i < targets.Count(); i++) {
//
//		for (int i = 0; i < area.Count(); i++) {
//			if (targets.At(i)->data->tilePos == iPoint(area.At(i)->data->x, area.At(i)->data->y)) {
//
//
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool BattleSystem::MoveEnemy() {
//
//
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 9; j++) {
//			if (combatMap[i][j].character == true) {
//				iPoint pos = iPoint(i, j);
//				if (app->pathfinding->IsWalkable(pos)) {
//
//					pos = app->map->MapToWorld(pos.x, pos.y);
//					app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
//					moveanim = true;
//				}
//			}
//
//		}
//
//	}
//
//	return true;
//
//}
//
//bool BattleSystem::Move(int pathindex, int length) {
//
//	iPoint dist;
//	fPoint pixelPosition;
//	fPoint finalPosition;
//	float distance;
//	b2Vec2 vel = b2Vec2(0, 0);
//
//	const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();
//
//
//	pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
//	pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;
//
//	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
//	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
//	LOG(" NextposX: %d", nextpos.x);
//	LOG(" NextposY: %d", nextpos.y);
//
//	dist.x = pixelPosition.x - characterTurn->position.x;
//	LOG(" disX: %d", dist.x);
//	dist.y = pixelPosition.y - characterTurn->position.y;
//	LOG(" disY: %d", dist.y);
//
//
//	xDir = 0;
//	yDir = 0;
//	xDir = (dist.x > 0) ? 1 : -1;
//	yDir = (dist.y > 0) ? 1 : -1;
//	if (dist.x == 0) {
//		xDir = 0;
//	}
//	if (dist.y == 0) {
//		yDir = 0;
//	}
//	if (dist.x == 0 && dist.y == 0) {
//		pathIndex++;
//
//	}
//	else if (abs(dist.x) > 0) {
//		vel.x = 5 * xDir;
//
//	}
//	else if (abs(dist.y) > 0) {
//		vel.y = 5 * yDir;
//
//	}
//
//	characterTurn->position.x = characterTurn->position.x + vel.x;
//	characterTurn->position.y = characterTurn->position.y + vel.y;
//
//	return true;
//}
//
//// Loads combat map from Map module using GID tile metadata
//bool BattleSystem::MakeCombatMap() {
//
//	bool ret = true;
//
//	for (int i = 0; i < 16; i++) {
//		for (int j = 0; j < 9; j++) {
//
//			combatMap[i][j].x = i;
//			combatMap[i][j].y = j;
//			combatMap[i][j].character = false;
//			combatMap[i][j].enemy = false;
//			combatMap[i][j].characterType = nullptr;
//			combatMap[i][j].inRange = false;
//			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
//		}
//	}
//
//	return ret;
//}
//
//bool BattleSystem::GetTargets() {
//
//
//	ListItem<TileDataa*>* tileListItem;
//	tileListItem = area.start;
//
//
//	while (tileListItem != NULL) {
//
//
//		iPoint pos = iPoint(tileListItem->data->x, tileListItem->data->y);
//
//		if (combatMap[pos.x][pos.y].enemy == true && characterTurn->isEnemy == false) {
//
//			targets.Add(combatMap[pos.x][pos.y].characterType);
//
//			std::cout << "posicion: " << pos.x << "x " << pos.y << "y" << std::endl;
//
//		}
//		else if (combatMap[pos.x][pos.y].character == true && characterTurn->isEnemy == true) {
//
//			targets.Add(combatMap[pos.x][pos.y].characterType);
//
//			std::cout << "posicion: " << pos.x << "x " << pos.y << "y" << std::endl;
//
//		}
//
//		tileListItem = tileListItem->next;
//	}
//
//	return true;
//}
//
//bool BattleSystem::DisplayEnemys() {
//
//
//
//	
//
//	for (ListItem<Entity*>* entitylist = targets.start; entitylist != NULL; entitylist = entitylist->next) {
//		iPoint pos = iPoint(entitylist->data->position.x, entitylist->data->position.y);
//
//		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 250, 0, 0, 100);
//
//		
//	}
//
//
//	return true;
//}
//
//bool BattleSystem::CreateArea(int range, int type, iPoint posTile) {
//
//
//
//
//	switch (type) {
//
//	case 0:
//		//attack
//		if (combatMap[posTile.x + 1][posTile.y].type == TILE_TYPE::FLOOR) {
//
//			area.Add(&combatMap[posTile.x + 1][posTile.y]);
//		}
//		if (combatMap[posTile.x - 1][posTile.y].type == TILE_TYPE::FLOOR) {
//			area.Add(&combatMap[posTile.x - 1][posTile.y]);
//		}
//		if (combatMap[posTile.x][posTile.y + 1].type == TILE_TYPE::FLOOR) {
//			area.Add(&combatMap[posTile.x][posTile.y + 1]);
//		}
//		if (combatMap[posTile.x][posTile.y - 1].type == TILE_TYPE::FLOOR) {
//			area.Add(&combatMap[posTile.x][posTile.y - 1]);
//		}
//		break;
//	case 1:
//		//lineal
//		for (int i = 1; i <= range; i++) {
//			if (combatMap[posTile.x + i][posTile.y].type == TILE_TYPE::FLOOR) {
//				area.Add(&combatMap[posTile.x + i][posTile.y]);
//			}
//			if (combatMap[posTile.x - i][posTile.y].type == TILE_TYPE::FLOOR) {
//				area.Add(&combatMap[posTile.x - i][posTile.y]);
//			}
//			if (combatMap[posTile.x][posTile.y + i].type == TILE_TYPE::FLOOR) {
//				area.Add(&combatMap[posTile.x][posTile.y + i]);
//			}
//			if (combatMap[posTile.x][posTile.y - i].type == TILE_TYPE::FLOOR) {
//				area.Add(&combatMap[posTile.x][posTile.y - i]);
//			}
//		}
//		break;
//	case 2:
//		//circular
//		int i;
//		int j;
//		for (i = 0; i <= range; i++) {
//			for (j = 0; j <= range - i; j++) {
//				if (combatMap[posTile.x + j][posTile.y + i].type == TILE_TYPE::FLOOR) {
//					area.Add(&combatMap[posTile.x + j][posTile.y + i]);
//				}
//				if (combatMap[posTile.x - j][posTile.y + i].type == TILE_TYPE::FLOOR) {
//					area.Add(&combatMap[posTile.x - j][posTile.y + i]);
//				}
//				if (combatMap[posTile.x - j][posTile.y - i].type == TILE_TYPE::FLOOR) {
//					area.Add(&combatMap[posTile.x - j][posTile.y - i]);
//				}
//				if (combatMap[posTile.x + j][posTile.y - i].type == TILE_TYPE::FLOOR) {
//					area.Add(&combatMap[posTile.x + j][posTile.y - i]);
//				}
//			}
//
//		}
//
//		break;
//	case 3:
//		//global
//		for (int i = 0; i < COMBAT_MAP_HEIGHT; i++) {
//			for (int j = 0; j < COMBAT_MAP_WIDTH; j++) {
//				iPoint pos = iPoint(i, j);
//				if (combatMap[j][i].type == TILE_TYPE::FLOOR) {
//
//					area.Add(&combatMap[j][i]);
//
//				}
//
//			}
//		}
//		break;
//	}
//
//	return true;
//
//}
//
//bool BattleSystem::DisplayArea(int type) {
//
//	bool ret = true;
//
//	ListItem<TileDataa*>* tileListItem;
//	tileListItem = area.start;
//
//	uint color[3];
//
//	switch (type)
//	{
//	case 0:
//		color[0] = 255;
//		color[1] = 0;
//		color[2] = 0;
//		break;
//	case 1:
//		color[0] = 0;
//		color[1] = 255;
//		color[2] = 0;
//		break;
//	case 2:
//		color[0] = 0;
//		color[1] = 0;
//		color[2] = 255;
//		break;
//	default:
//		break;
//	}
//
//	while (tileListItem != NULL) {
//
//
//		iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
//		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 0, 0, 250, 100);
//
//		tileListItem = tileListItem->next;
//	}
//
//	return ret;
//}
//
//
//
//// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
//bool BattleSystem::Combat(Entity* inturn, List<Entity*> target, int id) {
//
//	bool ret = true;
//
//	//id = 1 --> attack
//	if (id == 1) {
//		for (int i = 0; i < target.Count(); i++) {
//			target.At(i)->data->TakeDamage(inturn->Attack());
//		}
//	}
//	//id = 2 --> ability 1
//	if (id == 2) {
//		for (int i = 0; i < target.Count(); i++) {
//			if (inturn->Ab1Type != 3) {
//				target.At(i)->data->TakeDamage(inturn->Ability(1));
//			}
//			if (inturn->Ab1Type == 3) {
//				target.At(i)->data->TakeHealing(inturn->Ability(1));
//			}
//		}
//	}
//	//id = 3 --> ability 2
//	if (id == 3) {
//		for (int i = 0; i < target.Count(); i++) {
//			if (inturn->Ab1Type != 3) {
//				target.At(i)->data->TakeDamage(inturn->Ability(2));
//			}
//			if (inturn->Ab1Type == 3) {
//				target.At(i)->data->TakeHealing(inturn->Ability(2));
//			}
//		}
//	}
//	return ret;
//}
//
//void BattleSystem::DestroyListArea()
//{
//	//ListItem<TileDataa*>* item;
//	//int i = 0;
//	//for (item = area.start; item != NULL; item = item->next)
//	//{
//	//	if (item->data ==area.At(i)->data) area.Del(item);
//	//	i++;
//	//}
//
//	area.Clear();
//}
////Called before quitting
//bool BattleSystem::CleanUp()
//{
//	LOG("Freeing sceneBattle");
//	allentities.Clear();
//	area.Clear();
//	
//	targets.Clear();
//	app->pathfinding->ClearLastPath();
//	app->map->CleanUp();
//	app->entityManager->CleanUp();
//
//	//Unload textures
//	app->tex->UnLoad(mouseTileTex);
//	app->tex->UnLoad(originTex);
//	app->tex->UnLoad(timmytexture);
//	app->tex->UnLoad(bunnytexture);
//	app->tex->UnLoad(villagertexture);
//
//	app->tex->UnLoad(winScreen);
//	app->tex->UnLoad(loseScreen);
//
//
//	// ----------------------------------- NEW -------------------------------
//
//	turnQueue.clear();
//
//	return true;
//}
