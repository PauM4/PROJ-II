#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Scene.h"

enum GameScene {
	NONE = 0,
	INTRO,
	SCENE,
	BATTLE,
	GAME_OVER
};

class SceneManager : public Module {
public:

	SceneManager();
	~SceneManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();


	void LoadScene(GameScene gameScene);


public:
	GameScene scene;

	Module* currentScene = nullptr; 

	bool isBattle; 
};

#endif __SCENEMANAGER_H__
