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

	return ret;
}

// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load(mapName, mapFolder);

	pathIndex = 0;

	/*Entity* timmy = new Timmy();*/
	//Entity* bunny;
	//Entity villager;
	//timmy.level = 1;
	//timmy.health = 20;
	//timmy.maxhealth = 20;
	//timmy.defense = 5;
	//timmy.magic = 1;
	//timmy.stamina = 15;
	//timmy.maxstamina = 15;
	//timmy.speed=5;
	//timmy.attack = 6;
	//timmy.AttArea = iPoint(1, 1);
	//timmy.Ab1Type = 1;
	//timmy.Ab1Area = iPoint(1, 3);
	//timmy.Ab1Power = 2;
	//timmy.Ab2Type = 1;
	//timmy.Ab2Area = iPoint(1, 3);
	//timmy.Ab2Power = 3;
	//timmy.healingpower = 0;

	/*villager.level = 1;
	villager.health = 20;
	villager.maxhealth = 20;
	villager.defense = 5;
	villager.magic = 1;
	villager.stamina = 15;
	villager.maxstamina = 15;
	villager.speed = 5;
	villager.attack = 6;
	villager.AttArea = iPoint(1, 1);
	villager.Ab1Type = 1;
	villager.Ab1Area = iPoint(1, 3);
	villager.Ab1Power = 2;
	villager.Ab2Type = 1;
	villager.Ab2Area = iPoint(1, 3);
	villager.Ab2Power = 3;
	villager.healingpower = 0;*/


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

	pbody = app->physics->CreateRectangle(620, 500, 120, 140, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = timmy;

	

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
	
	bool ret = true;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;

	b2Vec2 vel = b2Vec2(0, 0);

	app->guiManager->Draw();

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	////if (!DisplayArea()) ret = false;

	app->map->Draw();

	//
	iPoint posTile = iPoint(0, 0);
	posTile = app->map->WorldToMap(pos.x - app->render->camera.x, pos.y - app->render->camera.y);

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,mouseY - app->render->camera.y);
	/*LOG("%d %d", mouseTile.x-4, mouseTile.y-1);*/
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x-4][mouseTile.y-1].character!=false) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);
		/*app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);*/
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
			if (originSelected == true)
			{
				if (app->pathfinding->IsWalkable(origin)) {
					length=app->pathfinding->CreatePath(origin, mouseTile);
					destination = mouseTile;
					originSelected = false;
				}
				else {
					app->pathfinding->ClearLastPath();
				}
			}
			else if(combatMap[mouseTile.x - 4][ mouseTile.y - 1].character != false)
			{
					origin = mouseTile;
					if (app->pathfinding->IsWalkable(origin)) {
						originSelected = true;
						
					}
					app->pathfinding->ClearLastPath();
				
			}
	}


   const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();
	for (uint i = 0; i < lastpath->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastpath->At(i)->x, lastpath->At(i)->y);
		
			app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	if (app->pathfinding->IsWalkable(origin)) {

	  app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	  app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}


	//int j = 0;
	//for (int i = 0; i < path.Count(); i++)
	//{
	//	if (path[i] != (*lastpath)[j])
	//		
	//	j++;
	//	
	//}
	//path.Clear();
	//for (int i = 0; i < lastpath->Count(); i++)
	//{
	//	path.PushBack((*lastpath)[i]);
	//}
	//if (j < path.Count())
	//	pathIndex = j;
	//else
	//	pathIndex = 0;


	/*fPoint pixelPosition;
	fPoint finalPosition;
	float distance;

	
		
	
	pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
	pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;

	finalPosition.x= lastpath->At(length-1)->x * app->map->mapData.tileWidth;
	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
	if (finalPosition.DistanceTo(pos) == 0) {
		pathIndex = 0;
	}

	distance = pixelPosition.DistanceTo(pos);



	if (distance == 0)
	{
		pathIndex++;
		vel.y = 0;
		vel.x = 0;
	}
	else
	{
		float xDiff = pixelPosition.x - pos.x;
		float yDiff = pixelPosition.y - pos.y;

	


		if (abs(xDiff) > abs(yDiff))
		{
			int xDir = (xDiff > 0) ? 1 : -1;
			if (abs(xDiff) < abs(xDir * 10))
			{
				vel.x += xDiff;
				vel.y = 0;
			}
			else
			{
				vel.x = 10 * xDir;
				vel.y = 0;
			}
		}
		else
		{
			int yDir = (yDiff > 0) ? 1 : -1;
			if (abs(yDiff) < abs(yDir *  10))
			{
				vel.y += yDiff;
				vel.x = 0;
			}
			else
			{
				vel.y += yDir * 10;
				vel.x = 0;
			}

		}
	}*/

	



	length = app->pathfinding->CreatePath(origin, destination);
	iPoint dist;
	iPoint finaldist;
	if (length > 1) {
		pos = app->map->WorldToMap(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y));
		lastpath = app->pathfinding->GetLastPath();
		nextpos = lastpath->At(pathIndex);
		LOG(" Nextpos: %d", nextpos);
		dist.x = nextpos->x - pos.x;
		LOG(" disX: %d", dist.x);
		dist.y = nextpos->y - pos.y;
		LOG(" disY: %d", dist.y);

		finaldist.x = pos.x - destination.x;
		finaldist.y = pos.y - destination.y;

		if (finaldist.x == 0 && finaldist.y == 0) {
			pathIndex=0;
		}
		else if (dist.x==0 || dist.y ==0) {
			pathIndex++;
		}
		else if (dist.x > 0) {
			vel.x = 10;

		}
		else if (dist.x < 0) {
			vel.x = -10;

		}
		else if (dist.y > 0) {
			vel.y = 10;

		}
		else if (dist.y < 0) {
			vel.y = -10;

		}
	}
	else {
		pathIndex = 0;
	}




	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	pbody->body->SetLinearVelocity(vel);

	pos.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	pos.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	/*LOG("%d %d", posTile.x-4, posTile.y);*/
	combatMap[posTile.x-4][ posTile.y ].character = true;
	
	app->render->DrawRectangle({ int(pos.x), int(pos.y), 110, 140 }, 250, 0, 0, 250, true);

	

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

// Loads combat map from Map module using GID tile metadata
bool  SceneBattle::MakeCombatMap() {
	
	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
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
		app->render->DrawRectangle({ tileListItem->data->x,tileListItem->data->y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, color[0], color[1], color[2], 100);

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

//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");
	app->map->CleanUp(); 
	return true;
}
