#ifndef __SCENE_BATTLE_H__
#define __SCENE_BATTLE_H__

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "List.h"

struct SDL_Texture;

enum class TILE_TYPE {
	UNKNOWN = -1,
	BARRIER,
	FLOOR,
	HALF_BARRIER,
	MUD
};

struct TileData
{
	int x;
	int y;
	Entity* character;
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
	bool MakeCombatMap();

	// Draws an area of attack/ability/movement from an ally
	bool DisplayArea(List<TileData*> area, int type);

public:

	TileData combatMap[16][9];

private:;

	TileData* selectedtile;

	Entity* characterTurn;

	List<Entity*> targets;

	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;


	iPoint origin;
	bool originSelected = false;
};

#endif // __SCENE_BATTLE_H__