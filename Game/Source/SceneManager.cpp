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

	scene = GameScene::INTRO;

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
		std::cout << "------------------------" << std::endl;
		std::cout << "To Main Menu" << std::endl;
		app->sceneManager->isBattle = false;
		scene = GameScene::MAIN_MENU;
	}
	if ((app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::GRANDMA;
	}

	switch (scene) {
	case GameScene::INTRO:
		if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) && currentScene->active == true)
		{
			scene = MAIN_MENU; 
		}
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
		if (currentScene == nullptr) {
			currentScene = (Module*)app->sceneIntro;
			currentScene->Enable();
			LOG("SCENE_INTRO");
		}
		else if (currentScene != (Module*)app->sceneIntro) {
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
		if(currentScene != (Module*)app->scene) {
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
	case GameScene::COMBATLHHR:
		if (currentScene != (Module*)app->sceneCombatLHHR) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneCombatLHHR, 20)) {
				currentScene = (Module*)app->sceneCombatLHHR;
				LOG("SCENE_BATTLE");
			}
		}
		break;
	case GameScene::GRANDMA:
		if (currentScene != (Module*)app->sceneGrandma) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneGrandma, 20)) {
				currentScene = (Module*)app->sceneGrandma;
				LOG("SCENE_GRANDMA");
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
