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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	speed = stats.attribute("speed").as_int();
	movement = stats.attribute("movement").as_int();


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
