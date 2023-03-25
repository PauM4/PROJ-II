#ifndef __SCENE_BATTLE_H__
#define __SCENE_BATTLE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiButton.h"
#include "List.h"

struct SDL_Texture;

enum class TILE_TYPE {
	UNKNOWN,
	FLOOR = 0,
	MUD,
	BARRIER,
	HALF_BARRIER
};

struct TileData
{
	int x;
	int y;
	TILE_TYPE type;
};

class SceneBattle : public Module
{
public:

	SceneBattle();

	// Destructor
	virtual ~SceneBattle();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Loads combat map from Map module using GID tile metadata
	bool LoadCombatMap();

public:



private:;

	TileData combatMap[12][9];

	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

};

#endif // __SCENE_BATTLE_H__