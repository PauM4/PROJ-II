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

	virtual void Attack();

	virtual void Ability();

	virtual void Movement();

	bool TakeDamage(uint attack);

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
};

#endif // __PLAYABLE_H__
