#ifndef __BATTLE_SCENE_PIGS_H__
#define __BATTLE_SCENE_PIGS_H__

#include "Module.h"
#include "Entity.h"
#include "GuiButton.h"
#include "List.h"
#include "UIModule.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class BattleScene_Pigs : public Module
{
public:

	BattleScene_Pigs(bool isActive);

	// Destructor
	virtual ~BattleScene_Pigs();

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

	void SaveResult();

	void RunTree();

	void UpdateAnimation(const char* name);

	void MoveAnimation(const char* name);

	void TakeDamageAnimation(const char* name);

	void UpdatePosForAnimation(const char* name, iPoint tilePos);

private:

	SString mapName;
	SString mapFolder;

	/*Timmy* timmy;
	Enemy_AngryVillager* villager;
	Bunny* bunny;*/

	iPoint timmyPrevPos, bunnyPrevPos, villagerPrevPos;
	bool timmyPrevPosBool, bunnyPrevPosBool, villagerPrevPosBool;

	int frames = 0;
	
public:
	bool conditionToRangeChecker;
	bool noStaminaToMove;
	
};

#endif // __BATTLE_SCENE_PIGS_H__