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
#include "Animation.h"
#include "Fonts.h"

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

	walkDownAnim.PushBack({ 0, 0, 140, 140 });
	//walkDownAnim.PushBack({ 600, 0, 562, 754 });
	//walkDownAnim.PushBack({ 1150, 0, 562, 754 });
	//walkDownAnim.PushBack({ 1685, 0, 562, 754 });
	//walkDownAnim.PushBack({ 2217, 0, 562, 754 });
	//walkDownAnim.PushBack({ 2780, 0, 562, 754 });
	//walkDownAnim.PushBack({ 3300, 0, 562, 754 });
	//walkDownAnim.PushBack({ 3838, 0, 562, 754 });
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);
	walkDownTexture = app->tex->Load("Assets/Characters/Medidas_sprites_anim-sombra_def.png");
	currentAnimation = &walkDownAnim;

	pbody = app->physics->CreateRectangle(2000,0,70,70, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;

	// Bool variables
	npcInteractAvailable = false;
	itemInteractAvailable = false;
	movementRestringed = false;
	playerState = PlayerState::MOVING;
	playerPrevState = PlayerState::MOVING;

	return true;
}

bool Player::Update()
{
	currentAnimation->Update();

	int speed = 10; 
	b2Vec2 vel = b2Vec2(0, 0); 

	switch (playerState)
	{
	case PAUSE:
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: PAUSE", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case INVENTORY:
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: INVENTORY", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/

		break;
	case MOVING:
		movementRestringed = false;
		/*app->fonts->DrawText("PLAYER STATE: MOVING", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case BATTLE:
		break;
	case NPC_INTERACT:
		LOG("TALKING TO NPC1");
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: NPC_INTERACT", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case ITEM_INTERACT:
		LOG("INTERACTING WITH ITEM");
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: ITEM_INTERACT", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case NONE:
		break;
	}

	// movement code
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
		if (playerState != PlayerState::PAUSE)
		{
			if (npcInteractAvailable)
			{
				if (playerState == NPC_INTERACT)
				{
					playerState = MOVING;
					npcInteractAvailable = false;
				}
				// Moving
				else
				{
					playerPrevState = playerState;
					playerState = NPC_INTERACT;
				}
			}

			else if (itemInteractAvailable)
			{
				if (playerState == ITEM_INTERACT)
				{
					playerState = MOVING;
					itemInteractAvailable = false;
				}
				// Moving
				else
				{
					playerPrevState = playerState;
					playerState = ITEM_INTERACT;
				}
			}
		}
		
	}

	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool Player::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	// Animation removed to continue working
	app->render->DrawTexture(walkDownTexture, position.x - 55, position.y-75, &rect);
	/*app->render->DrawTexture(texture, position.x , position.y);*/

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
