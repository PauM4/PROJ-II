#include "SceneManager.h"
#include "Log.h"
#include "Player.h"
#include "Map.h"
#include "App.h"


SceneManager::SceneManager(bool isActive) : Module(isActive)
{
	name.Create("Scene_manager");
}

// Destructor
SceneManager::~SceneManager()
{}

bool SceneManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool SceneManager::Start()
{
	bool ret = true;

	scene = GameScene::SCENE;

	return ret;
}

bool SceneManager::PreUpdate()
{
	bool ret = true;

	if ((app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = SCENE;
	}
	if ((app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = true;
		scene = BATTLE;
	}
	if ((app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = INTRO;
	}
	if ((app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = MAIN_MENU;
	}

	switch (scene) {
	case INTRO:
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			scene = SCENE;
		}
		break;
	case MAIN_MENU:
		break;
	case SCENE:
		break; 
	case GAME_OVER:
		break; 
	default:
		break; 
	}

	return ret;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	switch (scene) {
	case INTRO:
		if (currentScene != (Module*)app->sceneIntro) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneIntro, 20)) {
				currentScene = (Module*)app->sceneIntro;
				LOG("SCENE_INTRO");
			}
		}
		break;
	case MAIN_MENU:
		if (currentScene != (Module*)app->sceneMainMenu) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneMainMenu, 20)) {
				currentScene = (Module*)app->sceneMainMenu;
				LOG("SCENE_MAINMENU");
			}
		}
		break;
	case SCENE:
		if (currentScene == nullptr) {
			currentScene = (Module*)app->scene;
			currentScene->Enable(); 
			LOG("SCENE");
		}
		else if(currentScene!= (Module*)app->scene) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->scene, 20)) {
				currentScene = (Module*)app->scene;
				LOG("SCENE");
			}
		}
		break;
	case BATTLE:
		if (currentScene != (Module*)app->sceneBattle) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneBattle, 20)) {
				currentScene = (Module*)app->sceneBattle;
				LOG("SCENE_BATTLE");
			}
		}
		break;
	default:
		break;
	}

	return ret;
}

void SceneManager::LoadScene(GameScene gameScene) {
	if (scene != gameScene) {
		scene = gameScene; 
	}
}

bool SceneManager::CleanUp()
{
	if (currentScene != nullptr) {
		currentScene = nullptr;
	}
	return true;
}
