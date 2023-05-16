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

	if ((app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::W2_SCENE;
	}

	if ((app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::W3_SCENE;
	}

	if ((app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::FOXQUEST;
	}
	if ((app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = true;
		scene = GameScene::COMBATLHHR;
	}
	if ((app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = true;
		scene = GameScene::COMBATOINK;
	}
	if ((app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) && currentScene->active == true)
	{
		app->sceneManager->isBattle = false;
		scene = GameScene::W2_SCENE_MAZE;
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
				LOG("SCENE_BATTLELHHR");
			}
		}
		break;
	case GameScene::COMBATOINK:
		if (currentScene != (Module*)app->battleScene_Pigs) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->battleScene_Pigs, 20)) {
				currentScene = (Module*)app->battleScene_Pigs;
				LOG("SCENE_BATTLEOINK");
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
	case GameScene::W2_SCENE:
		if (currentScene != (Module*)app->w2_scene) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->w2_scene, 20)) {
				currentScene = (Module*)app->w2_scene;
				LOG("W2_SCENE");
			}
		}
		break;
	case GameScene::W2_SCENE_MAZE:
		if (currentScene != (Module*)app->w2_scene_maze) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->w2_scene_maze, 20)) {
				currentScene = (Module*)app->w2_scene_maze;
				LOG("W2_SCENE_MAZE");
			}
		}
		break;
	case GameScene::W3_SCENE:
		if (currentScene != (Module*)app->w3_scene) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->w3_scene, 20)) {
				currentScene = (Module*)app->w3_scene;
				LOG("W3_SCENE");
			}
		}
		break;
	case GameScene::FOXQUEST:
		if (currentScene != (Module*)app->sceneFoxQuest) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneFoxQuest, 20)) {
				currentScene = (Module*)app->sceneFoxQuest;
				LOG("SCENE_FOXQUEST");
			}
		}
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
