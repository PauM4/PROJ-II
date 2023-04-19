#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "UIModule.h"
#include "SceneManager.h"
#include "Fonts.h"
#include "SceneBattle.h"
#include "SceneMainMenu.h"
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


	mainmenu_play_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { 920, 600, 120,30 }, this);
	mainmenu_options_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { 920, 640, 120,30 }, this);
	mainmenu_credits_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { 920, 680, 120,30 }, this);
	mainmenu_quit_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Quit", { 920, 720, 120, 30 }, this);
	mainmenu_newGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "New Game", { 920, 640, 120,30 }, this);
	mainmenu_continueGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Continue", { 920, 680, 120,30 }, this);
	mainmenu_return_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Return", { 920, 925, 120,30 }, this);
	//2049, 1794
	pausemenu_resume_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "Resume", { 1620, 80, 120,30 }, this);
	pausemenu_save_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Save", { 1620, 115, 120,30 }, this);
	pausemenu_load_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 22, "Load", { 1620, 150, 120,30 }, this);
	pausemenu_options_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Options", { 1620, 185, 120,30 }, this);
	pausemenu_return_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "Return", { 1620, 80, 120,30 }, this);
	pausemenu_backtomain_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Back to Menu", { 1620, 220, 120,30 }, this);
	pausemenu_quit_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Quit", { 1620, 255, 120, 30 }, this);

	combat_attack_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "Attack", { 100, 780, 100, 30 }, this);
	combat_ability_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "Ability", { 100, 815, 100, 30 }, this);
	combat_move_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "Move", { 100, 850, 100, 30 }, this);
	combat_endTurn_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "End Turn", { 100, 885, 100, 30 }, this);

	dialog_option1_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "", { 100, 900, 800, 30 }, app->scene);
	dialog_option2_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "", { 100, 950, 800, 30 }, app->scene);
	dialog_option3_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "", { 1000, 900, 800, 30 }, app->scene);
	dialog_option4_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "", { 1000, 950, 800, 30 }, app->scene);
	dialog_text_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 150, "", { 100, 700, 1700, 150 }, this);

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
	dialog_text_button->state = GuiControlState::NONE;

	quitButtonBool = false;
	continueBool = false;

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

	if (app->scene->active) {
		if (app->scene->player->playerState == app->scene->player->PlayerState::NPC_INTERACT)
		{
			PrintDialogue(app->scene->GetDialogue());
			if (app->scene->player->dialogueActivate)
			{
				app->scene->AppearDialogue();
				app->scene->player->dialogueActivate = false;
			}
		}
	}
	

	return ret;
}


// Called before quitting
bool UIModule::CleanUp()
{
	LOG("Freeing UIModule");
	app->guiManager->guiControlsList.Clear();

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

		app->sceneMainMenu->creditsOpen = true;
		break;
		// When quit pressed, quit the game
	case 4:
		// Close app
		quitButtonBool = true;
		break;
		// New Game. Start the game with all original config.xml info
	case 5:
		app->scene->isNewGame = true;
		app->sceneManager->isBattle = false;
		app->sceneManager->scene = SCENE;
		break;
		// When continue pressed, go to gameplay
	case 6:
		continueBool = true;
		app->sceneManager->isBattle = false;
		app->sceneManager->scene = SCENE;
		/*app->LoadGameRequest();*/
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

		app->sceneMainMenu->creditsOpen = false;
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
		app->sceneBattle->attackpressed = true;
		app->sceneBattle->abiltypressed = false;

		app->sceneBattle->movepressed = false;

		app->sceneBattle->endturnpressed = false;

		break;
		// Ability
	case 17:
		app->sceneBattle->attackpressed = false;
		app->sceneBattle->abiltypressed = true;

		app->sceneBattle->movepressed = false;

		app->sceneBattle->endturnpressed = false;
		break;
		// Move
	case 18:

		app->sceneBattle->attackpressed = false;
		app->sceneBattle->abiltypressed = false;

		app->sceneBattle->movepressed = true;

		app->sceneBattle->endturnpressed = false;
		break;
		// End turn
	case 19:
		app->sceneBattle->attackpressed = false;
		app->sceneBattle->abiltypressed = false;

		app->sceneBattle->movepressed = false;

		app->sceneBattle->endturnpressed = true;
		break;
	}

	// Dialog Options Switch
	switch (control->id)
	{
		// Option 1
	case 12:
		std::cout << "a"<<std::endl;
		break;
		// Option 2
	case 13:
		std::cout << "a" << std::endl;
		break;
		// Option 3
	case 14:
		std::cout << "a" << std::endl;
		break;
		// Option 4
	case 15:
		std::cout << "a" << std::endl;
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
		dialog_text_button->state = GuiControlState::NONE;

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
		dialog_text_button->state = GuiControlState::NONE;

		// Disable other menus buttons:

		break;
	case DIALOG:
		// Activate dialog buttonts
		// Maybe there would be another switch for each dialog, idk how it will work
		dialog_option1_button->state = GuiControlState::NORMAL;
		dialog_option2_button->state = GuiControlState::NORMAL;
		dialog_option3_button->state = GuiControlState::NORMAL;
		dialog_option4_button->state = GuiControlState::NORMAL;
		dialog_text_button->state = GuiControlState::NORMAL;

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
		dialog_text_button->state = GuiControlState::NONE;

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
		dialog_text_button->state = GuiControlState::NONE;

		// Disable other menus buttons:

		break;
	}

	return true;
}


void UIModule::PrintDialogue(std::vector<std::string> dialogue)
{

	int posX = app->scene->player->position.x;
	int posY = app->scene->player->position.y;

	// Dialogue text block
	SDL_Rect rect = { 0 , 0, 800, 400 };
	SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, dialogue[0].c_str(), { 255,255,255,255 }, 1700);
	app->render->DrawTexture(textDialogue, posX - 850, posY + 220, NULL);

	// Change options buttons text
	SDL_Rect rectO1 = { 0, 0, 800, 30 };
	SDL_Rect rectO2 = { 0, 0, 800, 30 };
	SDL_Rect rectO3 = { 0, 0, 800, 30 };
	SDL_Rect rectO4 = { 0, 0, 800, 30 };

	
	// Check if there's dialogue available
	if (!(dialogue.size() <= 1))
	{
		SDL_Texture* textOption1 = app->fonts->LoadRenderedParagraph(rectO1, app->fonts->gameFont, dialogue[1].c_str(), { 255,255,255,255 }, rectO1.w);
		app->render->DrawTexture(textOption1, posX - 850, posY + 405, NULL);
		SDL_DestroyTexture(textOption1);
	}
	else
	{
		dialog_option1_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 2))
	{
		SDL_Texture* textOption2 = app->fonts->LoadRenderedParagraph(rectO2, app->fonts->gameFont, dialogue[2].c_str(), { 255,255,255,255 }, rectO2.w);
		app->render->DrawTexture(textOption2, posX - 850, posY + 455, NULL);
		SDL_DestroyTexture(textOption2);
	}
	else
	{
		dialog_option2_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 3))
	{
		SDL_Texture* textOption3 = app->fonts->LoadRenderedParagraph(rectO3, app->fonts->gameFont, dialogue[3].c_str(), { 255,255,255,255 }, rectO3.w);
		app->render->DrawTexture(textOption3, posX + 60, posY + 405, NULL);
		SDL_DestroyTexture(textOption3);
	}
	else
	{
		dialog_option3_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 3))
	{
		SDL_Texture* textOption4 = app->fonts->LoadRenderedParagraph(rectO4, app->fonts->gameFont, dialogue[4].c_str(), { 255,255,255,255 }, rectO4.w);
		app->render->DrawTexture(textOption4, posX + 60, posY + 455, NULL);
		SDL_DestroyTexture(textOption4);
	}
	else
	{
		dialog_option4_button->state = GuiControlState::NONE;
	}


	SDL_DestroyTexture(textDialogue);

	

	



}