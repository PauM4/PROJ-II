#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "UIModule.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro(bool isActive) : Module(isActive)
{
	name.Create("sceneIntro");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{	
	logo_image = app->tex->Load("Assets/UI/A_ElectronicFarts_SceneIntro.png");

	w = app->win->width;
	h = app->win->height;



	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{

	// Tell to UIModule which currentMenuType we are now
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when scene is changed
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(logo_image, w/2 + 65, -h/2 + 35, NULL);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing sceneIntro");
	app->tex->UnLoad(logo_image);

	return true;
}
