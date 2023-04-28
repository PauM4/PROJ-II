#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include <memory>

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
	ITEM,
	DOOR,

	//Enemies
	ENEMYLRRH,
	ANGRYVILLAGER,
	CORRUPTEDSPROUT,
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

	virtual bool Update(float dt)
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
		if (health > maxHealth) {
			health = maxHealth;
		}

		return true;
	};

	bool UseStamina(uint value) {

		stamina -= value;

		if (stamina < 0) {

			stamina = 0;
		}

		return true;
	}
	bool GainStamina(uint value) {

		stamina += value;

		if (stamina > 15) {

			stamina = 15;
		}

		return true;
	}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;
	pugi::xml_node stats;

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

	} battleState;

	//Character Stats
	uint level;
	int health;
	uint maxHealth;
	uint defense;
	uint magic;
	uint stamina;
	uint maxStamina;
	uint speed;
	uint movement;
	uint skill;
	uint attack;
	uint resistance; 
	int AttArea;
	uint Ab1Power;
	uint Ab2Power;
	//1 if physic 2 if mage 3 if healing
	uint Ab1Type;
	//0 if not able 1 if physic 2 if mage 3 if healing
	uint Ab2Type;
	//1 if lineal 2 if "circular" 3 if global
	uint Ab1RangeType;
	//1 if lineal 2 if "circular" 3 if global
	uint Ab2RangeType;
	int Ab1Area;
	int Ab2Area;
	uint healingpower;

	iPoint tilePos; 
	bool isEnemy;
	bool isAlive;
	//1 if timmy, 2 if bunny, 3 if villager, 4 i lrrh, 5 if sprout
	int id;
};

#endif // __ENTITY_H__