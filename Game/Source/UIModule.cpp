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
#include "SceneCombatLHHR.h"
#include "BattleManager.h"
#include "SceneMainMenu.h"
#include "Scene.h"
#include "W2_Scene.h"
#include "Player.h"
#include "Npc.h"
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

	levelUpFX = app->audio->LoadFx("Assets/Sounds/FX/fx_level_up.wav");

	uint w, h;
	app->win->GetWindowSize(w, h);

	mainmenu_play_button =		   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { 920, 600, 120,30 }, this);
	mainmenu_options_button =	   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { 920, 640, 120,30 }, this);
	mainmenu_credits_button =	   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { 920, 680, 120,30 }, this);
	mainmenu_quit_button =		   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Quit", { 920, 720, 120, 30 }, this);
	mainmenu_newGame_button =	   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "New Game", { 920, 640, 120,30 }, this);
	mainmenu_continueGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Continue", { 920, 680, 120,30 }, this);
	mainmenu_return_button =	   (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Return", { 920, 925, 120,30 }, this);

	pausemenu_resume_button =	  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "Resume", { 1620, 80, 120,30 }, this);
	pausemenu_inventory_button =  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 21, "Inventory", { 1620, 115, 120,30 }, this);
	pausemenu_party_button =	  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 24, "Party", { 1620, 150, 120,30 }, this);
	pausemenu_save_button =		  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Save", { 1620, 185, 120,30 }, this);
	pausemenu_load_button =		  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 22, "Load", { 1620, 220, 120,30 }, this);
	pausemenu_options_button =	  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Options", { 1620, 255, 120,30 }, this);
	pausemenu_return_button =	  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "Return", { 1620, 80, 120,30 }, this);
	pausemenu_backtomain_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Main Menu", { 1620, 290, 120,30 }, this);
	pausemenu_quit_button =		  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Quit", { 1620, 325, 120, 30 }, this);

	pausemenuCombat_resume_button =		(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 100, "Resume", { 1620, 80, 120,30 }, this);
	pausemenuCombat_options_button =	(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 101, "Options", { 1620, 115, 120,30 }, this);
	pausemenuCombat_backtomain_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 104, "Main Menu", { 1620, 150, 120,30 }, this);
	pausemenuCombat_return_button =		(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 102, "Return", { 1620, 80, 120,30 }, this);
	pausemenuCombat_quit_button =		(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 103, "Quit", { 1620, 255, 120, 30 }, this);

	combat_attack_button =  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "Attack", { 100, 780, 100, 30 }, app->battleManager);
	combat_ability_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "Ability", { 100, 815, 100, 30 }, app->battleManager);
	combat_move_button =	(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "Move", { 100, 850, 100, 30 }, app->battleManager);
	combat_endTurn_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "End Turn", { 100, 885, 100, 30 }, app->battleManager);

	dialog_option1_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "", { 100, 900, 800, 30 }, app->scene);
	dialog_option2_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "", { 100, 950, 800, 30 }, app->scene);
	dialog_option3_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "", { 1000, 900, 800, 30 }, app->scene);
	dialog_option4_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "", { 1000, 950, 800, 30 }, app->scene);

	dialog2_option1_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 30, "", { 100, 900, 800, 30 }, app->w2_scene);
	dialog2_option2_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 31, "", { 100, 950, 800, 30 }, app->w2_scene);
	dialog2_option3_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 32, "", { 1000, 900, 800, 30 }, app->w2_scene);
	dialog2_option4_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 33, "", { 1000, 950, 800, 30 }, app->w2_scene);

	levelup_defenseUp_button =		(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 60, "+", { 900, 400, 100, 30 }, this);
	levelup_magicUp_button =		(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 61, "+", { 900, 450, 100, 30 }, this);
	levelup_speedUp_button =        (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 62, "+", { 900, 500, 100, 30 }, this);
	levelup_movementUp_button =     (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 63, "+", { 900, 550, 100, 30 }, this);
	levelup_attackUp_button =       (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 64, "+", { 900, 600, 100, 30 }, this);
	levelup_AB1PowerUp_button =     (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 65, "+", { 900, 650, 100, 30 }, this);
	levelup_healingpowerUp_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 66, "+", { 900, 700, 100, 30 }, this);

	party_timmy_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 67, "Unavailable", { 1000, 400, 100, 30 }, this);
	party_bunny_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 68, "Unavailable", { 1000, 450, 100, 30 }, this);
	party_lrrh_button =  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 69, "Unavailable", { 1000, 500, 100, 30 }, this);
	party_Lpig_button =  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 70, "Unavailable", { 1000, 550, 100, 30 }, this);
	party_Mpig_button =  (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 71, "Unavailable", { 1000, 600, 100, 30 }, this);
	party_peter_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 72, "Unavailable", { 1000, 650, 100, 30 }, this);

	AddButtonsToList();

	// When creating a new button, iniciate it in NONE state

	DisableButtonsToNone();

	doorPlayerPosition = false;

	quitButtonBool = false;
	continueBool = false;

	dialogueHasChanged = false;
	indexDialogueOverTime = 0;
	timeToRefreshDialogue = 0.01f; 

	
	return true;
}

void UIModule::DisableButtonsToNone()
{
	for (int i = 0; i < buttonsList.Count(); ++i)
	{
		GuiButton* button = buttonsList[i];
		button->state = GuiControlState::NONE;

	}
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

	// Pergami fons level up screen
	if (app->teamManager->active && app->teamManager->lvlupbool)
	{
		if (app->scene->active)
		{
			app->render->DrawTexture(app->scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
		if (app->w2_scene->active)
		{
			app->render->DrawTexture(app->w2_scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
		app->teamManager->PrintLvlUpText();
	}

	if (currentMenuType == INVENTORY)
	{
		if (app->scene->active)
		{
			app->render->DrawTexture(app->scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
		if (app->w2_scene->active)
		{
			app->render->DrawTexture(app->w2_scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
		app->fonts->DrawText("INVENTORY", 640, 150, 100, 100, { 255, 255, 255 }, app->fonts->gameFontBig, true);
	}

	if (currentMenuType == PARTY)
	{
		if (app->scene->active)
		{
			app->render->DrawTexture(app->scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
		if (app->w2_scene->active)
		{
			app->render->DrawTexture(app->w2_scene->lvlupTexture, -app->render->camera.x, -app->render->camera.y - 200);
		}
			app->fonts->DrawText("PARTY", 800, 150, 100, 100, { 255, 255, 255 }, app->fonts->gameFontBig, true);
	}

	app->guiManager->Draw();

	// SCENE 1 UI
	if (app->scene->active) {
		if (app->scene->player->playerState == app->scene->player->PlayerState::NPC_INTERACT && currentMenuType != ROPE_MINIGAME)
		{
			PrintDialogue(app->scene->GetDialogue());
			if (app->scene->player->dialogueActivate)
			{
				app->scene->AppearDialogue();

				app->scene->player->dialogueActivate = false;
			}
		}


		// print UI that is on top of the SCENE SCREEN here
		
		//UI Minigame
		// Condition for minigame
		if (app->scene->minigameActive)
		{
			app->render->DrawTexture(app->scene->ropeTexture, app->scene->ropeX, app->scene->ropeY, &app->scene->ropeRect);
			app->render->DrawTexture(app->scene->pressKeyTexture, -app->render->camera.x + 400, -app->render->camera.y + 50, &app->scene->keyRect);
		}
		else
		{
			// Tutorial Textures here (on top of quest)
			if (app->scene->basicTutorialCounter == 0)
			{
				app->render->DrawTexture(app->scene->moveTutorialTextutre, -app->render->camera.x, -app->render->camera.y);
			}
			else if (app->scene->basicTutorialCounter == 1)
			{
				app->render->DrawTexture(app->scene->interactTutorialTextutre, -app->render->camera.x, -app->render->camera.y);
			}
			else // If no longer tutorial, print Quest
			{
				// UI Quest
				if (app->scene->player->playerState == app->scene->player->PlayerState::MOVING && !app->teamManager->lvlupbool)
				{
					app->render->DrawTexture(app->scene->questUiTexture, -app->render->camera.x + 30, -app->render->camera.y + 30, NULL);
					app->scene->drawQuest(-app->render->camera.x + 120, -app->render->camera.y + 120);
				}
			}
		}

	}

	//... check for others scenes if they are loaded before calling them here because
	// this module starts before any other scene

	// SCENE 2 UI
	if (app->w2_scene->active) {
		if (app->w2_scene->player->playerState == app->w2_scene->player->PlayerState::NPC_INTERACT)
		{
			PrintDialogue2(app->w2_scene->GetDialogue());
			if (app->w2_scene->player->dialogueActivate)
			{
				app->w2_scene->AppearDialogue();

				app->w2_scene->player->dialogueActivate = false;
			}
		}

		// print UI that is on top of the w2_scene SCREEN here

		// UI Quest
		if (app->w2_scene->player->playerState == app->w2_scene->player->PlayerState::MOVING && !app->teamManager->lvlupbool)
		{
			app->render->DrawTexture(app->w2_scene->questUiTexture, -app->render->camera.x + 30, -app->render->camera.y + 30, NULL);
			app->w2_scene->drawQuest(-app->render->camera.x + 120, -app->render->camera.y + 120);
		}

	}


	return ret;
}


// Called before quitting
bool UIModule::CleanUp()
{
	LOG("Freeing UIModule");
	app->guiManager->guiControlsList.Clear();
	buttonsList.Clear();


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
		app->LoadGameRequest();
		continueBool = true;
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
		pausemenu_inventory_button->state = GuiControlState::NONE;
		pausemenu_party_button->state = GuiControlState::NONE;

		if(app->scene->active) app->scene->player->playerState = app->scene->player->playerPrevState;
		if(app->w2_scene->active) app->w2_scene->player->playerState = app->w2_scene->player->playerPrevState;

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
		pausemenu_inventory_button->state = GuiControlState::NONE;
		pausemenu_party_button->state = GuiControlState::NONE;

		break;
		// Return pressed --> return from options or Inventory or Party to pause menu
	case 23:
		if (currentMenuType == INVENTORY || currentMenuType == PARTY)
		{
			app->teamManager->UpdateParty();

			// Tell to UIModule which currentMenuType
			app->uiModule->currentMenuType = PAUSE;
			// Call this function only when buttons change
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		}

		pausemenu_resume_button->state = GuiControlState::NORMAL;
		pausemenu_save_button->state = GuiControlState::NORMAL;
		pausemenu_options_button->state = GuiControlState::NORMAL;
		pausemenu_load_button->state = GuiControlState::NORMAL;
		pausemenu_backtomain_button->state = GuiControlState::NORMAL;
		pausemenu_quit_button->state = GuiControlState::NORMAL;
		pausemenu_inventory_button->state = GuiControlState::NORMAL;
		pausemenu_party_button->state = GuiControlState::NORMAL;

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

		// Inventory
	case 21:

		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = INVENTORY;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		break;

		// Party
	case 24:

		CheckPartyTextOnStart();

		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = PARTY;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		break;
	}

	// Pause Menu COMBAT Switch
	switch (control->id)
	{
		// Resume
	case 100:
		pausemenuCombat_resume_button->state = GuiControlState::NONE;
		pausemenuCombat_options_button->state = GuiControlState::NONE;
		pausemenuCombat_quit_button->state = GuiControlState::NONE;
		pausemenuCombat_backtomain_button->state = GuiControlState::NONE;
		pausemenuCombat_return_button->state = GuiControlState::NONE;

		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = COMBAT;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		break;
		// Options
	case 101:
		pausemenuCombat_return_button->state = GuiControlState::NORMAL;

		pausemenuCombat_backtomain_button->state = GuiControlState::NONE;
		pausemenuCombat_resume_button->state = GuiControlState::NONE;
		pausemenuCombat_options_button->state = GuiControlState::NONE;
		pausemenuCombat_quit_button->state = GuiControlState::NONE;

		break;
		// Return pressed --> return from options to pause menu
	case 102:
		pausemenuCombat_resume_button->state = GuiControlState::NORMAL;
		pausemenuCombat_options_button->state = GuiControlState::NORMAL;
		pausemenuCombat_quit_button->state = GuiControlState::NORMAL;
		pausemenuCombat_backtomain_button->state = GuiControlState::NORMAL;

		pausemenuCombat_return_button->state = GuiControlState::NONE;
		break;

		// Quit game
	case 103:
		quitButtonBool = true;
		break;

		// Back to main menu
	case 104:
		app->sceneManager->isBattle = false;
		app->sceneManager->scene = MAIN_MENU;

		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = MAIN;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
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

	// Dialog2 Options Switch
	switch (control->id)
	{
		// Option 1
	case 30:
		std::cout << "a" << std::endl;
		break;
		// Option 2
	case 31:
		std::cout << "a" << std::endl;
		break;
		// Option 3
	case 32:
		std::cout << "a" << std::endl;
		break;
		// Option 4
	case 33:
		std::cout << "a" << std::endl;
		break;
	}

	// Level Up Stats
	switch (control->id)
	{
	case 60:
		app->teamManager->defenseup = true;
		break;
	case 61:
		app->teamManager->magicup = true;
		break;
	case 62:
		app->teamManager->speedup = true;
		break;
	case 63:
		app->teamManager->movementup = true;
		break;
	case 64:
		app->teamManager->attackup = true;
		break;
	case 65:
		app->teamManager->Ab1Powerup = true;
		break;
	case 66:
		app->teamManager->healingpowerup = true;
		break;
	}

	// Party Buttons
	switch (control->id)
	{
		// Timmy
	case 67:
		if (app->teamManager->istimmyplayable)
		{
			if (app->teamManager->IsTimmyOnTeam)
			{
				app->teamManager->IsTimmyOnTeam = false;
				party_timmy_button->text = "-";
			}
			else
			{
				app->teamManager->IsTimmyOnTeam = true;
				party_timmy_button->text = "In Party";
			}
		}
		else
		{
			party_timmy_button->text = "Unavailable";
		}
		break;

		// Bunny
	case 68:
		if (app->teamManager->isbunnyplayable)
		{
			if (app->teamManager->IsBunnyOnTeam)
			{
				app->teamManager->IsBunnyOnTeam = false;
				party_bunny_button->text = "-";
			}
			else
			{
				app->teamManager->IsBunnyOnTeam = true;
				party_bunny_button->text = "In Party";
			}
		}
		else
		{
			party_bunny_button->text = "Unavailable";
		}
		break;

		// LRRH
	case 69:
		if (app->teamManager->islrrhplayable)
		{
			if (app->teamManager->IsLrrhOnTeam)
			{
				app->teamManager->IsLrrhOnTeam = false;
				party_lrrh_button->text = "-";
			}
			else
			{
				app->teamManager->IsLrrhOnTeam = true;
				party_lrrh_button->text = "In Party";
			}
		}
		else
		{
			party_lrrh_button->text = "Unavailable";
		}
		break;

		// LPig
	case 70:
		if (app->teamManager->islilpigplayable)
		{
			if (app->teamManager->IsLilPigOnTeam)
			{
				app->teamManager->IsLilPigOnTeam = false;
				party_Lpig_button->text = "-";
			}
			else
			{
				app->teamManager->IsLilPigOnTeam = true;
				party_Lpig_button->text = "In Party";
			}
		}
		else
		{
			party_Lpig_button->text = "Unavailable";
		}
		break;

		// MPig
	case 71:
		if (app->teamManager->ismidpigplayable)
		{
			if (app->teamManager->IsMidPigOnTeam)
			{
				app->teamManager->IsMidPigOnTeam = false;
				party_Mpig_button->text = "-";
			}
			else
			{
				app->teamManager->IsMidPigOnTeam = true;
				party_Mpig_button->text = "In Party";
			}
		}
		else
		{
			party_Mpig_button->text = "Unavailable";
		}
		break;

		// Peter
	case 72:
		if (app->teamManager->ispeterplayable)
		{
			if (app->teamManager->IsPeterOnTeam)
			{
				app->teamManager->IsPeterOnTeam = false;
				party_peter_button->text = "-";
			}
			else
			{
				app->teamManager->IsPeterOnTeam = true;
				party_peter_button->text = "In Party";
			}
		}
		else
		{
			party_peter_button->text = "Unavailable";
		}
	}


	return true;
}


// Change buttons state depending on the scene
bool UIModule::ChangeButtonState(int& currentMenuType)
{
	switch (currentMenuType)
	{
	case MAIN:
		
		DisableButtonsToNone();
		// Activate main menu buttons
		mainmenu_play_button->state = GuiControlState::NORMAL;
		mainmenu_options_button->state = GuiControlState::NORMAL;
		mainmenu_credits_button->state = GuiControlState::NORMAL;
		mainmenu_quit_button->state = GuiControlState::NORMAL;
		mainmenu_newGame_button->state = GuiControlState::NONE;
		mainmenu_continueGame_button->state = GuiControlState::NONE;
		mainmenu_return_button->state = GuiControlState::NONE;


		break;
	case PAUSE:

		DisableButtonsToNone();

		// Activate pause buttons
		pausemenu_resume_button->state = GuiControlState::NORMAL;
		pausemenu_save_button->state = GuiControlState::NORMAL;
		pausemenu_load_button->state = GuiControlState::NORMAL;
		pausemenu_options_button->state = GuiControlState::NORMAL;
		pausemenu_return_button->state = GuiControlState::NONE;
		pausemenu_backtomain_button->state = GuiControlState::NORMAL;
		pausemenu_quit_button->state = GuiControlState::NORMAL;
		pausemenu_inventory_button->state = GuiControlState::NORMAL;
		pausemenu_party_button->state = GuiControlState::NORMAL;

		break;
	case INVENTORY:

		DisableButtonsToNone();

		break;
	case PARTY:

		DisableButtonsToNone();

		// Enable party buttons
		party_timmy_button->state = GuiControlState::NORMAL;
		party_bunny_button->state = GuiControlState::NORMAL;
		party_lrrh_button->state = GuiControlState::NORMAL;
		party_Lpig_button->state = GuiControlState::NORMAL;
		party_Mpig_button->state = GuiControlState::NORMAL;
		party_peter_button->state = GuiControlState::NORMAL;

		break;
	case COMBAT_PAUSE:

		DisableButtonsToNone();

		// Enable all combat pause buttons
		pausemenuCombat_resume_button->state = GuiControlState::NORMAL;
		pausemenuCombat_options_button->state = GuiControlState::NORMAL;
		pausemenuCombat_quit_button->state = GuiControlState::NORMAL;
		pausemenuCombat_backtomain_button->state = GuiControlState::NORMAL;
		pausemenuCombat_return_button->state = GuiControlState::NONE;

		break;

	case DIALOG:

		DisableButtonsToNone();

		// Activate dialog buttonts
		// Maybe there would be another switch for each dialog, idk how it will work
		dialog_option1_button->state = GuiControlState::NORMAL;
		dialog_option2_button->state = GuiControlState::NORMAL;
		dialog_option3_button->state = GuiControlState::NORMAL;
		dialog_option4_button->state = GuiControlState::NORMAL;

		break;
	case DIALOG2:

		DisableButtonsToNone();
		// Enable all dialog2 buttons
		dialog2_option1_button->state = GuiControlState::NORMAL;
		dialog2_option2_button->state = GuiControlState::NORMAL;
		dialog2_option3_button->state = GuiControlState::NORMAL;
		dialog2_option4_button->state = GuiControlState::NORMAL;

		break;
	case COMBAT:

		DisableButtonsToNone();
		// Activate combat buttons
		combat_attack_button->state = GuiControlState::NORMAL;
		combat_ability_button->state = GuiControlState::NORMAL;
		combat_move_button->state = GuiControlState::NORMAL;
		combat_endTurn_button->state = GuiControlState::NORMAL;

		break;

	case ROPE_MINIGAME:

		DisableButtonsToNone();

		break;
	case LEVEL_UP:

		app->audio->PlayFx(levelUpFX);

		DisableButtonsToNone();

		levelup_defenseUp_button->state = GuiControlState::NORMAL;
		levelup_magicUp_button->state = GuiControlState::NORMAL;
		levelup_speedUp_button->state = GuiControlState::NORMAL;
		levelup_movementUp_button->state = GuiControlState::NORMAL;
		levelup_attackUp_button->state = GuiControlState::NORMAL;
		levelup_AB1PowerUp_button->state = GuiControlState::NORMAL;
		levelup_healingpowerUp_button->state = GuiControlState::NORMAL;

		break;

	case DISABLED:

		DisableButtonsToNone();

		break;
	}

	return true;
}

std::string UIModule::DialogueOverTime(std::string dialogue)
{
	std::string aux;

	if (dialogue.size() > 0)
	{
		for (int i = 0; i < indexDialogueOverTime-1; i++)
		{
			aux.push_back(dialogue.at(i));
		}

	}

	return aux;
}

void UIModule::CleaningDialogeOverTime()
{
	dialogueHasChanged = true;
	indexDialogueOverTime = 0;
	dialogueOverTime.clear();
	textDialogueTimer.Start(0.05f);
}

void UIModule::PrintDialogue(std::vector<std::string> dialogue)
{
	int posX, posY;
	if (app->scene->active)
	{
		posX = app->scene->player->position.x;
		posY = app->scene->player->position.y;
	}

	// Draw NPC Popup
	SDL_Rect angryVillagerRect = { 1198, 212, 416, 705 };
	SDL_Rect talismanVillagerRect = { 1174, 2024, 435, 726 };
	SDL_Rect grandmaRect = { 107, 1958, 457, 748 };
	SDL_Rect lrrhRect = { 176, 1097, 632, 701 };

	switch (app->scene->GetPlayerLastCollision())
	{
	case ColliderType::ANGRYVILLAGER:
		// Dibuixar Angry tal
		app->render->DrawTexture(app->scene->npcPopUpTexture, app->scene->player->position.x - 800, app->scene->player->position.y - 300, &angryVillagerRect);
		break;

	case ColliderType::TALISMANVILLAGER:
		app->render->DrawTexture(app->scene->npcPopUpTexture, app->scene->player->position.x - 800, app->scene->player->position.y - 300, &talismanVillagerRect);
		break;

	case ColliderType::GRANDMA:
		app->render->DrawTexture(app->scene->npcPopUpTexture, app->scene->player->position.x - 800, app->scene->player->position.y - 300, &grandmaRect);
		break;

	case ColliderType::LRRH:
		app->render->DrawTexture(app->scene->npcPopUpTexture, app->scene->player->position.x - 800, app->scene->player->position.y - 300, &lrrhRect);
		break;

	default:
		break;
	}

	// Draw dialogue text image
	SDL_Rect dialogueRect = { 17, 16, 1700, 178 };
	app->render->DrawTexture(app->scene->uiSpriteTexture, -app->render->camera.x + 100, -app->render->camera.y + 680, &dialogueRect);

	//---------------------
	// Dialogue text block
	SDL_Rect rect = { 0 , 0, 800, 400 };


	//Comprobar si el cronómetro para que se printe la siguiente letra ya ha llegado a su fin
	//15 letras por segundo. A 60 frames/segundo -> 1 letra cada 0.25s;
	if (indexDialogueOverTime <= dialogue[0].length())
	{
		//De haber llegado al final el cronónmetro:
		//Pedirle a la función que nos dé el trozo que se tiene que pintar en este frame
		if (textDialogueTimer.Test() == estadoTimerP::FIN)
		{
			indexDialogueOverTime++;
			dialogueOverTime = DialogueOverTime(dialogue[0]);
			textDialogueTimer.Start(timeToRefreshDialogue);
		}
	}


	if (indexDialogueOverTime == dialogue[0].length())
	{
		std::cout << "Ad";
	}
	

	//Printar el textDialogue
	SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, dialogueOverTime.c_str(), { 0,0,0 }, 1700);
	app->render->DrawTexture(textDialogue, posX - 850, posY + 240, NULL);

	//Printar el textDialogue -  //COMENTADO DE MOMENTO PARA HACER PRUEBAS
	/*SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, dialogue[0].c_str(), { 0,0,0 }, 1700);
	app->render->DrawTexture(textDialogue, posX - 850, posY + 240, NULL);*/

	//--------------------

	// Change options buttons text
	SDL_Rect rectO1 = { 0, 0, 800, 30 };
	SDL_Rect rectO2 = { 0, 0, 800, 30 };
	SDL_Rect rectO3 = { 0, 0, 800, 30 };
	SDL_Rect rectO4 = { 0, 0, 800, 30 };

	SDL_Rect optionRect = { 18, 238, 939, 69 };

	// Check if there's dialogue available
	if (!(dialogue.size() <= 1))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->scene->uiSpriteTexture, -app->render->camera.x + 90, -app->render->camera.y + 885, &optionRect);

		SDL_Texture* textOption1 = app->fonts->LoadRenderedParagraph(rectO1, app->fonts->gameFont, dialogue[1].c_str(), { 0,0,0 }, rectO1.w);
		app->render->DrawTexture(textOption1, posX - 850, posY + 405, NULL);
		SDL_DestroyTexture(textOption1);
	}
	else
	{
		dialog_option1_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 2))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->scene->uiSpriteTexture, -app->render->camera.x + 90, -app->render->camera.y + 935, &optionRect);

		SDL_Texture* textOption2 = app->fonts->LoadRenderedParagraph(rectO2, app->fonts->gameFont, dialogue[2].c_str(), { 0,0,0 }, rectO2.w);
		app->render->DrawTexture(textOption2, posX - 850, posY + 455, NULL);
		SDL_DestroyTexture(textOption2);
	}
	else
	{
		dialog_option2_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 3))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->scene->uiSpriteTexture, -app->render->camera.x + 990, -app->render->camera.y + 885, &optionRect);

		SDL_Texture* textOption3 = app->fonts->LoadRenderedParagraph(rectO3, app->fonts->gameFont, dialogue[3].c_str(), { 0,0,0 }, rectO3.w);
		app->render->DrawTexture(textOption3, posX + 60, posY + 405, NULL);
		SDL_DestroyTexture(textOption3);
	}
	else
	{
		dialog_option3_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 4))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->scene->uiSpriteTexture, -app->render->camera.x + 990, -app->render->camera.y + 935, &optionRect);

		SDL_Texture* textOption4 = app->fonts->LoadRenderedParagraph(rectO4, app->fonts->gameFont, dialogue[4].c_str(), { 0,0,0 }, rectO4.w);
		app->render->DrawTexture(textOption4, posX + 60, posY + 455, NULL);
		SDL_DestroyTexture(textOption4);
	}
	else
	{
		dialog_option4_button->state = GuiControlState::NONE;
	}


	SDL_DestroyTexture(textDialogue);

}


void UIModule::PrintDialogue2(std::vector<std::string> dialogue)
{
	int posX, posY;
	if (app->w2_scene->active)
	{
		posX = app->w2_scene->player->position.x;
		posY = app->w2_scene->player->position.y;
	}


	// Draw NPC Popup
	SDL_Rect pigsRect = { 9, 1855, 662, 604 };
	SDL_Rect zorroRect = { 18, 924, 788, 703 };

	switch (app->w2_scene->GetPlayerLastCollision())
	{
	case ColliderType::PIGS:
		
		app->render->DrawTexture(app->w2_scene->npcPopUpTexture, app->w2_scene->player->position.x - 800, app->w2_scene->player->position.y - 300, &pigsRect);
		break;

	case ColliderType::ZORRO:
		app->render->DrawTexture(app->w2_scene->npcPopUpTexture, app->w2_scene->player->position.x - 900, app->w2_scene->player->position.y - 300, &zorroRect);
		break;

	default:
		break;
	}

	// Draw dialogue text image
	SDL_Rect dialogueRect = { 17, 16, 1700, 178 };
	app->render->DrawTexture(app->w2_scene->uiSpriteTexture, -app->render->camera.x + 100, -app->render->camera.y + 680, &dialogueRect);

	//---------------------
	// Dialogue text block
	SDL_Rect rect = { 0 , 0, 800, 400 };


	//Comprobar si el cronómetro para que se printe la siguiente letra ya ha llegado a su fin
	//15 letras por segundo. A 60 frames/segundo -> 1 letra cada 0.25s;
	if (indexDialogueOverTime <= dialogue[0].length())
	{
		//De haber llegado al final el cronónmetro:
		//Pedirle a la función que nos dé el trozo que se tiene que pintar en este frame
		if (textDialogueTimer.Test() == estadoTimerP::FIN)
		{
			indexDialogueOverTime++;
			dialogueOverTime = DialogueOverTime(dialogue[0]);
			textDialogueTimer.Start(timeToRefreshDialogue);
		}
	}


	if (indexDialogueOverTime == dialogue[0].length())
	{
		std::cout << "Ad";
	}


	//Printar el textDialogue
	SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, dialogueOverTime.c_str(), { 0,0,0 }, 1700);
	app->render->DrawTexture(textDialogue, posX - 850, posY + 240, NULL);

	//Printar el textDialogue -  //COMENTADO DE MOMENTO PARA HACER PRUEBAS
	/*SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, dialogue[0].c_str(), { 0,0,0 }, 1700);
	app->render->DrawTexture(textDialogue, posX - 850, posY + 240, NULL);*/

	//--------------------

	// Change options buttons text
	SDL_Rect rectO1 = { 0, 0, 800, 30 };
	SDL_Rect rectO2 = { 0, 0, 800, 30 };
	SDL_Rect rectO3 = { 0, 0, 800, 30 };
	SDL_Rect rectO4 = { 0, 0, 800, 30 };

	SDL_Rect optionRect = { 18, 238, 939, 69 };

	// Check if there's dialogue available
	if (!(dialogue.size() <= 1))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->w2_scene->uiSpriteTexture, -app->render->camera.x + 90, -app->render->camera.y + 885, &optionRect);

		SDL_Texture* textOption1 = app->fonts->LoadRenderedParagraph(rectO1, app->fonts->gameFont, dialogue[1].c_str(), { 0,0,0 }, rectO1.w);
		app->render->DrawTexture(textOption1, posX - 850, posY + 405, NULL);
		SDL_DestroyTexture(textOption1);
	}
	else
	{
		dialog2_option1_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 2))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->w2_scene->uiSpriteTexture, -app->render->camera.x + 90, -app->render->camera.y + 935, &optionRect);

		SDL_Texture* textOption2 = app->fonts->LoadRenderedParagraph(rectO2, app->fonts->gameFont, dialogue[2].c_str(), { 0,0,0 }, rectO2.w);
		app->render->DrawTexture(textOption2, posX - 850, posY + 455, NULL);
		SDL_DestroyTexture(textOption2);
	}
	else
	{
		dialog2_option2_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 3))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->w2_scene->uiSpriteTexture, -app->render->camera.x + 990, -app->render->camera.y + 885, &optionRect);

		SDL_Texture* textOption3 = app->fonts->LoadRenderedParagraph(rectO3, app->fonts->gameFont, dialogue[3].c_str(), { 0,0,0 }, rectO3.w);
		app->render->DrawTexture(textOption3, posX + 60, posY + 405, NULL);
		SDL_DestroyTexture(textOption3);
	}
	else
	{
		dialog2_option3_button->state = GuiControlState::NONE;
	}

	if (!(dialogue.size() <= 4))
	{
		// Draw options text iamge
		app->render->DrawTexture(app->w2_scene->uiSpriteTexture, -app->render->camera.x + 990, -app->render->camera.y + 935, &optionRect);

		SDL_Texture* textOption4 = app->fonts->LoadRenderedParagraph(rectO4, app->fonts->gameFont, dialogue[4].c_str(), { 0,0,0 }, rectO4.w);
		app->render->DrawTexture(textOption4, posX + 60, posY + 455, NULL);
		SDL_DestroyTexture(textOption4);
	}
	else
	{
		dialog2_option4_button->state = GuiControlState::NONE;
	}


	SDL_DestroyTexture(textDialogue);

}

void UIModule::CheckPartyTextOnStart()
{
	// Party Buttons
	// Timmy
	if (app->teamManager->istimmyplayable)
	{
		if (app->teamManager->IsTimmyOnTeam)
		{
			party_timmy_button->text = "In Party";
		}
		else
		{
			party_timmy_button->text = "-";
		}
	}
	else
	{
		party_timmy_button->text = "Unavailable";
	}

	// Bunny
	if (app->teamManager->isbunnyplayable)
	{
		if (app->teamManager->IsBunnyOnTeam)
		{
			party_bunny_button->text = "In Party";
		}
		else
		{
			party_bunny_button->text = "-";
		}
	}
	else
	{
		party_bunny_button->text = "Unavailable";
	}

	// LRRH
	if (app->teamManager->islrrhplayable)
	{
		if (app->teamManager->IsLrrhOnTeam)
		{
			party_lrrh_button->text = "In Party";
		}
		else
		{
			party_lrrh_button->text = "-";
		}
	}
	else
	{
		party_lrrh_button->text = "Unavailable";
	}

	// LPig
	if (app->teamManager->islilpigplayable)
	{
		if (app->teamManager->IsLilPigOnTeam)
		{
			party_Lpig_button->text = "In Party";
		}
		else
		{
			party_Lpig_button->text = "-";
		}
	}
	else
	{
		party_Lpig_button->text = "Unavailable";
	}

	// MPig
	if (app->teamManager->ismidpigplayable)
	{
		if (app->teamManager->IsMidPigOnTeam)
		{
			party_Mpig_button->text = "In Party";
		}
		else
		{
			party_Mpig_button->text = "-";
		}
	}
	else
	{
		party_Mpig_button->text = "Unavailable";
	}

	// Peter
	if (app->teamManager->ispeterplayable)
	{
		if (app->teamManager->IsPeterOnTeam)
		{
			party_peter_button->text = "In Party";
		}
		else
		{
			party_peter_button->text = "-";
		}
	}
	else
	{
		party_peter_button->text = "Unavailable";
	}
}

const char* uintToCChar(uint myuInt)
{
	std::string myString = std::to_string(myuInt);
	return myString.c_str();
}

void UIModule::AddButtonsToList()
{
	buttonsList.Add(mainmenu_play_button);
	buttonsList.Add(mainmenu_options_button);
	buttonsList.Add(mainmenu_credits_button);
	buttonsList.Add(mainmenu_quit_button);
	buttonsList.Add(mainmenu_newGame_button);
	buttonsList.Add(mainmenu_continueGame_button);
	buttonsList.Add(mainmenu_return_button);

	buttonsList.Add(pausemenu_resume_button);
	buttonsList.Add(pausemenu_inventory_button);
	buttonsList.Add(pausemenu_party_button);
	buttonsList.Add(pausemenu_save_button);
	buttonsList.Add(pausemenu_load_button);
	buttonsList.Add(pausemenu_options_button);
	buttonsList.Add(pausemenu_return_button);
	buttonsList.Add(pausemenu_backtomain_button);
	buttonsList.Add(pausemenu_quit_button);

	buttonsList.Add(pausemenuCombat_resume_button);
	buttonsList.Add(pausemenuCombat_options_button);
	buttonsList.Add(pausemenuCombat_backtomain_button);
	buttonsList.Add(pausemenuCombat_return_button);
	buttonsList.Add(pausemenuCombat_quit_button);

	buttonsList.Add(combat_attack_button);
	buttonsList.Add(combat_ability_button);
	buttonsList.Add(combat_move_button);
	buttonsList.Add(combat_endTurn_button);

	buttonsList.Add(dialog_option1_button);
	buttonsList.Add(dialog_option2_button);
	buttonsList.Add(dialog_option3_button);
	buttonsList.Add(dialog_option4_button);

	buttonsList.Add(dialog2_option1_button);
	buttonsList.Add(dialog2_option2_button);
	buttonsList.Add(dialog2_option3_button);
	buttonsList.Add(dialog2_option4_button);

	buttonsList.Add(levelup_defenseUp_button);
	buttonsList.Add(levelup_magicUp_button);
	buttonsList.Add(levelup_speedUp_button);
	buttonsList.Add(levelup_movementUp_button);
	buttonsList.Add(levelup_attackUp_button);
	buttonsList.Add(levelup_AB1PowerUp_button);
	buttonsList.Add(levelup_healingpowerUp_button);

	buttonsList.Add(party_timmy_button);
	buttonsList.Add(party_bunny_button);
	buttonsList.Add(party_lrrh_button);
	buttonsList.Add(party_Lpig_button);
	buttonsList.Add(party_Mpig_button);
	buttonsList.Add(party_peter_button);
}