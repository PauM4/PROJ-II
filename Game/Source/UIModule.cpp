#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "UIModule.h"
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
	mainmenu_play_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { (int)w - 300, (int)h - 1000, 100, 20 }, this);
	mainmenu_play_button->state = GuiControlState::NORMAL;
	mainmenu_options_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { (int)w - 300, (int)h - 975, 100, 20 }, this);
	mainmenu_options_button->state = GuiControlState::NORMAL;
	mainmenu_credits_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { (int)w - 300, (int)h - 950, 100, 20 }, this);
	mainmenu_credits_button->state = GuiControlState::NORMAL;
	mainmenu_quit_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Quit", { (int)w - 300, (int)h - 925, 100, 20 }, this);
	mainmenu_quit_button->state = GuiControlState::NORMAL;
	mainmenu_newGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "New Game", { (int)w - 300, (int)h - 1000, 100, 20 }, this);
	mainmenu_newGame_button->state = GuiControlState::NONE;
	mainmenu_continueGame_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Continue", { (int)w - 300, (int)h - 975, 100, 20 }, this);
	mainmenu_continueGame_button->state = GuiControlState::NONE;
	mainmenu_return_button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Return", { (int)w - 300, (int)h - 950, 100, 20 }, this);
	mainmenu_return_button->state = GuiControlState::NONE;

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
	switch (currentMenuType)
	{
	case MAIN:
		break;
	case PAUSE:
		break;
	case DIALOG:
		break;
	case COMBAT:
		break;
	case DISABLED:
		break;
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

	switch (control->id)
	{
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
	return true;
}
