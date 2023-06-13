#ifndef __BATTLE_SCENE_WOLF_H__
#define __BATTLE_SCENE_WOLF_H__

#include "Module.h"
#include "Entity.h"
#include "GuiButton.h"
#include "List.h"
#include "UIModule.h"
#include "Enemy_Wolf.h"
#include "Enemy_FinalWolf.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class BattleScene_Wolf : public Module
{
public:

	BattleScene_Wolf(bool isActive);

	// Destructor
	virtual ~BattleScene_Wolf();

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

	Enemy_Wolf* wolf;
	iPoint wolfPrevPos;
	Enemy_FinalWolf* finalwolf;
	iPoint finalwolfPrevPos;

	int frames = 0;
	
public:
	bool conditionToRangeChecker;
	bool noStaminaToMove;
	
};

#endif // __BATTLE_SCENE_WOLF_H__