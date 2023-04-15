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

	w = app->win->width;
	h = app->win->height;

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
	// Tell to UIModule which currentMenuType we are now and what was the previous one
	app->uiModule->previousMenuType = app->uiModule->currentMenuType;
	app->uiModule->currentMenuType = CurrentMenuType::MAIN;
	return true;
}

// Called each loop iteration
bool SceneMainMenu::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(mainMenu_image, w/2 + 65, -h/2 + 35, NULL);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneMainMenu::CleanUp()
{
	LOG("Freeing sceneMainMenu");	
	app->tex->UnLoad(mainMenu_image);

	return true;
}
