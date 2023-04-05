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

#include "Timmy.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle(bool isActive) : Module(isActive)
{
	name.Create("scene");
}

// Destructor
SceneBattle::~SceneBattle()
{}

// Called before render is available
bool SceneBattle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load();

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


	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}


	mouseTileTex = app->tex->Load("Assets/Maps/Scenes/Path.png");

	originTex = app->tex->Load("Assets/Maps/Scenes/Cruz.png");

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

	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	////if (!DisplayArea()) ret = false;

	app->map->Draw();

	//

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,mouseY - app->render->camera.y);

	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile)) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);
		/*app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);*/
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
			if (originSelected == true)
			{
				if (app->pathfinding->IsWalkable(origin)) {
					app->pathfinding->CreatePath(origin, mouseTile);
					originSelected = false;
				}
				else {
					app->pathfinding->ClearLastPath();
				}
			}
			else
			{
					origin = mouseTile;
					if (app->pathfinding->IsWalkable(origin)) {
						originSelected = true;
						
					}
					app->pathfinding->ClearLastPath();
				
			}
		
		
	}


	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		
			app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	if (app->pathfinding->IsWalkable(origin)) {

	  app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	  app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}

	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	

	return true;
}

// Loads combat map from Map module using GID tile metadata
bool  SceneBattle::MakeCombatMap() {
	
	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = NULL;
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

	return true;
}
