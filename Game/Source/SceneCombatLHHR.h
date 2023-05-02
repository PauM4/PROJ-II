#ifndef __SCENE_COMBAT_LHHR_H__
#define __SCENE_COMBAT_LHHR_H__

#include "Module.h"
#include "Player.h"
#include "Timmy.h"
#include "Bunny.h"
#include "Enemy_LRRH.h"
#include "Enemy_CorruptedSprout.h"
#include "Entity.h"
#include "GuiButton.h"
#include "List.h"
#include "UIModule.h"


struct SDL_Texture;

enum class TILE_TYPEE {
	UNKNOWN = -1,
	BARRIER =4,
	FLOOR=2,
	HALF_BARRIER=3,
	MUD
};

struct TileData
{
	int x;
	int y;
	bool character;
	bool inRange;
	bool enemy;
	bool dead;
	Entity* characterType;
	TILE_TYPEE type;
};

class SceneCombatLHHR : public Module
{
public:

	SceneCombatLHHR(bool isActive);

	// Destructor
	virtual ~SceneCombatLHHR();

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

	bool ChekRangeEnemy();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool MoveEnemy();

	bool Move(int pathindex, int length);


	// Loads combat map from Map module using GID tile metadata
	bool MakeCombatMap();

	// Creates area of an attack or ability, type is 0 if atack, 1 if lineal, 2 if "circular", 3 if global
	bool CreateArea(int range, int type,iPoint posTile);

	

	// Draws an area of attack/ability/movement from an ally
	bool DisplayArea(int type);

	// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
	bool Combat(Entity* inturn, List<Entity*> target, int id);

	bool GetTargets();

	bool DisplayEnemys();

	bool GetTurns();

	bool GetNext();
	
	void DestroyListArea();


private:
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	// UI Things
	bool isPaused;

	TileData combatMap[16][9];
	bool movepressed;
	bool attackpressed;
	bool abiltypressed;
	bool endturnpressed;

	bool win, lose;

private:;

	SString mapName;
	SString mapFolder;

	TileData* selectedtile;

	Entity* characterTurn;
	List<Entity*> turnqueue;
	List<Entity*> allentities;
	List<Entity*> targets;
	List<TileData*> arealist;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
	SDL_Texture* timmytexture = nullptr;
	SDL_Texture* bunnytexture = nullptr;
	SDL_Texture* LRRHtexture = nullptr;
	SDL_Texture* sprouttexture = nullptr;

	SDL_Texture* winScreen = nullptr;
	SDL_Texture* loseScreen = nullptr;

	iPoint origin;
	bool originSelected = false;

	DynArray<iPoint> path;
	iPoint pos;
	iPoint mouseTile;
	
	List<TileData*> area;

	int  length;

    iPoint nextpos;
	int pathIndex;
	iPoint destination;
	bool move;
	bool moveenemy;
	bool atack;
	bool ability;
	bool turnstart;
	bool moveanim;
	bool godmode;

	uint value;

	int xDir;
	int yDir;
public:
	Timmy* timmy;
	Bunny* bunny;
	Enemy_LRRH* LRRH;
	Enemy_CorruptedSprout* sprout;

};

#endif // __SCENE_BATTLE_H__