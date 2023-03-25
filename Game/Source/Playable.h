#ifndef __PLAYABLE_H__
#define __PLAYABLE_H__ç

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

class Playable : public Entity {
public:

	Playable();
	
	virtual ~Playable();

	virtual void AttackArea();

	virtual void AbilityArea();

	bool TakeDamage(uint attack);

private:


public:


private:

	//Character Stats
	uint level;
	uint health;
	uint movement;
	uint attack;
	uint defense;
	uint magic;
	uint resistence;
	uint speed;
	uint skill;

	List<Entity*> targets;


};

#endif // __ENTITY_H__
