#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

class PhysBody;

enum class State {

	UNKNOWN = -1,
	BATTLE,
	SCENE
};

enum class EntityType
{
	UNKNOWN = -1,

	//Battle
	
	TIMMY,
	BUNNY,

	//Scene

	PLAYER,
	NPC,
	ITEM
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {
	
	};

	virtual void LoadInfo(iPoint pos, int state)
	{

	}

	virtual void SaveInfo()
	{

	}

	virtual int GetState()
	{
		return 0;
	}

	//In Combat Functions

	virtual int Attack() { return 0; };

	virtual int Ability(int id) { return 0; };

	virtual void Movement() { return; };

	bool TakeDamage(uint reciveattack) {

		health = health - (reciveattack - defense);

		return true;
	};

	bool TakeHealing(uint recievedhealing) {

		health += recievedhealing;
		if (health > maxhealth) {
			health = maxhealth;
		}

		return true;
	};

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	//In Combat Variables

	enum BattleState
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
};

#endif // __ENTITY_H__