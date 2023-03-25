#ifndef __NONPLAYABLE_H__
#define __NONPLAYABLE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

class NonPlayable : public Entity {
public:

	NonPlayable();

	virtual ~NonPlayable();

	//player interacts with Nonplayable
	void Interact();


private:


public:

	enum NonPlayableState
	{
		UNKNOWN = -1,
		IDLE,
		INTERACT, 
	};


private:

	PhysBody* pbody;
	SDL_Texture* texture;
	const char* texturePath;
	Animation* animation;

};

#endif // __NONPLAYABLE_H__
