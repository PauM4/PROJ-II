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
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "Timmy.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle(bool isActive) : Module(isActive)
{
	name.Create("sceneBattle");
}

// Destructor
SceneBattle::~SceneBattle()
{}

// Called before render is available
bool SceneBattle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	//This reads some parameters from xml
	if (config.child("timmy")) {
		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
		timmy->parameters = config.child("timmy");
	}
	if (config.child("bunny")) {
		bunny = (Bunny*)app->entityManager->CreateEntity(EntityType::BUNNY);
		bunny->parameters = config.child("bunny");
	}
	if (config.child("enemy_angryVillager")) {
		villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
		villager->parameters = config.child("enemy_angryVillager");
	}
	//This reads stats from xml
	if (config.parent().child("timmy")) {
		timmy->stats = config.parent().child("timmy");
	}
	if (config.parent().child("bunny")) {
		bunny->stats = config.parent().child("bunny");
	}
	if (config.parent().child("enemy_angryVillager")) {
		villager->stats = config.parent().child("enemy_angryVillager");
	}
	app->entityManager->Awake(config);
	
	return ret;
}


// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load(mapName, mapFolder);
	move = false;

	pathIndex = 1;
	turnstart = false;
	pathIndex = 0;
	
	app->entityManager->Start(); 
	//Load combat map
	/*MakeCombatMap();*/

	//Buttons
	uint w, h;
	app->win->GetWindowSize(w, h);
	button1_attack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Attack", { (int)w - 1820, (int)h - 300, 100, 20 }, this);
	button1_attack->state = GuiControlState::NORMAL;
	button2_skill = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Skill", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button2_skill->state = GuiControlState::NORMAL;
	button3_endTurn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "End Turn", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button3_endTurn->state = GuiControlState::NORMAL;

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}


	mouseTileTex = app->tex->Load("Assets/Maps/Scenes/Path.png");

	originTex = app->tex->Load("Assets/Maps/Scenes/Cruz.png");

	MakeCombatMap();


	/*timmy->position = iPoint(670, 420);*/
	allentities.Add(timmy);
	allentities.Add(bunny);
	allentities.Add(villager);
	GetTurns();
	return true;
}


// Called each loop iteration
bool SceneBattle::PreUpdate()
{
	bool ret = true;
	return true;
}

// Called each loop iteration
bool SceneBattle::Update(float dt)
{
	if (turnstart == true) {
		//if user selects attack
		CreateArea(characterTurn,characterTurn->AttArea, 0);
		Combat(characterTurn, targets, 1);
		turnstart = false;
		//if user selects ab1
		CreateArea(characterTurn,characterTurn->Ab1Area, characterTurn->Ab1RangeType);
		Combat(characterTurn, targets, 2);
		turnstart = false;

		//if user selects ab2
		CreateArea(characterTurn,characterTurn->Ab2Area, characterTurn->Ab2RangeType);
		Combat(characterTurn, targets, 3);
		turnstart = false;
	}
	

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;

	
	app->guiManager->Draw();


	timmy->tilePos = app->map->WorldToMap(timmy->position.x - app->render->camera.x , timmy->position.y - app->render->camera.y);
	bunny->tilePos = app->map->WorldToMap(bunny->position.x - app->render->camera.x, bunny->position.y - app->render->camera.y);

	/*timmy->position = app->map->MapToWorld(timmy->tilePos.x, timmy->tilePos.y);*/
	
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,mouseY - app->render->camera.y);
	LOG("%d %d", mouseTile.x, mouseTile.y);
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x][mouseTile.y].character!=false && move == false) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
			if (originSelected == true)
			{
				if (app->pathfinding->IsWalkable(origin) && combatMap[mouseTile.x ][mouseTile.y].inRange == true && combatMap[mouseTile.x][mouseTile.y].character == false) {
					length=app->pathfinding->CreatePath(origin, mouseTile);
					destination.x = mouseTile.x;
					destination.y = mouseTile.y;
					originSelected = false;
					move = false;
				}
				else {
					app->pathfinding->ClearLastPath();
				}
			}
			else if(combatMap[mouseTile.x ][ mouseTile.y ].character != false)
			{
					origin = mouseTile;
					if (app->pathfinding->IsWalkable(origin)) {
						originSelected = true;
						move = true;
						characterTurn = combatMap[mouseTile.x][mouseTile.y].characterType;
					}
					app->pathfinding->ClearLastPath();
				
			}
	}


   const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();
	for (uint i = 0; i < lastpath->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastpath->At(i)->x, lastpath->At(i)->y);

		    LOG("posTileY= %d", lastpath->At(i)->y);
		
			app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	if (app->pathfinding->IsWalkable(origin) && originSelected == true) {

	  app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	  app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}

	
	


	if (pathIndex != length) {
		length = app->pathfinding->CreatePath(origin, destination);
	}
	else {
		length = 1;
		pathIndex = 1;
		app->pathfinding->ClearLastPath();
		
	}

	if (length > 1) {

		Move(characterTurn, pathIndex, length);
	}


	
	if (move == true) {
		int j = 0;
		int i = 0;
		iPoint nexTile;
		iPoint pos;
		for (i = 0; i < characterTurn->movement; i++ ) {
			for ( j = 0 ; j < characterTurn->movement-i; j++) {

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
				
				combatMap[nexTile.x][nexTile.y].inRange = true;
				/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				  
				 nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
				 combatMap[nexTile.x ][nexTile.y].inRange = true;
				/* arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

				 nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
				 combatMap[nexTile.x][nexTile.y].inRange = true;
				 /*arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

				 nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
				 combatMap[nexTile.x][nexTile.y].inRange = true;
				 /*arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/
			
			}
	
		}

		for (i = 0; i < characterTurn->movement; i++) {
			for (j = 0; j < characterTurn->movement - i; j++) {

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false &&
					app->pathfinding->IsWalkable(nexTile)) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}
				nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false &&
					app->pathfinding->IsWalkable(nexTile)) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

				nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false && 
					app->pathfinding->IsWalkable(nexTile) ) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false &&
					app->pathfinding->IsWalkable(nexTile)) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

			}

		}


		/*DisplayArea(arealist, 0);*/
		

	}
	else
	{
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 9; j++) {

				combatMap[i][j].characterType = nullptr;
				combatMap[i][j].character = false;
				combatMap[i][j].inRange = false;
				
				
			}
		}

		/*DestroyListArea(arealist);*/
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {
			if (combatMap[i][j].inRange==true && combatMap[i][j].character == false) {
				iPoint pos = iPoint(i, j);
				if (app->pathfinding->IsWalkable(pos)) {
					pos = app->map->MapToWorld(pos.x, pos.y);
				   app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
				}
			}

		}

	}

	

	combatMap[bunny->tilePos.x][bunny->tilePos.y].character = true;
	combatMap[bunny->tilePos.x][bunny->tilePos.y].characterType = bunny;

	app->render->DrawRectangle({ int(bunny->position.x)+35, int(bunny->position.y)+35, 50, 50 }, 0, 0, 250, 250, true);

	
	combatMap[timmy->tilePos.x][timmy->tilePos.y ].character = true;
	combatMap[timmy->tilePos.x][timmy->tilePos.y].characterType = timmy;
	
	app->render->DrawRectangle({ int(timmy->position.x)+35, int(timmy->position.y)+35, 50, 50 }, 250, 0, 0, 250, true);

	

	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 Attack click");
		
		break;
	case 2:
		LOG("Button 2 Skill click");
		
		break;
	case 3:
		LOG("Button 3 End Turn click");

		break;
	}
	return true;
}

bool SceneBattle::Move(Entity * character, int pathindex,int length) {

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
	LOG(" NextposX: %d", nextpos.x);
	LOG(" NextposY: %d", nextpos.y);

	dist.x = pixelPosition.x - character->position.x;
	LOG(" disX: %d", dist.x);
	dist.y = pixelPosition.y - character->position.y;
	LOG(" disY: %d", dist.y);



	int xDir = (dist.x > 0) ? 1 : -1;
	int yDir = (dist.y > 0) ? 1 : -1;

	if (dist.x == 0 && dist.y == 0) {
		pathIndex++;

	}
	else if (abs(dist.x) > 0) {
		vel.x = 10 * xDir;

	}
	else if (abs(dist.y) > 0) {
		vel.y = 10 * yDir;

	}

	character->position.x = character->position.x + vel.x;
	character->position.y = character->position.y + vel.y;

	return true;
}

// Loads combat map from Map module using GID tile metadata
bool SceneBattle::MakeCombatMap() {
	
	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = false;
			combatMap[i][j].characterType = nullptr;
			combatMap[i][j].inRange = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
}
bool SceneBattle::GetTurns() {
	if (allentities.At(0)->data->speed >= allentities.At(1)->data->speed && allentities.At(0)->data->speed >= allentities.At(2)->data->speed)
	{
		characterTurn = allentities.At(0)->data;
		turnqueue.Add(allentities.At(0)->data);
	}
	if (allentities.At(1)->data->speed >= allentities.At(0)->data->speed && allentities.At(1)->data->speed >= allentities.At(2)->data->speed)
	{
		characterTurn = allentities.At(1)->data;
		turnqueue.Add(allentities.At(1)->data);
	}
	if (allentities.At(2)->data->speed >= allentities.At(0)->data->speed && allentities.At(2)->data->speed >= allentities.At(1)->data->speed)
	{
		characterTurn = allentities.At(2)->data;
		turnqueue.Add(allentities.At(2)->data);
	}
	if (allentities.At(1)->data->speed > allentities.At(2)->data->speed)
	{
		turnqueue.Add(allentities.At(1)->data);
		turnqueue.Add(allentities.At(2)->data);
	}
	if (allentities.At(2)->data->speed > allentities.At(1)->data->speed)
	{
		turnqueue.Add(allentities.At(2)->data);
		turnqueue.Add(allentities.At(1)->data);
	}
	return true;
}

bool SceneBattle::GetNext() {

	Entity* temp;
	temp = turnqueue.At(0)->data;
	turnqueue.At(0)->data = turnqueue.At(1)->data;
	turnqueue.At(1)->data = turnqueue.At(2)->data;
	turnqueue.At(2)->data = temp;
	characterTurn = turnqueue.At(0)->data;
	return true;

}
List<TileData*> SceneBattle::CreateArea(Entity*character, int range, int type) {

	List<TileData*> area;
	iPoint posTile = character->tilePos;
	
	switch (type) {

	case 0:
		//attack
		
		area.Add(&combatMap[posTile.x+1][posTile.y]);
		area.Add(&combatMap[posTile.x-1][posTile.y]);
		area.Add(&combatMap[posTile.x][posTile.y+1]);
		area.Add(&combatMap[posTile.x][posTile.y-1]);
		break;
	case 1:
		//lineal
		for (int i = 1; i <= range; i++) {
			area.Add(&combatMap[posTile.x + i][posTile.y]);
			area.Add(&combatMap[posTile.x - i][posTile.y]);
			area.Add(&combatMap[posTile.x][posTile.y + i]);
			area.Add(&combatMap[posTile.x][posTile.y - i]);
		}
		break;
	case 2:
		//circular
		int i;
		int j;
		for (i = 0; i < range; i++) {
			for (j = 0; j < range - i; j++) {
				area.Add(&combatMap[posTile.x + i][posTile.y+j]);
				area.Add(&combatMap[posTile.x -i][posTile.y+j]);
				area.Add(&combatMap[posTile.x + i][posTile.y + j]);
				area.Add(&combatMap[posTile.x +i][posTile.y - j]);
			}

		}

		break;
	case 3:
		//global
		for (int i = 0; i < COMBAT_MAP_HEIGHT; i++) {
			for (int j = 0; j < COMBAT_MAP_WIDTH; j++) {
				if (combatMap[j][i].type != TILE_TYPE::BARRIER && combatMap[j][i].type != TILE_TYPE::HALF_BARRIER) {
				
					area.Add(&combatMap[j][i]);

				}
			}
		}
		break;
	}

	return area;

}

bool SceneBattle::DisplayArea(List<TileData*> area, int type) {

	bool ret = true;

	ListItem<TileData*>* tileListItem;
	tileListItem = area.start;

	uint color[3];

	switch (type)
	{
	case 0:
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		break;
	case 1:
		color[0] = 0;
		color[1] = 255;
		color[2] = 0;
		break;
	case 2:
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

// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
bool SceneBattle::Combat(Entity* inturn, List<Entity*> target, int id) {
	
	bool ret = true;

	//id = 1 --> attack
	if (id == 1) {
		for (int i = 0; i++; i > target.Count()) {
			target.At(i)->data->TakeDamage(inturn->Attack());
		}
	}
	//id = 2 --> ability 1
	if (id == 2) {
		for (int i = 0; i++; i > target.Count()) {
			if (inturn->Ab1Type != 3) {
				target.At(i)->data->TakeDamage(inturn->Ability(1));
			}
			if (inturn->Ab1Type == 3) {
				target.At(i)->data->TakeHealing(inturn->Ability(1));
			}
		}
	}
	//id = 3 --> ability 2
	if (id == 3) {
		for (int i = 0; i++; i > target.Count()) {
			if (inturn->Ab1Type != 3) {
				target.At(i)->data->TakeDamage(inturn->Ability(2));
			}
			if (inturn->Ab1Type == 3) {
				target.At(i)->data->TakeHealing(inturn->Ability(2));
			}
		}
	}
	return ret;
}
void SceneBattle::DestroyListArea(List<TileData*> arealist)
{
	ListItem<TileData*>* item;
	int i = 0;
	for (item = arealist.start; item != NULL; item = item->next)
	{
		if (item->data ==arealist.At(i)->data) arealist.Del(item);
		i++;
	}
}
//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");
	app->map->CleanUp();
	app->entityManager->CleanUp(); 
	return true;
}
