#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	isPicked = false;

	return true;
}

bool Item::Start() {

	if (!isPicked)
	{
		pbody = app->physics->CreateRectangleSensor(position.x + width/2, position.y + height/2, width, height, bodyType::STATIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::ITEM;
	}

	return true;
}

bool Item::Update(float dt)
{ 
	return true;
}

bool Item::PostUpdate()
{
	return true;
}

bool Item::CleanUp()
{
	app->tex->UnLoad(texture);

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	return true;
}