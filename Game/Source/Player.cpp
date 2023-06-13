#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "W2_Scene.h"
#include "W2_Scene_Maze.h"
#include "W3_Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "Fonts.h"
#include "TeamManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {


	//Timmy Animations
	idleAnim.PushBack({ 0, 0, 150, 150 });
	rightIdleAnim.PushBack({ (4 * 150), 450, 150, 150 });
	leftIdleAnim.PushBack({ (4 * 150), 300, 150, 150 });
	upIdleAnim.PushBack({ 0, 600, 150, 150 });


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

	//Bunny Animations
	bunnyIdleAnim.PushBack({ 0, 0, 140, 140 });
	bunnyRightIdleAnim.PushBack({ (150), 450, 150, 150 });
	bunnyLeftIdleAnim.PushBack({ (150), 300, 150, 150 });
	bunnyUpIdleAnim.PushBack({ 0, 600, 150, 150 });

	for (int i = 0; i < 4; i++) 
	{
		bunnyWalkDownAnim.PushBack({ (i * 150), 150, 150, 150 });
	}
	bunnyWalkDownAnim.loop = true;
	bunnyWalkDownAnim.speed = 0.30f;

	for (int i = 0; i < 4; i++)
	{
		bunnyWalkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	bunnyWalkUpAnim.loop = true;
	bunnyWalkUpAnim.speed = 0.30f;

	for (int i = 0; i < 4; i++)
	{
		bunnyWalkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	bunnyWalkRightAnim.loop = true;
	bunnyWalkRightAnim.speed = 0.1f;

	for (int i = 0; i < 4; i++)
	{
		bunnyWalkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	bunnyWalkLeftAnim.loop = true;
	bunnyWalkLeftAnim.speed = 0.15f;

	//TexturePaths
	texturePath = parameters.attribute("texturepath").as_string();
	bunnyTexturePath = parameters.attribute("bunnyTexturepath").as_string();

	//If new game pressed:
	if (!app->scene->isNewGame)
	{
		position.x = app->scene->loadPlayerPosX;
		position.y = app->scene->loadPlayerPosY;
		app->scene->isNewGame = false;
	}
	else
	{
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
	}
	
	eKeyTexture = app->tex->Load("Assets/UI/eKey.png");

	
	
	speed = 350;
	vel = b2Vec2(0, 0);

	return true;
}

bool Player::Start() {

	chestFX = app->audio->LoadFx("Assets/Sounds/FX/fx_open_chest.wav");

	// Grab player position from save_game file
	texture = app->tex->Load(texturePath);
	bunnyTexture = app->tex->Load(bunnyTexturePath);

	currentAnimation = &idleAnim;
	bunnyCurrentAnimation = &bunnyIdleAnim;

	pbody = app->physics->CreateRectangle(position.x,position.y,70,70, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;


	bunnyPbody = app->physics->CreateCircle(position.x, position.y+80, 25, bodyType::DYNAMIC);
	bunnyPbody->body->SetFixedRotation(true);
	bunnyPbody->listener = nullptr;
	bunnyPbody->body->SetLinearDamping(5.0f);

	//Bunny Joint
	b2RopeJointDef ropeJointDef;
	ropeJointDef.bodyA = pbody->body;
	ropeJointDef.bodyB = bunnyPbody->body;
	ropeJointDef.localAnchorA = { 0, 0 }; 
	ropeJointDef.localAnchorB = { 0, 0 }; 
	ropeJointDef.maxLength = 2.0f;
	ropeJointDef.collideConnected = true;

	joint = (b2RopeJoint*)app->physics->world->CreateJoint(&ropeJointDef);
	


	const b2Vec2 aux = pbody->body->GetPosition();
	bunnyPbody->body->SetTransform(aux, 0); // posición inicial de la mascota


	// Bool variables
	npcInteractAvailable = false;
	itemInteractAvailable = false;
	movementRestringed = false;
	playerState = PlayerState::MOVING;
	playerPrevState = PlayerState::MOVING;

	lastCollision = ColliderType::UNKNOWN;

	godMode = false;
	
	if (app->uiModule->continueBool)
	{
		app->LoadGameRequest();
	}


	triggerBunnyDialogue = false;
	endBunnyDialogue = false;


	return true;
}

bool Player::Update(float dt)
{	
	currentAnimation->Update();
	bunnyCurrentAnimation->Update();

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
	{
		InteractWithEntities();

		if (playerState == NPC_INTERACT)
		{
			app->uiModule->CleaningDialogeOverTime();
		}

		if (playerState == ITEM_INTERACT)
		{
			app->audio->PlayFx(chestFX);
		}
	}

	if(app->teamManager->lvlupplayerstate == true)
	{
		if (playerState != PlayerState::LEVEL_UP)
		{
			playerPrevState = playerState;
			playerState = LEVEL_UP;

		}
		// If player is NOT in lvlUP, open it
		else
		{
			playerPrevState = playerState;
			playerState = MOVING;
		}
		app->teamManager->lvlupplayerstate = false;
	}


	if (app->scene->active)
	{
		if (triggerBunnyDialogue && endBunnyDialogue)
		{
			lastCollision = ColliderType::UNKNOWN;
			app->scene->dialogueTutorial = true;
		}

		if (app->scene->basicTutorialCounter == 2 && !triggerBunnyDialogue)
		{
			lastCollision = ColliderType::BUNNY;
			triggerBunnyDialogue = true;
			npcInteractAvailable = true;
			//playerState = NPC_INTERACT;
			//StopVelocity();
			app->uiModule->CleaningDialogeOverTime();
			TriggerDialogueTree(lastCollision);
			InteractWithEntities();
		}
	}


	


	switch (playerState)
	{
	case PAUSE:
		movementRestringed = true;
		StopVelocity();

		break;
	case INVENTORY:
		movementRestringed = true;
		StopVelocity();


		break;
	case MOVING:
		movementRestringed = false;

		break;
	case BATTLE:
		break;
	case NPC_INTERACT:
		LOG("TALKING TO NPC1");
		movementRestringed = true;
		StopVelocity();
		TriggerDialogueTree(lastCollision);
		InteractWithTree();

		break;
	case ITEM_INTERACT:
		LOG("INTERACTING WITH ITEM");
		movementRestringed = true;
		StopVelocity();

		if (app->scene->active)
		{
			if (isChest1Pickable)
			{
				app->teamManager->ironchestplate.ininventory = true;
				app->scene->chest1->isPicked = true;
				app->teamManager->loadinventory();

			}
			if (isChest2Pickable)
			{
				app->teamManager->reversehat.ininventory = true;
				app->scene->chest2->isPicked = true;
				app->teamManager->loadinventory();
			}
		}
		if (app->w2_scene_maze->active)
		{
			if (isChest3Pickable)
			{
				app->teamManager->bracelet.ininventory = true;
				app->w2_scene_maze->chest3->isPicked = true;
				app->teamManager->loadinventory();
			}

		}
		if (app->w2_scene->active)
		{
			if (isChest4Pickable)
			{
				app->teamManager->ironchestplate2.ininventory = true;
				app->w2_scene->chest4->isPicked = true;
				app->teamManager->loadinventory();
			}
			if (isChest5Pickable)
			{
				app->teamManager->susjar.ininventory = true;
				app->w2_scene->chest5->isPicked = true;
				app->teamManager->loadinventory();
			}
			if (isChest6Pickable)
			{
				app->teamManager->dentures.ininventory = true;
				app->w2_scene->chest6->isPicked = true;
				app->teamManager->loadinventory();
			}
		}

		break;
	case TUTORIAL:
		LOG("TUTORIAL MODE");
		movementRestringed = true;
		StopVelocity();
		break;
	case LEVEL_UP:
		LOG("LEVEL_UP MODE");
		movementRestringed = true;
		StopVelocity();
		break;
	case NONE:
		break;
	}

	//Movement
	if (!movementRestringed)
	{
		Movement(dt);
	}
	
	GodMode();


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//PLAYER TELEPORT
	if (teleport.turn == true)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(teleport.posX), PIXEL_TO_METERS(teleport.posY));
		b2Vec2 resetPos2 = b2Vec2(PIXEL_TO_METERS(teleport.posX+0.5f), PIXEL_TO_METERS(teleport.posY));
		pbody->body->SetTransform(resetPos, 0);
		bunnyPbody->body->SetTransform(resetPos2, 0);

		teleport.turn = false;
	}


	//Print

	UpdateAndPrintBunnyAnimation();

	UpdateAndPrintTimmyAnimation();

	// Print E key if interaction is available
	if ((itemInteractAvailable || npcInteractAvailable) && playerState == PlayerState::MOVING)
	{
		app->render->DrawTexture(eKeyTexture, position.x + 60, position.y - 60, NULL);
	}
	if (app->w2_scene->key1interact && app->w2_scene->key1state == false)
	{
		app->render->DrawTexture(eKeyTexture, position.x + 60, position.y - 60, NULL);
	}
	if (app->w2_scene->key2interact && app->w2_scene->key2state == false)
	{
		app->render->DrawTexture(eKeyTexture, position.x + 60, position.y - 60, NULL);
	}
	return true;
}

bool Player::PostUpdate() {

	

	return true;
}

void Player::UpdateAndPrintTimmyAnimation()
{

	if (currentAnimation == &walkUpAnim)
	{
		lastAnimation = &walkUpAnim;
	}
	else if (currentAnimation == &walkDownAnim)
	{
		lastAnimation = &walkDownAnim;
	}
	else if (currentAnimation == &walkRightAnim)
	{
		lastAnimation = &walkRightAnim;
	}
	else if (currentAnimation == &walkLeftAnim)
	{
		lastAnimation = &walkLeftAnim;
	}
	else
	{
		if (lastAnimation == &walkUpAnim)
		{
			currentAnimation = &upIdleAnim;
		}
		else if (lastAnimation == &walkDownAnim)
		{
			currentAnimation = &idleAnim;
		}
		else if (lastAnimation == &walkRightAnim)
		{
			currentAnimation = &rightIdleAnim;
		}
		else if (lastAnimation == &walkLeftAnim)
		{
			currentAnimation = &leftIdleAnim;
		}
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 55, position.y - 75, &rect);

}

void Player::UpdateAndPrintBunnyAnimation()
{
	

	if (bunnyCurrentAnimation == &bunnyWalkUpAnim)
	{
		lastBunnyAnimation = &bunnyWalkUpAnim;
	}
	else if (bunnyCurrentAnimation == &bunnyWalkDownAnim)
	{
		lastBunnyAnimation = &bunnyWalkDownAnim;
	}
	else if (bunnyCurrentAnimation == &bunnyWalkRightAnim)
	{
		lastBunnyAnimation = &bunnyWalkRightAnim;
	}
	else if (bunnyCurrentAnimation == &bunnyWalkLeftAnim)
	{
		lastBunnyAnimation = &bunnyWalkLeftAnim;
	}
	else
	{
		if (lastBunnyAnimation == &bunnyWalkUpAnim)
		{
			bunnyCurrentAnimation = &bunnyUpIdleAnim;
		}
		else if (lastBunnyAnimation == &bunnyWalkDownAnim)
		{
			bunnyCurrentAnimation = &bunnyIdleAnim;
		}
		else if (lastBunnyAnimation == &bunnyWalkRightAnim)
		{
			bunnyCurrentAnimation = &bunnyRightIdleAnim;
		}
		else if (lastBunnyAnimation == &bunnyWalkLeftAnim)
		{
			bunnyCurrentAnimation = &bunnyLeftIdleAnim;
		}
	}

	int bunnyPosX = METERS_TO_PIXELS(bunnyPbody->body->GetTransform().p.x-65);
	int bunnyPosY = METERS_TO_PIXELS(bunnyPbody->body->GetTransform().p.y-85);


	SDL_Rect rect2 = bunnyCurrentAnimation->GetCurrentFrame();
	//app->render->DrawTexture(bunnyTexture, position.x - 145, position.y - 115, &rect2);

	app->render->DrawTexture(bunnyTexture, bunnyPosX, bunnyPosY, &rect2);

}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(bunnyTexture);
	app->tex->UnLoad(eKeyTexture);

	if (joint != NULL)
	{
		app->physics->world->DestroyJoint(joint);
	}

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	if (bunnyPbody != NULL)
	{
		bunnyPbody->body->GetWorld()->DestroyBody(bunnyPbody->body);
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
		case ColliderType::CHEST1:
			isChest1Pickable = true;
			break;
		case ColliderType::CHEST2:
			isChest2Pickable = true;
			break;
		case ColliderType::CHEST3:
			isChest3Pickable = true;
			break;
		case ColliderType::CHEST4:
			isChest4Pickable = true;
			break;
		case ColliderType::CHEST5:
			isChest5Pickable = true;
			break;
		case ColliderType::CHEST6:
			isChest6Pickable = true;
			break;
		case ColliderType::BARRIER:
			LOG("Collision BARRIER");
			break;
		case ColliderType::DOOR:
			LOG("Collision DOOR");

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::ANGRYVILLAGER:
			LOG("Collision 	ANGRYVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::ANGRYVILLAGER;
			app->scene->isTalkingToAngry = true;
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
		case ColliderType::PIGS:
			LOG("Collision 	PIGS");
			npcInteractAvailable = true;
			lastCollision = ColliderType::PIGS;
			break;
		case ColliderType::ZORRO:
			LOG("Collision 	ZORRO");
			npcInteractAvailable = true;
			lastCollision = ColliderType::ZORRO;
			break;
		case ColliderType::DEADVILLAGER:
			LOG("Collision 	DEADVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::DEADVILLAGER;
			break;
		case ColliderType::WOLF:
			LOG("Collision 	WOLF");
			npcInteractAvailable = true;
			lastCollision = ColliderType::WOLF;
			break;
		case ColliderType::PEDRO:
			LOG("Collision 	PEDRO");
			npcInteractAvailable = true;
			lastCollision = ColliderType::PEDRO;
			break;
		case ColliderType::SHEEPA:
			LOG("Collision 	SHEEPA");
			npcInteractAvailable = true;
			lastCollision = ColliderType::SHEEPA;
			break;
		case ColliderType::SHEEPB:
			LOG("Collision 	SHEEPB");
			npcInteractAvailable = true;
			lastCollision = ColliderType::SHEEPB;
			break;
		case ColliderType::SHEEPC:
			LOG("Collision 	SHEEPC");
			npcInteractAvailable = true;
			lastCollision = ColliderType::SHEEPC;
			break;
		case ColliderType::SHEEPD:
			LOG("Collision 	SHEEPD");
			npcInteractAvailable = true;
			lastCollision = ColliderType::SHEEPD;
			break;
		case ColliderType::SECQUESTCOLLIDER:
			LOG("Collision SecondQuestCollider");
			app->scene->nextQuest();
			if (app->scene->secondQuestCollider != NULL)
			{
				app->scene->secondQuestCollider->body->SetActive(false);
			}
			break;

		case ColliderType::KEY1COLIDER:
			LOG("Collision key1colider");
			app->w2_scene->key1interact = true;
			break;
		case ColliderType::KEY2COLIDER:
			LOG("Collision key2colider");
			app->w2_scene->key2interact = true;
			break;
	}
}

void Player::EndContact(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::LRRH:
		npcInteractAvailable = false;
		break;
	case ColliderType::ITEM:
		itemInteractAvailable = false;
		break;
	case ColliderType::CHEST1:
		isChest1Pickable = false;
		break;
	case ColliderType::CHEST2:
		isChest2Pickable = false;
		break;
	case ColliderType::CHEST3:
		isChest3Pickable = false;
		break;
	case ColliderType::CHEST4:
		isChest4Pickable = false;
		break;
	case ColliderType::CHEST5:
		isChest5Pickable = false;
		break;
	case ColliderType::CHEST6:
		isChest6Pickable = false;
		break;
	case ColliderType::BARRIER:
		break;
	case ColliderType::DOOR:
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::ANGRYVILLAGER:
		npcInteractAvailable = false;
		app->scene->isTalkingToAngry = false;
		break;
	case ColliderType::TALISMANVILLAGER:
		npcInteractAvailable = false;
		break;
	case ColliderType::PIGS:
		npcInteractAvailable = false;
		break;
	case ColliderType::ZORRO:
		npcInteractAvailable = false;
		break;
	case ColliderType::GRANDMA:
		npcInteractAvailable = false;
		break;
	case ColliderType::KEY1COLIDER:
		app->w2_scene->key1interact = false;
		break;
	case ColliderType::KEY2COLIDER:
		app->w2_scene->key2interact = false;
		break;
	default:
		break;
	}
}

//This function toggles the player's god mode when the F10 key is pressed
void Player::GodMode()
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode)
		{
			godMode = false;
		}
		else
		{
			godMode = true;
		}
	}
}

//This function checks for input from the player's keyboard and updates the dialogue tree in the game's scene accordingly. The function checks if any button is being pressed, and if so, it calls the UpdateDialogueTree() function in the scene and passes it an integer value from 1 to 4, depending on which button was pressed.
void Player::InteractWithTree()
{
	if (app->scene->active)
	{
		if (buttonOption1)
		{
			app->scene->UpdateDialogueTree(1);
			app->uiModule->CleaningDialogeOverTime();
			buttonOption1 = false;
		}
		else if (buttonOption2)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->scene->UpdateDialogueTree(2);
			buttonOption2 = false;
		}
		else if (buttonOption3)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->scene->UpdateDialogueTree(3);
			buttonOption3 = false;
		}
		else if (buttonOption4)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->scene->UpdateDialogueTree(4);
			buttonOption4 = false;
		}
	}
	else if (app->w2_scene->active)
	{
		if (buttonOption1)
		{
			app->w2_scene->UpdateDialogueTree(1);
			app->uiModule->CleaningDialogeOverTime();
			buttonOption1 = false;
		}
		else if (buttonOption2)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w2_scene->UpdateDialogueTree(2);
			buttonOption2 = false;
		}
		else if (buttonOption3)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w2_scene->UpdateDialogueTree(3);
			buttonOption3 = false;
		}
		else if (buttonOption4)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w2_scene->UpdateDialogueTree(4);
			buttonOption4 = false;
		}
	}
	else if (app->w3_scene->active) {
		if (buttonOption1)
		{
			app->w3_scene->UpdateDialogueTree(1);
			app->uiModule->CleaningDialogeOverTime();
			buttonOption1 = false;
		}
		else if (buttonOption2)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w3_scene->UpdateDialogueTree(2);
			buttonOption2 = false;
		}
		else if (buttonOption3)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w3_scene->UpdateDialogueTree(3);
			buttonOption3 = false;
		}
		else if (buttonOption4)
		{
			app->uiModule->CleaningDialogeOverTime();
			app->w3_scene->UpdateDialogueTree(4);
			buttonOption4 = false;
		}
	}
}

//This function takes a ColliderType parameter and runs the corresponding dialogue tree in the game's scene
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
	case ColliderType::PIGS:
		app->w2_scene->RunDialogueTree(ColliderType::PIGS);
		break;
	case ColliderType::ZORRO:
		app->w2_scene->RunDialogueTree(ColliderType::ZORRO);
		break;
	case ColliderType::DEADVILLAGER:
		app->w3_scene->RunDialogueTree(ColliderType::DEADVILLAGER);
		break;
	case ColliderType::WOLF:
		app->w3_scene->RunDialogueTree(ColliderType::WOLF);
		break;
	case ColliderType::PEDRO:
		app->w3_scene->RunDialogueTree(ColliderType::PEDRO);
		break;
	case ColliderType::SHEEPA:
		app->w3_scene->RunDialogueTree(ColliderType::SHEEPA);
		break;
	case ColliderType::SHEEPB:
		app->w3_scene->RunDialogueTree(ColliderType::SHEEPB);
		break;
	case ColliderType::SHEEPC:
		app->w3_scene->RunDialogueTree(ColliderType::SHEEPC);
		break;
	case ColliderType::SHEEPD:
		app->w3_scene->RunDialogueTree(ColliderType::SHEEPD);
		break;

	case ColliderType::BUNNY:
		app->scene->RunDialogueTree(ColliderType::BUNNY);

		break;

	default:
		break;
	}
}

//This function handles the player's movement in the game. The function also sets the player's animation based on their direction of movement.
void Player::Movement(float dt)
{
	vel = b2Vec2(0, 0);

	bool isMovingH, isMovingV;

	isMovingH = HorizontalMovement(dt);
	isMovingV = VerticalMovement(dt);
	bool isRunning = SprintMovement(dt);


	if (!isMovingH && !isMovingV)
	{
		currentAnimation = &idleAnim;
		bunnyCurrentAnimation = &bunnyIdleAnim;

	}
		

	if (godMode)
	{
		if(!isRunning)
		vel *= 1.5 * 2;
	}

	pbody->body->SetLinearVelocity(vel);
	//bunnyPbody->body->SetLinearVelocity(vel);

}

//This function checks if the player is holding down the left shift key to sprint. If the key is being held down, the function increases the player's velocity and sets the animation speed to a faster value
bool Player::SprintMovement(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || app->input->pad->r2)
	{
		vel *= 1.5;

		walkDownAnim.speed = 0.45f;
		walkUpAnim.speed = 0.45f;
		walkRightAnim.speed = 0.45f;
		walkLeftAnim.speed = 0.45f;

		bunnyWalkDownAnim.speed = 0.25f;
		bunnyWalkUpAnim.speed = 0.25f;
		bunnyWalkRightAnim.speed = 0.25f;
		bunnyWalkLeftAnim.speed = 0.25f;

		return true;
	}
	else
	{
		walkDownAnim.speed = 0.30f;
		walkUpAnim.speed = 0.30f;
		walkRightAnim.speed = 0.30f;
		walkLeftAnim.speed = 0.30f;

		bunnyWalkDownAnim.speed = 0.10f;
		bunnyWalkUpAnim.speed = 0.10f;
		bunnyWalkRightAnim.speed = 0.10f;
		bunnyWalkLeftAnim.speed = 0.10f;

		return false;
	}
}

//Handles the player vertical movement.
bool Player::VerticalMovement(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->left_y < -0.5) {
		vel.y = -speed * dt;
		currentAnimation = &walkUpAnim;
		bunnyCurrentAnimation = &bunnyWalkUpAnim;
		app->audio->PlayFx(walkFx);
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->left_y > 0.5) {
		vel.y = speed * dt;
		currentAnimation = &walkDownAnim;
		bunnyCurrentAnimation = &bunnyWalkDownAnim;
		app->audio->PlayFx(walkFx);
		return true;
	}

	return false;
}

//Handles the player horizontal movement.
bool Player::HorizontalMovement(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->left_x < -0.5)
	{
		vel.x = -speed * dt;
		currentAnimation = &walkLeftAnim;
		bunnyCurrentAnimation = &bunnyWalkLeftAnim;
		app->audio->PlayFx(walkFx);
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->left_x > 0.5)
	{
		vel.x = speed * dt;
		currentAnimation = &walkRightAnim;
		bunnyCurrentAnimation = &bunnyWalkRightAnim;
		app->audio->PlayFx(walkFx);
		return true;
	}

	return false;

}

//This function handles the player's interaction with other entities in the game. It checks if the player is near an NPC or item that can be interacted with, and if so, it sets the player's state to either NPC_INTERACT or ITEM_INTERACT. If the player is already in one of these states, the function resets the player's state to MOVING.
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

				// Tell to UIModule which currentMenuType
				app->uiModule->currentMenuType = DISABLED;
				// Call this function only when buttons change
				app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			}
			// Moving
			else
			{
				
				playerPrevState = playerState;
				playerState = NPC_INTERACT;
				StopVelocity();

				dialogueActivate = true;
			}
		}

		else if (itemInteractAvailable)
		{
			// If already interacting, stop interact
			if (playerState == ITEM_INTERACT)
			{
				playerState = MOVING;
				itemInteractAvailable = false;

				// Tell to UIModule which currentMenuType
				app->uiModule->currentMenuType = DISABLED;
				// Call this function only when buttons change
				app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			}
			// If moving, start interact
			else
			{
				playerPrevState = playerState;
				playerState = ITEM_INTERACT;


				StopVelocity();
			}
		}
	}
}

//This function sets the player's velocity to zero and sets the player's animation to the idle animation. This is used to stop the player's movement when they are interacting with an NPC or item.
void Player::StopVelocity()
{
	vel = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(vel);
	//bunnyPbody->body->SetLinearVelocity(vel);
	currentAnimation = &idleAnim;
	bunnyCurrentAnimation = &bunnyIdleAnim;
}

void Player::ChangePosition(int x, int y)
{

 	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}

