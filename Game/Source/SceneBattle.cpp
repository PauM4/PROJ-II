#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneBattle.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle() : Module()
{
	name.Create("scene");
}

// Destructor
SceneBattle::~SceneBattle()
{}

// Called before render is available
bool SceneBattle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load();

	return true;
}


// Called each loop iteration
bool SceneBattle::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool SceneBattle::Update(float dt)
{
	
	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method

	return true;
}

//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");

	return true;
}
