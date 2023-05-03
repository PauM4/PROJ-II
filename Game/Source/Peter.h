	#ifndef __PETER_H__
#define __PETER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

class Peter : public Entity {
public:

	Peter();
	
	virtual ~Peter();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;


private:
	

public:

	Animation* currentAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation takedmgAnim;
private:

	
	
	
};

#endif // __PETER_H__
