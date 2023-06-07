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

	iPoint posWolf;
	PhysBody* pbodyWolf;
	Animation wolfAnimation;







};

#endif // __NPCW3_H__