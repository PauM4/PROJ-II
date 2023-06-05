#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "UIModule.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro(bool isActive) : Module(isActive)
{
	name.Create("sceneIntro");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{	
	logo_image = app->tex->Load("Assets/UI/logo.png");
	logo_background = app->tex->Load("Assets/UI/logo_background.png");
	logo_title = app->tex->Load("Assets/UI/logo_title.png");

	w = app->win->width;
	h = app->win->height;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Tell to UIModule which currentMenuType we are now
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when scene is changed
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	app->audio->PlayMusic("Assets/Sounds/Music/intro_audio.ogg", 0.1f);

	logo_animation_picture.Set();
	logo_animation_picture.smoothness = 4;
	logo_animation_picture.AddTween(100, 70, EXPONENTIAL_OUT);

	logo_animation_title.Set();
	logo_animation_title.smoothness = 4;
	logo_animation_title.AddTween(100, 70, EXPONENTIAL_OUT);

	

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	logo_animation_picture.Step(1, false);
	logo_animation_title.Step(1, false);

	logo_animation_picture.Foward();
	logo_animation_title.Foward();

	int offset = 720;

	float point = logo_animation_picture.GetPoint();
	
	app->render->DrawTexture(logo_background, 0, 0, NULL);
	app->render->DrawTexture(logo_image, -(offset + point * (0 - offset)), 0);
	app->render->DrawTexture(logo_title, offset + point * (0 - offset), 0);

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing sceneIntro");
	app->tex->UnLoad(logo_image);
	app->tex->UnLoad(logo_background);
	app->tex->UnLoad(logo_title);

	return true;
}
