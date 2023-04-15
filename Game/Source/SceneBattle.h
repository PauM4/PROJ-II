#ifndef __SCENE_BATTLE_H__
#define __SCENE_BATTLE_H__

#include "Module.h"
#include "Player.h"
#include "Timmy.h"
#include "Bunny.h"
#include "Enemy_AngryVillager.h"
#include "Entity.h"
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
	bool character;
	bool inRange;
	Entity* characterType;
	TILE_TYPE type;
};

class SceneBattle : public Module
{
public:

	SceneBattle(bool isActive);

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

	bool Move(TileData* tile);

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool Move(Entity* character, int pathindex, int length);

	bool Move();

	// Loads combat map from Map module using GID tile metadata
	bool MakeCombatMap();

	// Creates area of an attack or ability, type is 0 if atack, 1 if lineal, 2 if "circular", 3 if global
	List<TileData*> CreateArea(int range, int type);

	// Draws an area of attack/ability/movement from an ally
	bool DisplayArea(List<TileData*> area, int type);

	// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
	bool Combat(Entity* inturn, List<Entity*> target, int id);

	bool GetTurns();

	bool GetNext();
	void DestroyListArea(List<TileData*> arealist);




public:

	TileData combatMap[16][9];
	

private:;

	SString mapName;
	SString mapFolder;

	TileData* selectedtile;

	Entity* characterTurn;
	List<Entity*> turnqueue;
	List<Entity*> allentities;
	List<Entity*> targets;
	List<TileData*> arealist;

	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;


	iPoint origin;
	bool originSelected = false;

	//Buttons
	GuiButton* button1_attack;
	GuiButton* button2_skill;
	GuiButton* button3_endTurn;
	DynArray<iPoint> path;
	iPoint pos;
	

	int  length;

    iPoint nextpos;
	int pathIndex;
	iPoint destination;
	bool move;

	bool turnstart;
	
	Timmy* timmy;
	Bunny* bunny;
	Enemy_AngryVillager* villager;
	PhysBody* pbody;
};

#endif // __SCENE_BATTLE_H__