#ifndef __PLAYABLE_H__
#define __PLAYABLE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

class Playable : public Entity {
public:

	Playable();
	
	virtual ~Playable();

	virtual int Attack();

	virtual int Ability(int id);

	virtual void Movement();
	
	bool TakeDamage(uint reciveattack);

	bool TakeHealing(uint recievedhealing);


private:
	

public:

	enum PlayableState
	{
		UNKNOWN = -1,
		OUTOFCOMBAT,
		THINKING,
		MOVING,
		ATTACKING,
		IDLE,
		
	};
	//Character Stats
	uint level;
	uint health;
	uint maxhealth;
	uint defense;
	uint magic;
	uint stamina;
	uint maxstamina;
	uint speed;
	uint skill;
	uint attack;
	iPoint AttArea;
	uint Ab1Power;
	uint Ab2Power;
	//1 if melee 2 if mage 3 if healing;
	uint Ab1Type;
	//1 if melee 2 if mage 3 if healing;
	uint Ab2Type;
	iPoint Ab1Area;
	iPoint Ab2Area;
	uint healingpower;

private:


	
};

#endif // __PLAYABLE_H__
