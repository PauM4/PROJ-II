#include "Lrrh.h"
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
#include"BattleManager.h"
Lrrh::Lrrh() : Entity(EntityType::LRRH)
{
	name.Create("lrrh");
	namechar.Create("---RedHoody---");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
	prehealth = health;
}

Lrrh::~Lrrh() {

}

bool Lrrh::Awake()
{
	if (app->teamManager->statsdone == false) {
		id = 3;
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

		app->teamManager->statsdone = true;
	}
	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	//ability
	for (int i = 10; i < 12; i++)
	{
		abilityAnim.PushBack({ (i * 150), 235, 150, 195 });

	}
	for (int i = 10; i < 11; i++)
	{
		abilityAnim.PushBack({ (i * 150), 525, 150, 208 });

	}
	abilityAnim.loop = false;
	abilityAnim.speed = 0.95f;

	//arrow
	for (int i = 12; i < 15; i++)
	{
		arrow.PushBack({ (i * 150), 61, 150, 679 });
	}
	arrow.loop = false;
	arrow.speed = 0.20f;

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

	texture = app->tex->Load("Assets/Characters/F_sprites_lrrh-atack.png");

	currentAnimation = &idleAnim;
	abilityAnimation = &none;

	prehealth = health;
	PrevPos = position;
	return true;
}

bool Lrrh::Start()
{


	return true;
}

bool Lrrh::Update(float dt)
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


	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsLrrhOnTeam) {
		currentAnimation->Update();

		if ((app->battleManager->actionType == ActionType::ATTACK || app->battleManager->actionType == ActionType::ABILITY) && app->battleManager->battleState == BattleState::INACTION)
		{
			if (name == app->battleManager->currentTurn->name)
			{
				currentAnimation = &abilityAnim;
				abilityAnimation = &arrow;

				arrowPos.x = app->battleManager->targetPosForAnimation.x;
				arrowPos.y = app->battleManager->targetPosForAnimation.y;

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

	if (health < prehealth)
	{

		currentAnimation = &takedmgAnim;
		prehealth = health;
	}

	currentAnimation->Update();
	abilityAnimation->Update();

	return true;

}

bool Lrrh::PostUpdate()
{
	if (currentAnimation == &abilityAnim)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 13, position.y - (35 + 60), &rect);
	}
	else if(app->uiModule->currentMenuType == COMBAT && app->teamManager->IsLrrhOnTeam) 
	{

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
	}


	if (currentAnimation == &abilityAnim)
	{
		if (abilityAnim.HasFinished())
		{
			finishAnimBool = true;
			currentAnimation == &idleAnim;

		}
	}

	if (finishAnimBool)
	{
		if (!arrow.HasFinished())
		{
			SDL_Rect rect = abilityAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, arrowPos.x - 10, arrowPos.y - 600 + 75, &rect);
		}

		if (arrow.HasFinished())
		{
			abilityAnimation = &none;
			abilityAnim.Reset();
			arrow.Reset();
			finishAnimBool = false;

		}
	}
	return true;
}

bool Lrrh::CleanUp()
{
	return true;
}

int Lrrh::Attack()
{
	stamina -= 1;
	return attack;

}

int Lrrh::Ability(int id)
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

void Lrrh::Movement() {

}

void Lrrh::TakeDamageAnimation()
{

	currentAnimation = &takedmgAnim;

}
