#include "Bunny.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "TeamManager.h"
#include "BattleManager.h"

Bunny::Bunny() : Entity(EntityType::BUNNY)
{
	name.Create("bunny");
	namechar.Create("Bunny");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
	

}

Bunny::~Bunny() {

}

bool Bunny::Awake()
{

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	abilityAnim.PushBack({ 300, 0, 140, 140 });
	abilityAnim.PushBack({ 450, 0, 140, 140 });
	abilityAnim.loop = false;
	abilityAnim.speed = 0.75;
	
	for (int i = 4; i < 8; i++) //penutlima:cabezon
	{
		thunder.PushBack({ (i*150), 0, 150, 600 });
	}
	thunder.loop = false;
	thunder.speed = 0.35f;

	for (int i = 0; i < 4; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 4; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 4; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 4; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;

	texture = app->tex->Load("Assets/Characters/F_sprites_bunny-atack.png");
	IconGtexture = app->tex->Load("Assets/UI/BunnyIconG.png");
	Icontexture = app->tex->Load("Assets/UI/BunnyIcon.png");
	currentAnimation = &idleAnim;
	abilityAnimation = &none;
	PrevPos = position;


	// Attacks description
	attackDescription = "Cast a magic thunder";
	abilityDescription = "Cast a magic super thunder!";

	return true;
}

bool Bunny::Start()
{
	PrevPos = position;
	attackFx = app->audio->LoadFx("Assets/Sounds/FX/fx_attack.wav");
	abilityFx = app->audio->LoadFx("Assets/Sounds/FX/fx_lightning.wav");
	prehealth = health;
	return true;
}

bool Bunny::Update(float dt) 
{
	currentAnimation->Update();
	switch (battleState) {
	case IDLE:
		break; 
	case THINKING:
		break; 
	case MOVING:
		break; 
	case ATTACKING:
		break; 

	}
	

	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsBunnyOnTeam) {

		currentAnimation->Update();
		abilityAnimation->Update();
		
	
		if ((app->battleManager->actionType == ActionType::ATTACK || app->battleManager->actionType == ActionType::ABILITY) && app->battleManager->battleState == BattleState::INACTION)
		{
			if (this->name == app->battleManager->currentTurn->name)
			{
				currentAnimation = &abilityAnim;
				abilityAnimation = &thunder;

				thunderPos.x = app->battleManager->targetPosForAnimation.x;
				thunderPos.y = app->battleManager->targetPosForAnimation.y;

			}


		}
		else if (position.x > PrevPos.x)
		{
			currentAnimation = &walkRightAnim;
		}
		//Moverse a la izquierda
		else if (position.x < PrevPos.x)
		{
			currentAnimation = &walkLeftAnim;
		}
		//Moverse a abajo
		else if (position.y > PrevPos.y)
		{
			currentAnimation = &walkDownAnim;
		}
		//Moverse a arriba
		else if (position.y < PrevPos.y)
		{
			currentAnimation = &walkUpAnim;
		}
		else if (position.y == PrevPos.y && position.x == PrevPos.x)
		{
			if (frames > 60)
			{
				currentAnimation = &idleAnim;
				frames = 0;
			}
			else
			{
				frames++;
			}

		}


		PrevPos.x = position.x;
		PrevPos.y = position.y;
	}

	if (health < prehealth) {

		currentAnimation = &takedmgAnim;
		prehealth = health;
	}
	return true;
}

bool Bunny::PostUpdate()
{
	if (health > 0) {
		if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsBunnyOnTeam) {
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
		}

		if (abilityAnimation != &none)
		{
			SDL_Rect rect = abilityAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, thunderPos.x, thunderPos.y - 600 + 150, &rect);

			if (thunder.HasFinished())
			{
				abilityAnimation->Reset();
				abilityAnimation = &none;
				

			}
		}
	}


	return true;
}

bool Bunny::CleanUp()
{
	return true;
}

int Bunny::Attack()
{
	stamina -= 1;
	return attack;

}

int Bunny::Ability(int id)
{
	//If ability 1 is selected
	if (id == 1) {
		stamina -= 3;
		if (Ab1Type == 1) {
			
			return attack * Ab1Power;

		}
		if (Ab1Type == 2) {

			return magic * Ab1Power;

		}
		if (Ab1Type == 3) {

			return healingpower;

		}
	}
	//If ability 2 is selected
	if (id == 2) {
		stamina -= 5;
		if (Ab1Type == 1) {

			return attack * Ab1Power;

		}
		if (Ab1Type == 2) {

			return magic * Ab1Power;

		}
		if (Ab1Type == 3) {

			return healingpower * 1.5;

		}
	}
}

void Bunny::Movement() {

}

void Bunny::TakeDamageAnimation()
{

	currentAnimation = &takedmgAnim;

}

void Bunny::ResetStats() {

	id = 1;
	health = 21;
	maxHealth = 21;
	defense = 1;
	magic = 7;
	stamina = 15;
	maxStamina = 15;
	speed = 8;
	attack = 8;
	AttArea = 3;
	Ab1Type = 1;
	Ab1Area = 3;
	Ab1RangeType = 1;
	Ab1Power = 20;
	Ab2Type = 0;
	Ab2Area = 0;
	Ab2RangeType = 0;
	Ab2Power = 0;
	healingpower = 0;
	movement = 4;
}