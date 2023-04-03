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

	virtual void Ability();

	virtual void Movement();
	
	bool TakeDamage(uint reciveattack);


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
	uint movement;
	uint defense;
	uint magic;
	uint resistence;
	uint speed;
	uint skill;
	uint attack;
	iPoint AttArea;
	uint Ab1Dmg;
	iPoint Ab1Area;
	uint Ab2Dmg;
	iPoint Ab2Area;

private:


	
};

#endif // __PLAYABLE_H__
