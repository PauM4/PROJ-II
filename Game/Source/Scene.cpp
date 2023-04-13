#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Item.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool isActive) : Module(isActive)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	
	//L02: DONE 3: Instantiate the player using the entity manager
	npc1 = (Npc*)app->entityManager->CreateEntity(EntityType::NPC);

	item1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	//app->physics->Enable();
	//Fonts initialize
	char lookUpTable[] = { " !�#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[�]^_�abcdefghijklmnopqrstuvwxyz{|}~" };

	font = app->fonts->Load("Assets/Fonts/GameFont.png", lookUpTable, 1);

	// L03: DONE: Load map
	bool retLoad = app->map->Load(mapName, mapFolder);

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		//bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		//if(retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	
	uint w, h;
	app->win->GetWindowSize(w, h);
	button1_continue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continue", { (int)w - 1820, (int)h - 300, 100, 20 }, this);
	button1_continue->state = GuiControlState::NONE;
	button2_exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button2_exit->state = GuiControlState::NONE;

	pauseMenuActive = false;
	exitButtonBool = false;
	

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	app->render->camera.x = -(int)player->position.x + app->render->camera.w / 2;
	app->render->camera.y = -(int)player->position.y + app->render->camera.h / 2;

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// L14: TODO 4: Make the camera movement independent of framerate
	float speed = 0.2 * dt;
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);


	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (player->playerState == player->PlayerState::PAUSE)
		{
			player->playerState = player->playerPrevState;

			button1_continue->state = GuiControlState::NONE;
			button2_exit->state = GuiControlState::NONE;
		}
		else
		{
			// Save previous state to go back
			player->playerPrevState = player->playerState;
			player->playerState = player->PlayerState::PAUSE;
			button1_continue->state = GuiControlState::NORMAL;
			button2_exit->state = GuiControlState::NORMAL;			
		}
	}

	if(player->playerState == player->PAUSE) app->guiManager->Draw();

	
	// Draw map
	app->map->Draw();
	//Font test
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;



	//L15: Draw GUI
	app->guiManager->Draw();
	
	// When exit button click, close app
	if (exitButtonBool == true)
	{
		return false;
	}
	//app->fonts->DrawText("NPC1", -20, -90, 100, 100, { 255,255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("ITEM1", 100, -90, 100, 100, { 255,255,255,255 }, app->fonts->gameFont);


	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 Continue click");
		player->playerState = player->playerPrevState;
		break;
	case 2:
		LOG("Button 2 Exit click");
		exitButtonBool = true;
		break;
	}
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->fonts->UnLoad(font);
	app->map->CleanUp(); 
	app->entityManager->CleanUp(); 
	//app->physics->Disable();	
	

	return true;
}
