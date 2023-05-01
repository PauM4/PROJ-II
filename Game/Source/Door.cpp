#include "Door.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Door::Door() : Entity(EntityType::DOOR)
{
	name.Create("Door");
}

Door::~Door() {}

bool Door::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	nextScene = parameters.attribute("nextScene").as_int();

	nextX = parameters.attribute("nextPlayerX").as_int();
	nextY = parameters.attribute("nextPlayerY").as_int();

	return true;
}

bool Door::Start() {

	godMode = false;

	pbody = app->physics->CreateRectangleSensor(position.x + width/2, position.y + height / 2, width, height, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DOOR;

	return true;
}

bool Door::Update(float dt)
{ 

	//Toggle godMode

	if ((app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)) godMode = !godMode;
	return true;
}

bool Door::PostUpdate()
{
	if (godMode) app->render->DrawRectangle({position.x, position.y, width, height},0,255,0,50);

	return true;
}

bool Door::CleanUp()
{
	app->tex->UnLoad(texture);

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	int nextScene = NULL;

	return true;
}

void Door::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision door/player");

		app->uiModule->doorPlayerPosition = true;
		app->SaveGameRequest();

		app->sceneManager->LoadScene((GameScene)nextScene);
		break;
	}
}

void Door::TriggerDoor(int scene)
{

	app->uiModule->doorPlayerPosition = true;
	app->SaveGameRequest();

	app->sceneManager->LoadScene((GameScene)scene);
}
