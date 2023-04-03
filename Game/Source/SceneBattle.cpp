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
#include "Playable.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle() : Module()
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

	Playable timmy;
	Playable villager;
	timmy.level = 1;
	timmy.health = 20;
	timmy.maxhealth = 20;
	timmy.defense = 5;
	timmy.magic = 1;
	timmy.stamina = 15;
	timmy.maxstamina = 15;
	timmy.speed=5;
	timmy.attack = 6;
	timmy.AttArea = iPoint(1, 1);
	timmy.Ab1Type = 1;
	timmy.Ab1Area = iPoint(1, 3);
	timmy.Ab1Power = 2;
	timmy.Ab2Type = 1;
	timmy.Ab2Area = iPoint(1, 3);
	timmy.Ab2Power = 3;
	timmy.healingpower = 0;

	//Load combat map
	MakeCombatMap();

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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//if (!DisplayArea()) ret = false;

	app->map->Draw();

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

//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");

	return true;
}
