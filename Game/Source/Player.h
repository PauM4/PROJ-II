#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

private:
	void TriggerDialogueTree(ColliderType NPC);
	void InteractWithTree();
	void Movement();
	bool VerticalMovement();
	bool HorizontalMovement();
	bool SprintMovement();
	void InteractWithEntities();
	void StopVelocity();
	void GodMode();

public:
	// This bool checks if the player is interacting with something (In the furute will be change for a state machine)
	// (In the furute will be change for a state machine)
	bool movementRestringed;


	//These two bools checks if the player can interact with an npc or an item
	bool npcInteractAvailable;
	bool itemInteractAvailable;

	enum PlayerState
	{
		PAUSE,
		INVENTORY,
		MOVING,
		BATTLE,
		NPC_INTERACT,
		ITEM_INTERACT,
		NONE
	};

	int playerState;
	int playerPrevState;

	ColliderType lastCollision;
	int speed;
	b2Vec2 vel;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	Animation* currentAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;

	bool godMode;


	
};

#endif // __PLAYER_H__