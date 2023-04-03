#include "Playable.h"

Playable::Playable() : Entity(EntityType::PLAYABLE)
{
	name.Create("Playable");
}

Playable::~Playable() {

}

int Playable::Attack()
{
	return attack;

}

void Playable::Ability()
{

}


void Playable::Movement()
{

 }

bool Playable::TakeDamage(uint reciveattack)
{

	health = reciveattack - defense;

	return false;
}
