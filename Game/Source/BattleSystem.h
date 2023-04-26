#ifndef __BATTLE_SYSTEM_H__
#define __BATTLE_SYSTEM_H__
#include "Module.h"
#include "Player.h"
#include "Timmy.h"
#include "Bunny.h"
#include "Enemy_AngryVillager.h"
#include "Entity.h"
#include "GuiButton.h"
#include "List.h"
#include "UIModule.h"




enum class TILE_TYPE {
	UNKNOWN = -1,
	BARRIER = 4,
	FLOOR = 2,
	HALF_BARRIER = 3,
	MUD
};

struct TileDataa
{
	int x;
	int y;
	bool character;
	bool inRange;
	bool enemy;
	bool dead;
	Entity* characterType;
	TILE_TYPE type;
};

enum class CombatButtons
{
	NONE,
	ATTACK,
	ABILITY,
	MOVE,
	END,
};

//enum class Action
//{
//	ATTACK,
//	ABILITY,
//	MOVE,
//
//};

class BattleSystem : public Module
{
public:

	BattleSystem(bool isActive);

	// Destructor
	virtual ~BattleSystem();

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
	bool CreateArea(int range, int type, iPoint posTile);



	// Draws an area of attack/ability/movement from an ally
	bool DisplayArea(int type);

	// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
	bool Combat(Entity* inturn, List<Entity*> target, int id);

	bool GetTargets();

	bool DisplayEnemys();

	bool GetTurns();

	bool GetNext();

	void DestroyListArea();

public:

	// UI Things
	bool isPaused;

	TileDataa combatMap[16][9];
	bool movepressed;
	bool attackpressed;
	bool abiltypressed;
	bool endturnpressed;
	bool godMode;
	bool win, lose;

private:

	SString mapName;
	SString mapFolder;

	TileDataa* selectedtile;

	Entity* characterTurn;
	
	List<Entity*> allentities;
	List<Entity*> targets;
	List<TileDataa*> arealist;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
	SDL_Texture* timmytexture = nullptr;
	SDL_Texture* bunnytexture = nullptr;
	SDL_Texture* villagertexture = nullptr;

	SDL_Texture* winScreen = nullptr;
	SDL_Texture* loseScreen = nullptr;

	iPoint origin;
	bool originSelected = false;

	DynArray<iPoint> path;
	iPoint pos;
	iPoint mouseTile;

	List<TileDataa*> area;

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
	bool awchanim1;
	bool awchanim2;
	bool awchanim3;

	uint value;

public:

	Timmy* timmy;
	Bunny* bunny;
	Enemy_AngryVillager* villager;

private:

	int xDir;
	int yDir;






	// ----------------------------------- NEW -------------------------------

private:
	int currentTurn;

	std::vector<Entity*> turnQueue;

	//Cuando jugador pase turno o el turno de una entity acabe, turnEnded = true
	bool turnEnded; 


	int staminaToAttack, staminaToMove, staminaToAbility;

public:
	CombatButtons buttonPressed;
	//Action actionToDo;

private:

	//Update
	void PauseMenuAppear();
	void TriggerButtonPressed(CombatButtons button);
	void ActionFromButton(CombatButtons button);
	void GodMode();
	void UpdateEntitiesTilePos();
	void UpdateTilesInRange();
	
	// Creates area of an attack or ability, type is 0 if atack, 1 if lineal, 2 if "circular", 3 if global
	bool CreateArea(int range, int type, iPoint posTile);

	// Draws an area of attack/ability/movement from an ally
	bool DisplayArea(int type);

	void CheckWinCondition();

	void DoDamageToEnemy(CombatButtons button);

	int StaminaToTakeAway(CombatButtons button);

	//PostUpdate
	void UIStatsForBattle();
	const char* UintToChar(uint param);

	


};



#endif // __BATTLE_SYSTEM_H__