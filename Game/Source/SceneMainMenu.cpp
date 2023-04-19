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
	credits_image = app->tex->Load("Assets/UI/A_Credits_MainMenu.png");

	w = app->win->width;
	h = app->win->height;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = MAIN;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	optionsOpen = false;

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


	return true;
}

// Called each loop iteration
bool SceneMainMenu::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(mainMenu_image, 0, 0, NULL);

	if (optionsOpen)
	{
		app->render->DrawTexture(app->sceneMainMenu->credits_image, 0, 0, NULL);
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
	app->tex->UnLoad(credits_image);

	return true;
}
