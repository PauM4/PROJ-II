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

	position.x = 100;
	position.y = 0;

	return true;
}

bool Item::Start() {

	
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 40, 40, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update()
{ 
	return true;
}

bool Item::PostUpdate()
{
	return true;
}

bool Item::CleanUp()
{
	return true;
}