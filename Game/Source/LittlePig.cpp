#include "LittlePig.h"
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

LittlePig::LittlePig() : Entity(EntityType::LPIG)
{
	name.Create("littlePig");
	namechar.Create("---Little Pig---");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
}

LittlePig::~LittlePig() {

}

bool LittlePig::Awake()
{
	if (app->teamManager->statsdone == false) {
		id = 1;
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
		health = stats.attribute("health").as_int();
		maxHealth = stats.attribute("maxHealth").as_int();
		defense = stats.attribute("defense").as_int();
		magic = stats.attribute("magic").as_int();
		stamina = stats.attribute("stamina").as_int();
		maxStamina = stats.attribute("maxStamina").as_int();
		speed = stats.attribute("speed").as_int();
		attack = stats.attribute("attack").as_int();
		AttArea = stats.attribute("AttArea").as_int();
		Ab1Type = stats.attribute("Ab1Type").as_int();
		Ab1Area = stats.attribute("Ab1Area").as_int();
		Ab1RangeType = stats.attribute("Ab1RangeType").as_int();
		Ab1Power = stats.attribute("Ab1Power").as_int();
		Ab2Type = stats.attribute("Ab2Type").as_int();
		Ab2Area = stats.attribute("Ab2Area").as_int();
		Ab2RangeType = stats.attribute("Ab2RangeType").as_int();
		Ab2Power = stats.attribute("Ab2Power").as_int();
		healingpower = stats.attribute("healingpower").as_int();
		movement = stats.attribute("movement").as_int();
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
	

	texture = app->tex->Load("Assets/Characters/Sprites_Cerdo_Pequeño.png");

	currentAnimation = &idleAnim;

	PrevPos = position;

	return true;
}

bool LittlePig::Start()
{


	return true;
}

bool LittlePig::Update(float dt)
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

	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsLilPigOnTeam==true) {
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
	return true;


}

bool LittlePig::PostUpdate()
{
	if (app->uiModule->currentMenuType == COMBAT && app->teamManager->IsLilPigOnTeam == true) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
	}

	return true;
}

bool LittlePig::CleanUp()
{
	return true;
}

int LittlePig::Attack()
{
	stamina -= 1;
	return attack;

}

int LittlePig::Ability(int id)
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

void LittlePig::Movement() {

}

void LittlePig::TakeDamageAnimation()
{

	currentAnimation = &takedmgAnim;

}
