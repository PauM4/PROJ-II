#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:

	bool isPicked;
	int width, height;
	int chestId;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __ITEM_H__