#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);
	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;

	// Bool variables
	npcInteractAvailable = false;
	itemInteractAvailable = false;
	movementRestringed = false;

	return true;
}

bool Player::Update()
{

	int speed = 10; 
	b2Vec2 vel = b2Vec2(0, 0); 


	if (movementRestringed == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel.y = -speed;

		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y = speed;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = -speed;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = speed;
		}
	}

	

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		// Interaction with NPC
		if (npcInteractAvailable == true)
		{
			movementRestringed = true;
			LOG("TALKING TO NPC1");
		}

		// Interaction with ITEM
		if (itemInteractAvailable == true)
		{
			movementRestringed = true;
			LOG("INTERACTING WITH ITEM1");
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		movementRestringed = false;
	}

	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	app->render->DrawTexture(texture, position.x , position.y);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			itemInteractAvailable = true;
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::NPC:
			LOG("Collision NPC");
			npcInteractAvailable = true;
			break;
	}
	


}
