#pragma once
#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Portal : public Entity
{
public:

	Portal();

	virtual ~Portal();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	static void TriggerPortal(int scene);

private:

	bool godMode;

	SDL_Texture* texture;
	const char* texturePath;

	PhysBody* pbody;

	int nextScene;

	Animation* currentAnimation; 
	Animation idleAnim; 


	int width, height;
	int nextX, nextY;
};

#endif // __PORTAL_H__