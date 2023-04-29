	#ifndef __TEAMMANAGER_H__
#define __TEAMMANAGER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "Timmy.h"
#include "Bunny.h"
class TeamManager : public Module {
public:

	TeamManager(bool isActive);
	
	virtual ~TeamManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


private:
	

public:
	//List of acquaried playable characters
	List<Entity*> characters;
	//List of the selected playable characters in the team
	List<Entity*> team;

private:

	
	
	
};

#endif // __TEAMMANAGER_H__
