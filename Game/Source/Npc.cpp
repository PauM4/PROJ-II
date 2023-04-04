#include "Npc.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Nonplayable.h"
#include "Entity.h"
#include "Npc.h"

Npc::Npc() : NonPlayable(NonPlayableType::NPC)
{
	name.Create("Npc");
}

Npc::~Npc() {

}

bool Npc::Awake() {

	// Initialize Npc parameters
	position.x = 0;
	position.y = 0;

	return true;
}

bool Npc::Start() {
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 128, 128, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;
	return true;
}

bool Npc::Update()
{

	return true;
}

bool Npc::CleanUp()
{
	return true;
}

void Npc::OnCollision(PhysBody* physA, PhysBody* physB)
{

}
