#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneGrandma.h"
#include "FadeToBlack.h"

enum GameScene {
	NONE = 0,
	INTRO,
	MAIN_MENU,
	//World_01
	SCENE,
	BATTLE,
	GRANDMA,
	COMBATLHHR,
	GAME_OVER,
	//World_02
	W2_SCENE,
	//World_03
	W3_SCENE
};

class SceneManager : public Module {
public:

	SceneManager(bool isActive);
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

	// Intro Scene things:
	Timer intro_timer;
	bool skipIntroScene;
	uint32 introCurrentTime;

	bool isBattle; 


};

#endif __SCENEMANAGER_H__
