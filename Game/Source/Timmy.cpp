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

Timmy::Timmy() : Entity(EntityType::TIMMY)
{
	name.Create("Timmy");
	isAlive = true;
	battleState = IDLE; 
}

Timmy::~Timmy() {

}

bool Timmy::Awake()
{
	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	speed = parameters.attribute("speed").as_int();
	movement = parameters.attribute("movement").as_int();
	return true;
}

bool Timmy::Start()
{
	level = 1;
	health = 20;
	maxHealth = 20;
	defense = 5;
	magic = 1;
	stamina = 15;
	maxStamina = 15;
	speed = 5;
	attack = 6;
	AttArea = 1;
	Ab1Type = 1;
	Ab1Area = 4;
	Ab1RangeType = 1;
	Ab1Power = 2;
	Ab2Type = 2;
	Ab2Area = 3;
	Ab2RangeType = 1;
	Ab2Power = 3;
	healingpower = 0;
	

	return true;
}

bool Timmy::Update()
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




}

bool Timmy::PostUpdate()
{

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
