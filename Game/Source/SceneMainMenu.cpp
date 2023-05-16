#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMainMenu.h"
#include "UIModule.h"

#include "Defs.h"
#include "Log.h"

SceneMainMenu::SceneMainMenu(bool isActive) : Module(isActive)
{
	name.Create("sceneMainMenu");
}

// Destructor
SceneMainMenu::~SceneMainMenu()
{}

// Called before render is available
bool SceneMainMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMainMenu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneMainMenu::Start()
{	
	mainMenu_image = app->tex->Load("Assets/UI/A_MainMenuBackground_SceneMainMenu.png");
	mainMenu_image_tittle = app->tex->Load("Assets/UI/TittleScreen_Tittle.png");
	UI_spritesheet_final = app->tex->Load("Assets/UI/UI_Spritesheet_FINAL.png");
	mainMenuRipped_image = app->tex->Load("Assets/UI/A_MainMenuRipped_SceneManager.png");
	credits_image = app->tex->Load("Assets/UI/A_Credits_MainMenu.png");

	press_enterRect = { 3487, 2199, 351, 40 };

	w = app->win->width;
	h = app->win->height;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	app->audio->PlayMusic("Assets/Sounds/Music/music_main_menu.ogg", 0.1f);

	creditsOpen = false;
	returnPressed = false;

	return true;
}

// Called each loop iteration
bool SceneMainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMainMenu::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = MAIN;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		// Add cool audio
		app->audio->PlayFx(app->guiManager->pressedFxId);

		returnPressed = true;
	}



	return true;
}

// Called each loop iteration
bool SceneMainMenu::PostUpdate()
{
	bool ret = true;

	if (returnPressed)
	{
		app->render->DrawTexture(mainMenuRipped_image, 0, 0, NULL);
	}
	else
	{
		app->render->DrawTexture(mainMenu_image, 0, 0, NULL);
		//app->render->DrawTexture(UI_spritesheet_final, 786, 453, &press_enterRect);
	}

	if (creditsOpen)
	{
		app->render->DrawTexture(credits_image, 0, 0, NULL);
	}

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneMainMenu::CleanUp()
{
	LOG("Freeing sceneMainMenu");	
	app->tex->UnLoad(mainMenu_image);
	app->tex->UnLoad(mainMenu_image_tittle);
	app->tex->UnLoad(credits_image);
	app->tex->UnLoad(mainMenuRipped_image);
	

	return true;
}
