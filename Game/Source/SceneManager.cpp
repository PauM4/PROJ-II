#include "SceneManager.h"
#include "Log.h"
#include "Player.h"
#include "Map.h"
#include "App.h"


SceneManager::SceneManager() : Module()
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

	switch (scene) {
	case INTRO:
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
		break;
	case SCENE:
		if (currentScene == nullptr) {
			currentScene = (Module*)app->scene;
			currentScene->Enable(); 
			LOG("SCENE");
		}
		break;
	case BATTLE:
		if (currentScene != (Module*)app->sceneBattle) {
			if (app->fadeToBlack->Fade(currentScene, (Module*)app->sceneBattle, 60)) {
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
