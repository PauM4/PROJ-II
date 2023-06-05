#ifndef __BATTLE_MANAGER_H__
#define __BATTLE_MANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "UIModule.h"
#include "GuiButton.h"
#include "TeamManager.h"
#include"SDL_Timer.h"

enum class TILE_TYPE {
	UNKNOWN = -1,
	BARRIER = 4,
	FLOOR = 2,
	HALF_BARRIER = 3,
	MUD
};

struct TileData {
	int x;
	int y;
	Entity* character = nullptr;
	bool inRange;
	bool isCharacter;
	TILE_TYPE type;
};

enum class BattleState {
	UNKNOWN = -1,
	THINKING,
	SELCETED,
	INACTION,
	ENEMY,
	WIN,
	LOSE

};

enum class ActionType {
	UNKNOWN = -1,
	MOVE,
	ATTACK,
	ABILITY,
	END_TURN
};

enum class CombatButtons
{
	NONE,
	ATTACK,
	ABILITY,
	MOVE,
	END,
};

class BattleManager : public Module {
public:

	BattleManager(bool active);

	// Destructor
	virtual ~BattleManager();

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

	void DrawResult();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void UIStatsForBattle();

	// Called before quitting
	bool CleanUp();

public:

	Entity* GetCurrentTurn();

public:

	//Adds a playable Entity to allies/enenmies list and combatMap
	bool AddCharacter(Entity* character, int x, int y, bool isEnemy); //Better way of identifing if its an enemy

	//Fills turnList with entities from allies/emenies list and rearrenges them by turn priority
	bool MakeTurnList();

	bool DisplayEnemys();

	//Updates TurnList and currentTurn
	bool UpdateTurnList();

	//Displays the turn order in battle
	bool DisplayTurnList();

	//Displays the area of effect of an action
	bool DisplayAtackArea(ActionType type);

	bool DisplayMoveArea(ActionType type);

	//Apply action type from character to all targets
	bool ApplyAction(Entity* character, ActionType type);

	void CheckWinCondition();

	void LiveCondition();

	void PauseMenuAppear();

	void GodMode();

	bool IaEnemyAttack();

	void TriggerAIAttack();

	bool IaEnemyMove();

	bool CreateArea(int range, int type, iPoint posTile);

	// Loads combat map from Map module using GID tile metadata
	bool MakeCombatMap();

	void UpdateCombatMap();

	void UpdateEntitiesTilePos();

	//Sets the actionArea from a character, type idicates the type of action
	bool GetActionArea(Entity* character, ActionType type);

	bool MousePosition();

	bool Move(int pathindex, int length);

	//Fills the target list depending of action
	bool SelectTargets();

	void TriggerButtonPressed(CombatButtons button);

	//Checks if either all the entities in allies or enemies are dead
	//and updates battle state
	BattleState CheckState();

public:

	iPoint targetPosForAnimation;
	iPoint targetPosForAIAnimation;

public:
	TileData combatMap[16][9];

	List<Entity*> allies;
	List<Entity*> enemies;

	List<Entity*> targets;

	Entity* currentTurn;
	List<Entity*> turnList;

	TileData* selectedTile;
	List<TileData*> actionArea;
	List<TileData*> area;
	ActionType actionType;
	CombatButtons buttonPressed;
	BattleState battleState;

	SDL_Texture* winScreen = nullptr;
	SDL_Texture* loseScreen = nullptr;

	bool godMode;
	bool isPaused;
	bool win;
	bool lose;
	bool actionfinish;
	SDL_Timer enemyAreaTimer;
	SDL_Timer enemyAttackTimer;
	bool triggerMoveTimer;
	bool displayEnemyAttackAreaOnce = false;

	int changeScreenTimer;


	//Pathfinding varibles
	iPoint origin;
	int length;
	iPoint mouseTile;
	iPoint destination;
	bool moveanim;
	int xDir;
	int yDir;
	int pathIndex;

	//SFX
	int deathFx = -1;
	int rechargemanaFx = -1;
	int moveFx = -1;
	int victoryFx = -1;
	int looseFx = -1;

	Tween lose_screen_animation;
	Tween win_screen_animation;
};




#endif // __BATTLE_MANAGER_H__
