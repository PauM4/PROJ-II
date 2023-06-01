#include "Portal.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "W2_Scene.h"
#include "W2_Scene_Maze.h"

Portal::Portal() : Entity(EntityType::PORTAL)
{
	name.Create("Portal");
}

Portal::~Portal() {}

bool Portal::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	nextScene = parameters.attribute("nextScene").as_int();

	nextX = parameters.attribute("nextPlayerX").as_int();
	nextY = parameters.attribute("nextPlayerY").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	for (int i = 0; i < 5; i++) 
	{
		idleAnim.PushBack({ (i * 282), 111, 282, 354 });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.30f;

	return true;
}

bool Portal::Start() {

	godMode = false;
	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	pbody = app->physics->CreateRectangleSensor(position.x + width / 2, position.y + height / 2, width, height, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PORTAL;

	return true;
}

bool Portal::Update(float dt)
{

	//Toggle godMode

	if ((app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)) godMode = !godMode;
	currentAnimation->Update();
	return true;
}

bool Portal::PostUpdate()
{
	if (godMode) app->render->DrawRectangle({ position.x, position.y, width, height }, 0, 255, 0, 50);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 55, position.y - 75, &rect);

	return true;
}

bool Portal::CleanUp()
{
	app->tex->UnLoad(texture);

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	int nextScene = NULL;

	return true;
}

void Portal::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision portal/player");
		switch (app->sceneManager->scene) {
		case SCENE:
			app->scene->takePortal = true; 
			app->uiModule->doorPlayerPosition = true;
			app->SaveGameRequest();
			app->sceneManager->LoadScene((GameScene)nextScene);
			break;
		case W2_SCENE:
			app->w2_scene->takePortal = true;
			app->uiModule->doorPlayerPosition = true;
			app->SaveGameRequest();
			app->sceneManager->LoadScene((GameScene)nextScene);
			break; 
		case W2_SCENE_MAZE:
			app->uiModule->doorPlayerPosition = true;
			app->SaveGameRequest();
			app->sceneManager->LoadScene((GameScene)nextScene);
			break;
		}
		
		break;
	}
}

void Portal::TriggerPortal(int scene)
{

	app->uiModule->doorPlayerPosition = true;
	app->SaveGameRequest();

	app->sceneManager->LoadScene((GameScene)scene);
}
