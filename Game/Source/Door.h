#ifndef __DOOR_H__
#define __DOOR_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Door : public Entity
{
public:

	Door();

	virtual ~Door();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	static void TriggerDoor(int scene);

private:
	
	bool godMode;

	SDL_Texture* texture;
	const char* texturePath;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;

	int nextScene;


	int width, height;
	int nextX, nextY;
};

#endif // __DOOR_H__