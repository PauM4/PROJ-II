	#ifndef __MPIG_H__
#define __MPIG_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

class MiddlePig : public Entity {

public:

	MiddlePig();
	
	virtual ~MiddlePig();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;

	void TakeDamageAnimation();

private:
	

public:
	SDL_Texture* texture;
	Animation* currentAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation takedmgAnim;
private:

	
	
	
};

#endif // __MPIG_H__
