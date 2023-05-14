#ifndef __SCENEGRANDMA_H__
#define __SCENEGRANDMA_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "Npc.h"
#include "Portal.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"

struct SDL_Texture;

class SceneGrandma : public Module
{
public:

	SceneGrandma(bool isActive);

	// Destructor
	virtual ~SceneGrandma();

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


public:

	Player* player;
	Door* door;
	Portal* portal; 
	Npc* npc1;

	// UI Things
	bool isPaused;

private:

	SString mapName;
	SString mapFolder;
};

#endif // __SCENEGRANDMA_H__