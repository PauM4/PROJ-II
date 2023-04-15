#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Npc : public Entity
{
public:

	Npc();

	virtual ~Npc();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

private:
	
	Animation* currentAnimation;

	SDL_Texture* npcTexture;
	const char* npcTexturePath;

	iPoint posAngryVillager;
	PhysBody* pbodyAVillager;
	Animation angryVillagerAnimation;

	iPoint posTalismanVillager;
	PhysBody* pbodyTLVillager;
	Animation talismanVillagerAnimation;

	iPoint posGrandma;
	PhysBody* pbodyGrandma;
	Animation grandmaAnimation;

	iPoint posLRRH;
	PhysBody* pbodyLRRH;
	Animation lrrhAnimation;

};

#endif // __NPC_H__