#ifndef __W2_SCENE_MAZE_H__
#define __W2_SCENE_MAZE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "NpcW3.h"
#include "Portal.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"

struct SDL_Texture;

class W2_Scene_Maze : public Module
{
public:

	W2_Scene_Maze(bool isActive);

	// Destructor
	virtual ~W2_Scene_Maze();

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

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

private:

	void Camera();
	void GodMode();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);


public:

	Player* player;

	List<Door*> doors;


	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;

	SDL_Texture* uiSpriteTexture;

	SDL_Texture* fog;

	Portal* portal;

private:

	SString mapName;
	SString mapFolder;


	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	//Fonts
	int font;

	bool pauseMenuActive;
	bool exitButtonBool;

	bool godMode;


};

#endif // __W2_SCENE_MAZE_H__