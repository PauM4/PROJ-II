	#ifndef __LPIG_H__
#define __LPIG_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

class LittlePig : public Entity {
public:

	LittlePig();
	
	virtual ~LittlePig();

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
	Animation abilityAnim;
	Animation* abilityAnimation;

	bool finishAnimBool;

private:

	
	
	
};

#endif // __TIMMY_H__
