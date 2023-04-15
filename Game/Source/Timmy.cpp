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
	name.Create("timmy");
	isAlive = true;
	battleState = IDLE; 
	isEnemy = false;
}

Timmy::~Timmy() {

}

bool Timmy::Awake()
{
	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	speed = stats.attribute("speed").as_int();
	movement = stats.attribute("movement").as_int();
	return true;
}

bool Timmy::Start()
{


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
