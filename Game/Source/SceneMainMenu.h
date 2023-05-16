#ifndef __SCENEMAINMENU_H__
#define __SCENEMAINMENU_H__

#include "Module.h"



struct SDL_Texture;

class SceneMainMenu : public Module
{
public:

	SceneMainMenu(bool isActive);

	// Destructor
	virtual ~SceneMainMenu();

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
	
	SDL_Texture* mainMenu_image;
	SDL_Texture* mainMenu_image_tittle;
	SDL_Texture* UI_spritesheet_final;
	SDL_Texture* mainMenuRipped_image;
	SDL_Texture* credits_image;

	SDL_Rect press_enterRect;

	//Tween tittleAnim;

	bool creditsOpen;

	// Window width and height
	int w, h;

	// Bool to transition between ripped and not in main menu
	bool returnPressed;

private:

};

#endif // __SCENEMAINMENU_H__