#include "MiddlePig.h"
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

MiddlePig::MiddlePig() : Entity(EntityType::MPIG)
{
	name.Create("middlePig");
	namechar.Create("Middle Pig");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
}

MiddlePig::~MiddlePig() {

}

bool MiddlePig::Awake()
{

	idleAnim.PushBack({ 0, 0, 210, 210 });
	idleAnim.loop = true;

	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 225), 225, 225, 225 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 225), 900, 225, 225 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 225), 600, 225, 225 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 225), 450, 225, 225 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;

	currentAnimation = &idleAnim;
	texture = app->tex->Load("Assets/Characters/Sprites_Cerdo_Mediano.png");

	PrevPos = position;
	return true;
}

bool MiddlePig::Start()
{


	return true;
}

bool MiddlePig::Update(float dt)
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

	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsMidPigOnTeam==true){
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

bool MiddlePig::PostUpdate()
{
	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsMidPigOnTeam == true) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 50, position.y - 120, &rect);
	}
	return true;
}

bool MiddlePig::CleanUp()
{
	return true;
}

int MiddlePig::Attack()
{
	stamina -= 1;
	return attack;

}

int MiddlePig::Ability(int id)
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

void MiddlePig::Movement() {

}

void MiddlePig::TakeDamageAnimation()
{

	currentAnimation = &takedmgAnim;

}
void MiddlePig::ResetStats() {

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
