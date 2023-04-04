#ifndef __NONPLAYABLE_H__
#define __NONPLAYABLE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

enum class NonPlayableType {

	PLAYER,
	NPC,
	ITEM

};

class NonPlayable : public Entity {
public:

	NonPlayable(NonPlayableType type_): Entity(EntityType::NONPLAYABLE){
		NonType = type_;
	}

	virtual ~NonPlayable();

	//player interacts with Nonplayable
	//void Interact();


private:


public:
	NonPlayableType NonType;
	enum NonPlayableState
	{
		UNKNOWN = -1,
		IDLE,
		INTERACT, 
	};


private:

	/*PhysBody* pbody;
	SDL_Texture* texture;
	const char* texturePath;
	Animation* animation;*/

};

#endif // __NONPLAYABLE_H__
