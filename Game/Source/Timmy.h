#ifndef __TIMMY_H__
#define __TIMMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

class Timmy : public Entity {
public:

	Timmy();
	
	virtual ~Timmy();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;


private:
	

public:


private:


	
};

#endif // __TIMMY_H__
