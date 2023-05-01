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

	idleAnim.PushBack({ 0, 0, 150, 150 });

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

		app->uiModule->doorPlayerPosition = true;
		app->SaveGameRequest();

		app->sceneManager->LoadScene((GameScene)nextScene);
		break;
	}
}

void Portal::TriggerPortal(int scene)
{

	app->uiModule->doorPlayerPosition = true;
	app->SaveGameRequest();

	app->sceneManager->LoadScene((GameScene)scene);
}
