#include "Timmy.h"
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

Timmy::Timmy() : Entity(EntityType::TIMMY)
{
	name.Create("timmy");
	namechar.Create("Timmy");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
}

Timmy::~Timmy() {

}

bool Timmy::Awake()
{

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	attackDAnim.PushBack({ 59, 808, 150, 185 });
	attackDAnim.loop = true;
	attackUAnim.PushBack({ 982,822, 150, 155 });
	attackUAnim.loop = true;
	attackRAnim.PushBack({ 677, 828, 150, 175 });
	attackRAnim.loop = true;
	attackLAnim.PushBack({ 370, 827, 150, 175 });
	attackLAnim.loop = true;


	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;


	abilityAnim.PushBack({1671,94,404,404 });
	abilityAnim.PushBack({ 1671,541,404,404 });
	abilityAnim.PushBack({ 1671,94,404,404 });
	abilityAnim.PushBack({ 1671,541,404,404 });
	abilityAnim.PushBack({ 1671,94,404,404 });
	abilityAnim.PushBack({ 1671,541,404,404 });
	abilityAnim.loop = true;
	abilityAnim.speed = 0.05f;


	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;

	texture = app->tex->Load("Assets/Characters/F_sprites_timmy.png");
	IconGtexture = app->tex->Load("Assets/UI/TimmyIconG.png");
	Icontexture = app->tex->Load("Assets/UI/TimmyIcon.png");
	currentAnimation = &idleAnim;
	PrevPos = position;

	// Attacks description
	attackDescription = "Melee attack with the yo-yo";
	abilityDescription = "Circular attack with the yo-yo";

	return true;
}

bool Timmy::Start()
{
	attackFx = app->audio->LoadFx("Assets/Sounds/FX/fx_attack.wav");
	abilityFx = app->audio->LoadFx("Assets/Sounds/FX/fx_yoyo.wav");
	PrevPos = position;
	frames = 0;
	prehealth = health;
	return true;
}

bool Timmy::Update(float dt)
{
	

	b2Vec2 vel = b2Vec2(0, 0);

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
	

	if (app->uiModule->currentMenuType == COMBAT) {
		currentAnimation->Update();

		if (app->battleManager->actionType == ActionType::ATTACK && app->battleManager->battleState == BattleState::INACTION)
		{
			if (this->name == app->battleManager->currentTurn->name)
			{
				animDirection = CheckDirection();

				switch (animDirection)
				{
				case AnimDirection::DOWN:
					currentAnimation = &attackDAnim;
					break;
				case AnimDirection::UP:
					currentAnimation = &attackUAnim;
					break;
				case AnimDirection::RIGHT:
					currentAnimation = &attackRAnim;
					break;
				case AnimDirection::LEFT:
					currentAnimation = &attackLAnim;
					break;
				}
			}
			

		}
		else if (app->battleManager->actionType == ActionType::ABILITY && app->battleManager->battleState == BattleState::INACTION)
		{
			if (this->name == app->battleManager->currentTurn->name)
			{
				currentAnimation = &abilityAnim;
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

	if (health < prehealth || health <=0) {

		currentAnimation = &takedmgAnim;
		prehealth = health;
	}

	return true;
}

AnimDirection Timmy::CheckDirection()
{
	iPoint dist;
	
	dist.x = app->battleManager->targetPosForAnimation.x - app->battleManager->currentTurn->position.x;
	dist.y = app->battleManager->targetPosForAnimation.y - app->battleManager->currentTurn->position.y;


	int xDir = 0;
	int yDir = 0;
	xDir = (dist.x > 0) ? 1 : -1;
	yDir = (dist.y > 0) ? 1 : -1;

	if (dist.x == 0) xDir = 0;
	if (dist.y == 0) yDir = 0;

	if (xDir == 1) return AnimDirection::RIGHT;
	if (xDir == -1) return AnimDirection::LEFT;
	if (yDir == 1) return AnimDirection::DOWN;
	if (yDir == -1) return AnimDirection::UP;

	return AnimDirection::NONE;
}
bool Timmy::PostUpdate()
{
	if (health > 0) {
		if (currentAnimation == &abilityAnim)
		{
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - (13 + 125), position.y - (35 + 125), &rect);
		}
		else
		{
			if (app->uiModule->currentMenuType == COMBAT) {
				SDL_Rect rect = currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
			}
		}
	}

	

	return true;
}

bool Timmy::CleanUp()
{
	return true;
}

int Timmy::Attack()
{
	stamina -= 1;
	return attack;

}

int Timmy::Ability(int id)
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

void Timmy::Movement() {

}

void Timmy::TakeDamageAnimation()
{

	currentAnimation = &takedmgAnim;

}
void Timmy::ResetStats() {

	id = 1;
	health = 20;
	maxHealth = 20;
	defense = 3;
	magic = 1;
	stamina = 15;
	maxStamina = 15;
	speed = 6;
	attack = 10;
	AttArea = 2;
	Ab1Type = 1;
	Ab1Area = 3;
	Ab1RangeType = 1;
	Ab1Power = 18;
	Ab2Type = 0;
	Ab2Area = 0;
	Ab2RangeType = 0;
	Ab2Power = 0;
	healingpower = 0;
	movement = 3;

}
