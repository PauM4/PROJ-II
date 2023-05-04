#ifndef __NPCW3_H__
#define __NPCW3_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class NpcW3 : public Entity
{
public:

	NpcW3();

	virtual ~NpcW3();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	// TOT AIXO ABANS ESTAVA A PRIVATE :)
	
	Animation* currentAnimation;

	SDL_Texture* NpcW3Texture;
	const char* NpcW3TexturePath;

	iPoint posDeadVillager;
	PhysBody* pbodyDeadVillager;
	Animation deadVillagerAnimation;


	iPoint posWolf;
	PhysBody* pbodyWolf;
	Animation wolfAnimation;


	iPoint posPedro;
	PhysBody* pbodyPedro;
	Animation pedroAnimation;


	iPoint posSheepA;
	PhysBody* pbodySheepA;
	Animation sheepAAnimation;

	iPoint posSheepB;
	PhysBody* pbodySheepB;
	Animation sheepBAnimation;

	iPoint posSheepC;
	PhysBody* pbodySheepC;
	Animation sheepCAnimation;

	iPoint posSheepD;
	PhysBody* pbodySheepD;
	Animation sheepDAnimation;



};

#endif // __NPCW3_H__