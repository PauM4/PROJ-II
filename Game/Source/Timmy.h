	#ifndef __TIMMY_H__
#define __TIMMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

enum class AnimDirection
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
class Timmy : public Entity {
public:

	Timmy();
	
	virtual ~Timmy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;

	void TakeDamageAnimation();

	void ResetStats() override;

	AnimDirection CheckDirection();

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
	Animation attackRAnim;
	Animation attackLAnim;
	Animation attackDAnim;
	Animation attackUAnim;
	Animation abilityAnim;
	
	AnimDirection animDirection;

	int prehealth;

private:

	
	
	
};

#endif // __TIMMY_H__
