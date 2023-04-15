	#ifndef __BUNNY_H__
#define __BUNNY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"

class Bunny : public Entity {
public:

	Bunny();
	
	virtual ~Bunny();

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

#endif // __Bunny_H__
