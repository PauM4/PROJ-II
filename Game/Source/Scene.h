#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"

#include "GuiButton.h"
#include "Item.h"
#include "Npc.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	Player* player;


	Npc* npc1;

	Item* item1;

private:
	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	//Fonts
	int font;

	// L15: DONE 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* button1_continue;
	GuiButton* button2_exit;

	bool pauseMenuActive;
	bool exitButtonBool;


};

#endif // __SCENE_H__