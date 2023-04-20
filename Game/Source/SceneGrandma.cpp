#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGrandma.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

SceneGrandma::SceneGrandma(bool isActive) : Module(isActive)
{
	name.Create("sceneGrandma");
}

// Destructor
SceneGrandma::~SceneGrandma()
{}

// Called before render is available
bool SceneGrandma::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	app->physics->Enable();

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	door = (Door*)app->entityManager->CreateEntity(EntityType::DOOR);
	door->parameters = config.child("door");

	app->entityManager->Awake(config);

	return ret;
}

// Called before the first frame
bool SceneGrandma::Start()
{	
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->entityManager->Start();

	// Tell to UIModule which currentMenuType we are now
	//app->uiModule->currentMenuType = DISABLED;
	// Call this function only when scene is changed
	//app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	bool retLoad = app->map->Load(mapName, mapFolder);

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		//bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		//if(retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	app->audio->PlayMusic("Assets/Sounds/Music/music_grandaHouse.wav", 0.2f);

	return true;
}

// Called each loop iteration
bool SceneGrandma::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGrandma::Update(float dt)
{

	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
	{
		// If player is in pause, close it
		if (!isPaused)
		{
			app->uiModule->currentMenuType = DISABLED;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			isPaused = true;
		}
		// If player is NOT in pause, open it
		else
		{
			app->uiModule->currentMenuType = PAUSE;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			isPaused = false;
		}
	}

	//Follow player
	app->render->FollowObject((-1)*player->position.x, (-1) * player->position.y, app->render->camera.w/2, app->render->camera.h / 2);

	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneGrandma::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneGrandma::CleanUp()
{
	LOG("Freeing sceneIntro");

	app->map->CleanUp();
	app->entityManager->CleanUp();
	app->physics->Disable();

	return true;
}