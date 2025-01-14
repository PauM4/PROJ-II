#ifndef __SCENE_BATTLE_H__
#define __SCENE_BATTLE_H__

#include "Module.h"
#include "Entity.h"
#include "GuiButton.h"
#include "List.h"
#include "UIModule.h"
#include "Enemy_AngryVillager.h"
#include "Timmy.h"

struct SDL_Texture;

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

	// Called before quitting
	bool CleanUp();

	void SaveResult();


private:

	SString mapName;
	SString mapFolder;

	Timmy* timmy;
	Enemy_AngryVillager* villager;


	
	
};

#endif // __SCENE_BATTLE_H__