#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "UIModule.h"
#include "SceneManager.h"
#include <iostream>

#include "Defs.h"
#include "Log.h"

UIModule::UIModule(bool isActive) : Module(isActive)
{
	name.Create("UIModule");
}

// Destructor
UIModule::~UIModule()
{}

// Called before render is available
bool UIModule::Awake(pugi::xml_node& config)
{
	LOG("Loading UIModule");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UIModule::Start()
{
	currentMenuType = DISABLED;

	uint w, h;
	app->win->GetWindowSize(w, h);


	mainmenu_play_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { (int)w - 860, (int)h - 1000, 100, 20 }, this);
	mainmenu_options_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { (int)w - 860, (int)h - 975, 100, 20 }, this);
	mainmenu_credits_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { (int)w - 860, (int)h - 950, 100, 20 }, this);
	mainmenu_quit_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Quit", { (int)w - 860, (int)h - 925, 100, 20 }, this);
	mainmenu_newGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "New Game", { (int)w - 860, (int)h - 975, 100, 20 }, this);
	mainmenu_continueGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Continue", { (int)w - 860, (int)h - 950, 100, 20 }, this);
	mainmenu_return_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Return", { (int)w - 860, (int)h - 925, 100, 20 }, this);

	pausemenu_resume_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "Resume", { (int)w - 300, (int)h - 1000, 100, 20 }, this);
	pausemenu_save_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Save", { (int)w - 300, (int)h - 975, 100, 20 }, this);
	pausemenu_load_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 22, "Load", { (int)w - 300, (int)h - 950, 100, 20 }, this);
	pausemenu_options_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Options", { (int)w - 300, (int)h - 925, 100, 20 }, this);
	pausemenu_return_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "Return", { (int)w - 300, (int)h - 1000, 100, 20 }, this);
	pausemenu_backtomain_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Back to Menu", { (int)w - 300, (int)h - 900, 100, 20 }, this);
	pausemenu_quit_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Quit", { (int)w - 300, (int)h - 875, 100, 20 }, this);

	combat_attack_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "Attack", { (int)w - 1800, (int)h - 300, 100, 20 }, this);
	combat_ability_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "Ability", { (int)w - 1800, (int)h - 275, 100, 20 }, this);
	combat_move_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "Move", { (int)w - 1800, (int)h - 250, 100, 20 }, this);
	combat_endTurn_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "End Turn", { (int)w - 1800, (int)h - 225, 100, 20 }, this);

	dialog_option1_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Option 1", { 1580, 850, 100, 20 }, this);
	dialog_option2_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Option 2", { 1580, 885, 100, 20 }, this);
	dialog_option3_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Option 3", { 1580, 920, 100, 20 }, this);
	dialog_option4_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "Option 4", { 1580, 955, 100, 20 }, this);

	// When creating a new button, iniciate it in NONE state

	mainmenu_play_button->state = GuiControlState::NONE;
	mainmenu_options_button->state = GuiControlState::NONE;
	mainmenu_credits_button->state = GuiControlState::NONE;
	mainmenu_quit_button->state = GuiControlState::NONE;
	mainmenu_newGame_button->state = GuiControlState::NONE;
	mainmenu_continueGame_button->state = GuiControlState::NONE;
	mainmenu_return_button->state = GuiControlState::NONE;

	pausemenu_resume_button->state = GuiControlState::NONE;
	pausemenu_save_button->state = GuiControlState::NONE;
	pausemenu_options_button->state = GuiControlState::NONE;
	pausemenu_return_button->state = GuiControlState::NONE;
	pausemenu_load_button->state = GuiControlState::NONE;
	pausemenu_backtomain_button->state = GuiControlState::NONE;
	pausemenu_quit_button->state = GuiControlState::NONE;

	combat_attack_button->state = GuiControlState::NONE;
	combat_ability_button->state = GuiControlState::NONE;
	combat_move_button->state = GuiControlState::NONE;
	combat_endTurn_button->state = GuiControlState::NONE;

	dialog_option1_button->state = GuiControlState::NONE;
	dialog_option2_button->state = GuiControlState::NONE;
	dialog_option3_button->state = GuiControlState::NONE;
	dialog_option4_button->state = GuiControlState::NONE;

	quitButtonBool = false;

	return true;
}

// Called each loop iteration
bool UIModule::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool UIModule::Update(float dt)
{

	// If Quit button is pressed in Main Menu or Pause, close app
	if (quitButtonBool)
	{
		return false;
	}

	return true;
}

// Called each loop iteration
bool UIModule::PostUpdate()
{
	bool ret = true;

	app->guiManager->Draw();

	return ret;
}


// Called before quitting
bool UIModule::CleanUp()
{
	LOG("Freeing UIModule");

	return true;
}

bool UIModule::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	// Main Menu Switch
	switch (control->id)
	{
		// When button Play Is Pressed, show the Continue, New Game & Return buttons
	case 1:
		std::cout << "PLAY PRESSED" << std::endl;
		mainmenu_continueGame_button->state = GuiControlState::NORMAL;
		mainmenu_newGame_button->state = GuiControlState::NORMAL;
		mainmenu_return_button->state = GuiControlState::NORMAL;

		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;

		break;
		// When options pressed, go to options (image with settings)
	case 2:
		mainmenu_return_button->state = GuiControlState::NORMAL;

		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;


		break;

		// When credits pressed, appear image with credits and return button
	case 3:
		mainmenu_return_button->state = GuiControlState::NORMAL;

		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		break;
		// When quit pressed, quit the game
	case 4:
		// Close app
		quitButtonBool = true;
		break;
		// New Game. Start the game with all original config.xml info
	case 5:

		app->scene->player->position.x = app->scene->player->parameters.attribute("x").as_int();
		app->scene->player->position.y = app->scene->player->parameters.attribute("y").as_int();

		app->sceneManager->isBattle = false;
		app->sceneManager->scene = SCENE;
		break;
		// When continue pressed, go to gameplay
	case 6:
		app->LoadGameRequest();
		app->sceneManager->isBattle = false;
		app->sceneManager->scene = SCENE;
		break;

		// When button Return Is Pressed, show the Main Menu buttons
	case 7:
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;

		mainmenu_play_button->state = GuiControlState::NORMAL;
		mainmenu_credits_button->state = GuiControlState::NORMAL;
		mainmenu_options_button->state = GuiControlState::NORMAL;
		mainmenu_quit_button->state = GuiControlState::NORMAL;
		break;
	}

	// Pause Menu Switch
	switch (control->id)
	{
		// Resume
	case 20:
		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;
		pausemenu_return_button->state = GuiControlState::NONE;

		app->scene->player->playerState = app->scene->player->playerPrevState;

		break;
		// Save
	case 8:
		app->SaveGameRequest();
		break;
		//Load
	case 22:
		app->LoadGameRequest();
		break;
		// When options pressed, go to options (image with settings)
	case 9:
		pausemenu_return_button->state = GuiControlState::NORMAL;

		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;

		break;
		// Return pressed --> return from options to pause menu
	case 23:
		pausemenu_resume_button->state = GuiControlState::NORMAL;
		pausemenu_save_button->state = GuiControlState::NORMAL;
		pausemenu_options_button->state = GuiControlState::NORMAL;
		pausemenu_load_button->state = GuiControlState::NORMAL;
		pausemenu_backtomain_button->state = GuiControlState::NORMAL;
		pausemenu_quit_button->state = GuiControlState::NORMAL;

		pausemenu_return_button->state = GuiControlState::NONE;
		break;
		//Back to Main Menu
	case 10:

		app->sceneManager->isBattle = false;
		app->sceneManager->scene = MAIN_MENU;

		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = MAIN;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		break;
		//Quit the game
	case 11:
		quitButtonBool = true;
		break;
	}

	// Combat Menu Switch
	switch (control->id)
	{
		// Attack
	case 16:
		break;
		// Ability
	case 17:
		break;
		// Move
	case 18:
		break;
		// End turn
	case 19:
		break;
	}

	// Dialog Options Switch
	switch (control->id)
	{
		// Option 1
	case 12:
		break;
		// Option 2
	case 13:
		break;
		// Option 3
	case 14:
		break;
		// Option 4
	case 15:
		break;
	}

	return true;
}


// Change buttons state depending on the scene
bool UIModule::ChangeButtonState(int& currentMenuType)
{
	switch (currentMenuType)
	{
	case MAIN:
		// Activate main menu buttons
		mainmenu_play_button->state = GuiControlState::NORMAL;
		mainmenu_options_button->state = GuiControlState::NORMAL;
		mainmenu_credits_button->state = GuiControlState::NORMAL;
		mainmenu_quit_button->state = GuiControlState::NORMAL;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;
		//...

		// Disable other menu buttons
		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;

		// Disable all combat buttons
		combat_attack_button->state = GuiControlState::NONE;
		combat_ability_button->state = GuiControlState::NONE;
		combat_move_button->state = GuiControlState::NONE;
		combat_endTurn_button->state = GuiControlState::NONE;

		// Disable all dialog buttons
		dialog_option1_button->state = GuiControlState::NONE;
		dialog_option2_button->state = GuiControlState::NONE;
		dialog_option3_button->state = GuiControlState::NONE;
		dialog_option4_button->state = GuiControlState::NONE;

		break;
	case PAUSE:

		// Activate pause buttons
		pausemenu_resume_button->state = GuiControlState::NORMAL;
		pausemenu_save_button->state = GuiControlState::NORMAL;
		pausemenu_load_button->state = GuiControlState::NORMAL;
		pausemenu_options_button->state = GuiControlState::NORMAL;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NORMAL;
		pausemenu_quit_button->state = GuiControlState::NORMAL;

		// Disable all main menu buttons
		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;

		// Disable all combat buttons
		combat_attack_button->state = GuiControlState::NONE;
		combat_ability_button->state = GuiControlState::NONE;
		combat_move_button->state = GuiControlState::NONE;
		combat_endTurn_button->state = GuiControlState::NONE;

		// Disable all dialog buttons
		dialog_option1_button->state = GuiControlState::NONE;
		dialog_option2_button->state = GuiControlState::NONE;
		dialog_option3_button->state = GuiControlState::NONE;
		dialog_option4_button->state = GuiControlState::NONE;

		// Disable other menus buttons:

		break;
	case DIALOG:
		// Activate dialog buttonts
		// Maybe there would be another switch for each dialog, idk how it will work
		dialog_option1_button->state = GuiControlState::NORMAL;
		dialog_option2_button->state = GuiControlState::NORMAL;
		dialog_option3_button->state = GuiControlState::NORMAL;
		dialog_option4_button->state = GuiControlState::NORMAL;

		// Disable all main menu buttons
		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;

		// Disable all pause menu buttons
		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;

		// Disable all combat buttons
		combat_attack_button->state = GuiControlState::NONE;
		combat_ability_button->state = GuiControlState::NONE;
		combat_move_button->state = GuiControlState::NONE;
		combat_endTurn_button->state = GuiControlState::NONE;

		// Disable other menus buttons:

		break;
	case COMBAT:
		// Activate combat buttons
		combat_attack_button->state = GuiControlState::NORMAL;
		combat_ability_button->state = GuiControlState::NORMAL;
		combat_move_button->state = GuiControlState::NORMAL;
		combat_endTurn_button->state = GuiControlState::NORMAL;

		// Disable all main menu buttons
		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;

		// Disable pause menu buttons just in case
		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;

		// Disable all dialog buttons
		dialog_option1_button->state = GuiControlState::NONE;
		dialog_option2_button->state = GuiControlState::NONE;
		dialog_option3_button->state = GuiControlState::NONE;
		dialog_option4_button->state = GuiControlState::NONE;

		// Disable other menus buttons:


		break;
	case DISABLED:

		//...

		// Disable all main menu buttons
		mainmenu_play_button->state = GuiControlState::NONE;
		mainmenu_options_button->state = GuiControlState::NONE;
		mainmenu_credits_button->state = GuiControlState::NONE;
		mainmenu_quit_button->state = GuiControlState::NONE;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;

		// Disable all pause menu buttons
		pausemenu_resume_button->state = GuiControlState::NONE;
		pausemenu_save_button->state = GuiControlState::NONE;
		pausemenu_load_button->state = GuiControlState::NONE;
		pausemenu_options_button->state = GuiControlState::NONE;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NONE;
		pausemenu_quit_button->state = GuiControlState::NONE;

		// Disable all combat buttons
		combat_attack_button->state = GuiControlState::NONE;
		combat_ability_button->state = GuiControlState::NONE;
		combat_move_button->state = GuiControlState::NONE;
		combat_endTurn_button->state = GuiControlState::NONE;

		// Disable all dialog buttons
		dialog_option1_button->state = GuiControlState::NONE;
		dialog_option2_button->state = GuiControlState::NONE;
		dialog_option3_button->state = GuiControlState::NONE;
		dialog_option4_button->state = GuiControlState::NONE;

		// Disable other menus buttons:

		break;
	}

	return true;
}
