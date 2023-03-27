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
	return true;
}

bool SceneManager::Start()
{
	scene = GameScene::INTRO;

}

bool SceneManager::PreUpdate()
{
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
}

bool SceneManager::Update(float dt)
{
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
