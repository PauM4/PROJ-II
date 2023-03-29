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

	scene = GameScene::INTRO;

	return ret;
}

bool SceneManager::PreUpdate()
{
	bool ret = true;

	switch (scene) {
	case INTRO:
		break;
	case SCENE:
		break; 
	case BATTLE:
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
		break;
	case BATTLE:
		break;
	case GAME_OVER:
		break;
	default:
		break;
	}

	return ret;
}

void SceneManager::LoadScene(GameScene gameScene) {
	if (scene != gameScene) {
		gameScene = scene; 
	}
}

bool SceneManager::CleanUp()
{
	if (currentScene != nullptr) {
		currentScene = nullptr;
	}
	return true;
}
