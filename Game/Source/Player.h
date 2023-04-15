#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

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

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	// Magic function that allows the player to teleport :D
	void Player::ChangePosition(int x, int y);

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


	transformPosition teleport;

private:

	PhysBody* pbody;
	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;


	SDL_Texture* walkDownTexture;

	Animation* currentAnimation;
	Animation walkDownAnim;



};

#endif // __PLAYER_H__