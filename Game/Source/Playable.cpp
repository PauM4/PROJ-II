#include "Playable.h"

Playable::Playable() : Entity(EntityType::PLAYABLE)
{
	name.Create("Playable");
}

Playable::~Playable() {

}

int Playable::Attack()
{
	stamina -= 1;
	return attack;

}

int Playable::Ability(int id)
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


void Playable::Movement()
{

 }

bool Playable::TakeDamage(uint reciveattack)
{

	health = health - (reciveattack - defense);

	return true;
}

bool Playable::TakeHealing(uint recievedhealing)
{
	health += recievedhealing;
	if (health > maxhealth) {
		health = maxhealth;
	}
	return true;
}
