#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMainMenu.h"
#include "UIModule.h"

#include "Defs.h"
#include "Log.h"

SceneMainMenu::SceneMainMenu(bool isActive) : Module(isActive)
{
	
	name.Create("sceneMainMenu");
	
}

// Destructor
SceneMainMenu::~SceneMainMenu()
{}

// Called before render is available
bool SceneMainMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMainMenu");
	bool ret = true;
	press_enter = app->tex->Load("Assets/UI/press_enter.png");

	press_enter_animation.PushBack({ 0, 0, 1920, 1080 });
	press_enter_animation.PushBack({ 1930, 0, 1920, 1080 });
	press_enter_animation.loop = true;
	press_enter_animation.speed = 0.02f;

	currentAnimation = &press_enter_animation;

	return ret;
}

// Called before the first frame
bool SceneMainMenu::Start()
{	
	
	mainMenu_left = app->tex->Load("Assets/UI/main_title_left.png");
	mainMenu_right = app->tex->Load("Assets/UI/main_title_right.png");

	mainMenu_image_tittle = app->tex->Load("Assets/UI/TittleScreen_Tittle.png");
	

	UI_spritesheet_final = app->tex->Load("Assets/UI/UI_Spritesheet_FINAL.png");
	mainMenuRipped_image = app->tex->Load("Assets/UI/A_MainMenuRipped_SceneManager.png");
	credits_image = app->tex->Load("Assets/UI/A_Credits_MainMenu.png");

	press_enterRect = { 3487, 2199, 351, 40 };

	w = app->win->width;
	h = app->win->height;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	app->audio->PlayMusic("Assets/Sounds/Music/music_main_menu.ogg", 0.1f);

	creditsOpen = false;
	returnPressed = false;

	left_animation.Set();
	left_animation.smoothness = 4;
	left_animation.AddTween(100, 50, EXPONENTIAL_OUT);

	right_animation.Set();
	right_animation.smoothness = 4;
	right_animation.AddTween(100, 50, EXPONENTIAL_OUT);

	mouseSpeed = 15; 

	return true;
}

// Called each loop iteration
bool SceneMainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMainMenu::Update(float dt)
{
	app->input->GetMousePosition(mouseX, mouseY); 
	app->input->HandlePadMouse(mouseX, mouseY, mouseSpeed);

	currentAnimation->Update();
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
	{
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = MAIN;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		// Add cool audio
		app->audio->PlayFx(app->guiManager->pressedFxId);

		returnPressed = true;
	}



	return true;
}

// Called each loop iteration
bool SceneMainMenu::PostUpdate()
{
	bool ret = true;

	if (returnPressed)
	{
		left_animation.Step(1, false);
		right_animation.Step(-1, false);

		left_animation.Foward();
		right_animation.Foward();

		int offset = 162;
		int offset2 = 194;

		float point = left_animation.GetPoint();

		app->render->DrawTexture(mainMenuRipped_image, 0, 0, NULL);

		app->render->DrawTexture(mainMenu_left, -(offset2 + point * (offset2)), 0);
		app->render->DrawTexture(mainMenu_right, offset + point * (offset), 0);
	}
	else
	{
		app->render->DrawTexture(mainMenu_left, 0, 0, NULL);
		app->render->DrawTexture(mainMenu_right, 0, 0, NULL);

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(press_enter, 0, 0, &rect);
	}

	if (creditsOpen)
	{
		app->render->DrawTexture(credits_image, 0, 0, NULL);
	}

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneMainMenu::CleanUp()
{
	LOG("Freeing sceneMainMenu");	
	app->tex->UnLoad(mainMenu_left);
	app->tex->UnLoad(mainMenu_right);
	app->tex->UnLoad(mainMenu_image_tittle);
	app->tex->UnLoad(credits_image);
	app->tex->UnLoad(mainMenuRipped_image);
	app->tex->UnLoad(UI_spritesheet_final);
	

	return true;
}
