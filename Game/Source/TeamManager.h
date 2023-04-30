	#ifndef __TEAMMANAGER_H__
#define __TEAMMANAGER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "Timmy.h"
#include "Bunny.h"
#include "Lrrh.h"
#include <iostream>
using namespace std;

struct item
{
	//true if player has obtained this item
	bool ininventory;
	//type = 1 if weapon, 2 if armor, 3 if accessory
	int type;
	//weaponuser = 0 if noneweapon, 1 if timmy, 2 if bunny, 3 if lrrh, 4 if LilPig, 5 if MiddlePig, 6 if Peter
	int weaponuser;
	string name;
	int defense;
	int magic;
	int speed;
	int movement;
	int attack;
	int AttArea;
	int Ab1Power;
	int Ab2Power;
	int Ab1Area;
	int Ab2Area;
	int healingpower;
};

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:
	//List of acquaried playable characters
	List<Entity*> characters;
	//List of the selected playable characters in the team
	List<Entity*> team;

	Timmy* timmy;
	Bunny* bunny;
	Lrrh* lrrh;
	item yoyo;
	bool hasyoyo;
	List<item> inventory;

	//True if is a playable character
	bool istimmyplayable;
	//Truislrrhplayablee if is a playable character
	bool isbunnyplayable;
	//True if is a playable character
	bool islrrhplayable;

	//True if it's on the team
	bool IsTimmyOnTeam;
	//True if it's on the team
	bool IsBunnyOnTeam;
	//True if it's on the team
	bool IsLrrhOnTeam;

private:

	
	
	
};

#endif // __TEAMMANAGER_H__
