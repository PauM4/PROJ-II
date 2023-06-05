#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;

struct transformPosition {
	float posX;
	float posY;
	bool turn;

};

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

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void EndContact(PhysBody* physA, PhysBody* physB);

	// Magic function that allows the player to teleport :D
	void Player::ChangePosition(int x, int y);
	void StopVelocity();

private:
	void TriggerDialogueTree(ColliderType NPC);
	void InteractWithTree();
	void Movement(float dt);
	bool VerticalMovement(float dt);
	bool HorizontalMovement(float dt);
	bool SprintMovement(float dt);
	void InteractWithEntities();
	void GodMode();
	void UpdateAndPrintBunnyAnimation();
	void UpdateAndPrintTimmyAnimation();


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
		TUTORIAL,
		LEVEL_UP,
		NONE
	};

	int playerState;
	int playerPrevState;

	ColliderType lastCollision;
	float speed;
	b2Vec2 vel;

	transformPosition teleport;

	// Bool to print once the dialogues
	bool dialogueActivate;

	bool buttonOption1, buttonOption2, buttonOption3, buttonOption4;

	int walkFx = -1;
	int chestFX = -1;

private:

	PhysBody* pbody;
	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnimation;
	Animation* lastAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation rightIdleAnim;
	Animation leftIdleAnim;
	Animation upIdleAnim;

	PhysBody* bunnyPbody;
	SDL_Texture* bunnyTexture;
	const char* bunnyTexturePath;

	Animation* bunnyCurrentAnimation;
	Animation* lastBunnyAnimation;
	Animation bunnyWalkDownAnim;
	Animation bunnyWalkUpAnim;
	Animation bunnyWalkRightAnim;
	Animation bunnyWalkLeftAnim;
	Animation bunnyIdleAnim;
	Animation bunnyRightIdleAnim;
	Animation bunnyLeftIdleAnim;
	Animation bunnyUpIdleAnim;

	b2PrismaticJointDef prismaticJointDef;
	b2RopeJoint* joint;

	SDL_Texture* eKeyTexture;


	bool godMode;

public:
	bool isChest1Pickable;
	bool isChest2Pickable;
	bool isChest3Pickable;
	bool isChest4Pickable;
	bool isChest5Pickable;
	bool isChest6Pickable;


};

#endif // __PLAYER_H__