#ifndef __UIMODULE_H__
#define __UIMODULE_H__

#include "Module.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "SceneManager.h"


struct SDL_Texture;

enum CurrentMenuType
{
	MAIN,
	PAUSE,
	DIALOG,
	COMBAT,
	DISABLED
};

class UIModule : public Module
{
public:

	UIModule(bool isActive);

	// Destructor
	virtual ~UIModule();

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

	// Change buttons state depending on the scene
	bool ChangeButtonState(int& currentMenuType);


public:

	int currentMenuType;
	int previousMenuType;

	// Buttons list
	GuiButton* mainmenu_play_button;
	GuiButton* mainmenu_options_button;
	GuiButton* mainmenu_credits_button;
	GuiButton* mainmenu_quit_button;
	GuiButton* mainmenu_newGame_button;
	GuiButton* mainmenu_continueGame_button;
	GuiButton* mainmenu_return_button;

	bool quitButtonBool;

private:

};

#endif // __UIMODULE_H__