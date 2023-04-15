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

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i*150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.30f;

	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	
	speed = 10;
	vel = b2Vec2(0, 0);

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

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

	lastCollision = ColliderType::UNKNOWN;

	

	return true;
}

bool Player::Update()
{
	currentAnimation->Update();

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
		TriggerDialogueTree(lastCollision);
		InteractWithTree();

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

	//Movement
	if (!movementRestringed)
	{
		Movement();
	}


	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		InteractWithEntities();
	}


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool Player::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 55, position.y-75, &rect);

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

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
		case ColliderType::ANGRYVILLAGER:
			LOG("Collision 	ANGRYVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::ANGRYVILLAGER;
			break;
		case ColliderType::TALISMANVILLAGER:
			LOG("Collision 	TALISMANVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::TALISMANVILLAGER;
			break;
		case ColliderType::GRANDMA:
			LOG("Collision 	GRANDMA");
			npcInteractAvailable = true;
			lastCollision = ColliderType::GRANDMA;
			break;
		case ColliderType::LRRH:
			LOG("Collision 	LRRH");
			npcInteractAvailable = true;
			lastCollision = ColliderType::LRRH;
			break;
	}
	
}

void Player::InteractWithTree()
{
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(2);
	}
	else if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(3);
	}
	else if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(4);
	}


}

void Player::TriggerDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:
		app->scene->RunDialogueTree(ColliderType::ANGRYVILLAGER);
		break;
	case ColliderType::TALISMANVILLAGER:
		app->scene->RunDialogueTree(ColliderType::TALISMANVILLAGER);
		break;
	case ColliderType::GRANDMA:
		app->scene->RunDialogueTree(ColliderType::GRANDMA);
		break;
	case ColliderType::LRRH:
		app->scene->RunDialogueTree(ColliderType::LRRH);
		break;
	default:
		break;
	}
}

void Player::Movement()
{
	vel = b2Vec2(0, 0);

	bool isMovingH, isMovingV;

	isMovingH = HorizontalMovement();
	isMovingV = VerticalMovement();

	if (!isMovingH && !isMovingV)
		currentAnimation = &idleAnim;

	pbody->body->SetLinearVelocity(vel);
}

bool Player::VerticalMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel.y = -speed;
		currentAnimation = &walkUpAnim;
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		vel.y = speed;
		currentAnimation = &walkDownAnim;
		return true;
	}

	return false;
}

bool Player::HorizontalMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		vel.x = -speed;
		currentAnimation = &walkLeftAnim;
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		vel.x = speed;
		currentAnimation = &walkRightAnim;
		return true;
	}

	return false;

}

void Player::InteractWithEntities()
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
				StopVelocity();
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
				StopVelocity();
			}
		}
	}
}

void Player::StopVelocity()
{
	vel = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(vel);
	currentAnimation = &idleAnim;
}
