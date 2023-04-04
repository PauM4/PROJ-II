#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Npc : public Entity
{
public:

	Npc();

	virtual ~Npc();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

private:

	SDL_Texture* texture;
	const char* texturePath;

	PhysBody* pbody;


};

#endif // __NPC_H__