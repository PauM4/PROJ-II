	#ifndef __LRRH_H__
#define __LRRH_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

class Lrrh : public Entity {
public:

	Lrrh();
	
	virtual ~Lrrh();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;

	void TakeDamageAnimation();

	void ResetStats();


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
	Animation attackAnim;
	Animation abilityAnim;

	Animation* abilityAnimation;
	Animation arrow;
	Animation none;

	iPoint arrowPos;

	bool finishAnimBool;

	int prehealth;
private:

	
	
	
};

#endif // __TIMMY_H__
