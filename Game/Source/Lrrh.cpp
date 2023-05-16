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
Lrrh::Lrrh() : Entity(EntityType::LRRH)
{
	name.Create("lrrh");
	namechar.Create("---RedHoody---");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
}

Lrrh::~Lrrh() {

}

bool Lrrh::Awake()
{
	if (app->teamManager->statsdone == false) {
		id = 3;
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
		 
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
	return true;

	if (app->uiModule->currentMenuType == COMBAT) {
		currentAnimation->Update();

		if (position.x > PrevPos.x)
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


}

bool Lrrh::PostUpdate()
{

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
