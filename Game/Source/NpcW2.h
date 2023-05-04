#ifndef __NPCW2_H__
#define __NPCW2_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class NpcW2 : public Entity
{
public:

	NpcW2();

	virtual ~NpcW2();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	// TOT AIXO ABANS ESTAVA A PRIVATE :)
	
	Animation* currentAnimation;

	SDL_Texture* npcW2Texture;
	const char* npcW2TexturePath;

	iPoint posPigs;
	PhysBody* pbodyPigs;
	Animation pigsAnimation;


};

#endif // __NPCW2_H__