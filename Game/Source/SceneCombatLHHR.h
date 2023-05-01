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



	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	


	

public:

	// UI Things


private:;

	SString mapName;
	SString mapFolder;

	





public:


};

#endif // __SCENE_BATTLE_H__