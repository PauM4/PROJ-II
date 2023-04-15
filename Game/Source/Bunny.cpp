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

Bunny::Bunny() : Entity(EntityType::BUNNY)
{
	name.Create("bunny");
	isAlive = true;
	battleState = IDLE; 

}

Bunny::~Bunny() {

}

bool Bunny::Awake()
{
	
	level = 1;
	health = 15;
	maxHealth = 15;
	defense = 5;
	magic = 5;
	stamina = 15;
	maxStamina = 15;
	speed = 6;
	attack = 3;
	AttArea = 1;
	Ab1Type = 3;
	Ab1Area = 1;
	Ab1RangeType = 3;
	Ab1Power = 2;
	Ab2Type = 2;
	Ab2Area = 3;
	Ab2RangeType = 2;
	Ab2Power = 2;
	healingpower = 8;
	isEnemy = false;

	return true;
}

bool Bunny::Start()
{


	return true;
}

bool Bunny::Update()
{

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


	
}

bool Bunny::PostUpdate()
{
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
