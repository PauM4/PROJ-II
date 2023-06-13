#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "EndScene.h"
#include "UIModule.h"
#include "TeamManager.h"

#include "Defs.h"
#include "Log.h"

EndScene::EndScene(bool isActive) : Module(isActive)
{
	name.Create("endScene");
}

// Destructor
EndScene::~EndScene()
{}

// Called before render is available
bool EndScene::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool EndScene::Start()
{
	
	endComicTexture = app->tex->Load("Assets/UI/EndGame.png");

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
	return true;
}

// Called each loop iteration
bool EndScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool EndScene::Update(float dt)
{
	
	app->render->DrawTexture(endComicTexture, -app->render->camera.x, -app->render->camera.y);
	return true;
}

// Called each loop iteration
bool EndScene::PostUpdate()
{
	bool ret = true;


	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool EndScene::CleanUp()
{
	LOG("Freeing sceneIntro");
	app->tex->UnLoad(endComicTexture);

	return true;
}
