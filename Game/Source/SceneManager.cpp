#include "SceneManager.h"
#include "Log.h"
#include "Player.h"
#include "Map.h"
#include "App.h"
#include <iostream>

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

	skipIntroScene = false;

	scene = GameScene::SCENE;

	return ret;
}

bool SceneManager::PreUpdate()
{
	bool ret = true;

	if ((app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::SCENE;
	}
	if ((app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = true;
		scene = GameScene::BATTLE;
	}
	if ((app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::INTRO;
		intro_timer.Start();
	}
	// If click key 4 or (we are at sceneIntro and time is >= 3), change scene to MainMenu
	if ((app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN || (currentScene == (Module*)app->sceneIntro && intro_timer.ReadSec() >= 5)) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::MAIN_MENU;
	}

	switch (scene) {
	case GameScene::INTRO:
		break;
	case GameScene::MAIN_MENU:
		break;
	case GameScene::SCENE:
		break; 
	case GameScene::GAME_OVER:
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
	case GameScene::INTRO:
		if (currentScene != (Module*)app->sceneIntro) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneIntro, 20)) {
				currentScene = (Module*)app->sceneIntro;
				LOG("SCENE_INTRO");
			}
			intro_timer.Start();
		}
		break;
	case GameScene::MAIN_MENU:
		if (currentScene != (Module*)app->sceneMainMenu) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneMainMenu, 20)) {
				currentScene = (Module*)app->sceneMainMenu;
				LOG("SCENE_MAINMENU");
			}
		}
		break;
	case GameScene::SCENE:
		if (currentScene == nullptr) {
			currentScene = (Module*)app->scene;
			currentScene->Enable(); 
			LOG("SCENE");
		}
		else if(currentScene != (Module*)app->scene) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->scene, 20)) {
				currentScene = (Module*)app->scene;
				LOG("SCENE");
			}
		}
		break;
	case GameScene::BATTLE:
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

	introCurrentTime = intro_timer.ReadSec();

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
